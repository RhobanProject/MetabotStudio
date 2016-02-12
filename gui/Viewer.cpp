#include <iostream>
#include <sstream>
#include "Viewer.h"
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#include <QMouseEvent>
#include <math.h>

using namespace Metabot;

Viewer::Viewer(int framesPerSecond, QWidget *parent, char *name)
    : QGLWidget(parent), robot(NULL)
{
    // client.debug = true;
    mode = MODE_NORMAL;
    shouldRedraw = true;
    drawXYZ = true;
    drawGrid = true;
    alpha = 0;
    pressed = false;
    movePressed = false;
    t = 0.0;
    framesPerSecond = 24;
    setMouseTracking(true);

    setFocusPolicy(Qt::ClickFocus);

    QSizePolicy policy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    policy.setHorizontalStretch(1);
    policy.setVerticalStretch(1);
    this->setSizePolicy(policy);

    if(framesPerSecond == 0)
        t_Timer = NULL;
    else
    {
        int timerInterval = 1000 / framesPerSecond;
        t_Timer = new QTimer(this);
        connect(t_Timer, SIGNAL(timeout()), this, SLOT(timeOutSlot()));
        t_Timer->start( timerInterval );
    }
}

void Viewer::setRobot(Robot *robot_)
{
    robot = robot_;
    updateRatio();
}

void Viewer::updateRatio()
{
    tX = 0;
    tY = 0;
    setAutorotate(false);
    beta = M_PI/4.0;
    matrix = TransformMatrix::identity();

    Model m = robot->toModel();
    Point3 maxP = m.max();
    Point3 minP = m.min();
    radius = maxP.x;
    if (maxP.y > radius) radius = maxP.y;
    if (maxP.z > radius) radius = maxP.z;
    if (-minP.x > radius) radius = -minP.x;
    if (-minP.y > radius) radius = -minP.y;
    if (-minP.z > radius) radius = -minP.z;

    setPlateDimension(minP.x, minP.y, maxP.x, maxP.y, minP.z);

    radius *= 3;
}

void Viewer::updatePlate()
{
    Model m = robot->toModel();
    Point3 maxP = m.max();
    Point3 minP = m.min();
    setPlateDimension(minP.x, minP.y, maxP.x, maxP.y, minP.z);
}

void Viewer::redraw()
{
    shouldRedraw = true;
}

void Viewer::initializeGL()
{
    GLfloat specular[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat ambient[] = {0.05f, 0.05f, 0.05f, 0.0f};
    GLfloat diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};

    glShadeModel(GL_SMOOTH);
    glClearColor(1.0f, 1.0f, 1.0f, 0.2f);
    glClearStencil(0);
    glClearDepth(1.0f);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    // glEnable(GL_MODULATE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_ALPHA);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    glEnable(GL_CULL_FACE);

    // glDepthFunc(GL_LEQUAL);
    // glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void Viewer::resizeGL(int width, int height)
{
    if(height == 0) {
        height = 1;
    }
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 1000000.0f);

    // gluLookAt(2.0, 2.0, 2.0, 0, 0, 0, 0, 0, 1);
    glEnable(GL_LIGHTING);
    if (robot) {
        if (autorotate) {
            alpha += 0.02;
        }

        float aX = radius*cos(beta);
        float aY = 0;
        float aZ = radius*sin(beta);

        float X = aX*cos(alpha)-aY*sin(alpha);
        float Y = aX*sin(alpha)+aY*cos(alpha);
        float Z = aZ;

        float Xl = aX*cos(alpha+M_PI/2)-aY*sin(alpha*M_PI/2);
        float Yl = aX*sin(alpha+M_PI/2)+aY*cos(alpha*M_PI/2);

        GLfloat light0_pos[] = {Xl, Yl, Z, 0.0f};
        glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
        gluLookAt(X, Y, Z, 0, 0, 0, 0, 0, 1);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (mode == MODE_PHYSICS && client.robot) {
        auto state = client.robot->getState();
        tX = -state.x();
        tY = -state.y();
    }
}

void Viewer::paintGL()
{
    if (robot == NULL) return;

    GLfloat mat_amb_diff[] = { 0.6, 0.6, 0.6, 1.0 };
    GLfloat mat_dif_diff[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_spe_diff[] = { 0.2, 0.2, 0.2, 1.0 };

    resizeGL(size().width(), size().height());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    drawBackground();

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,
                mat_amb_diff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,
                mat_dif_diff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,
                mat_spe_diff);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor4ub(250, 250, 250, 255);

    glTranslatef(tX, tY, 0);
    glPushMatrix();
    if (mode == MODE_NORMAL) {
        robot->openGLDraw(false);
    } else {
        client.lock();
        if (client.robot) {
            client.robot->openGLDraw(true);

            // Mirror
            glScalef(1.0, 1.0, -1.0);
            if (sin(beta) > 0) {
                client.robot->openGLDraw(true, 0.25);
            }
        }
        client.unlock();
    }
    glPopMatrix();
    glStencilFunc(GL_ALWAYS, 255, -1);

    glDisable(GL_LIGHTING);

    // Drawing Bullet debug, can be enabled for debug purpose
    glPushMatrix();
    glScalef(1000, 1000, 1000);
    /*
    for (int k=0; k<12; k++)
        robot->world.stepSimulation(0.001);
    */
    // robot->world.debugDraw();
    glPopMatrix();

    if (drawGrid) {
        drawGridLines();
    }

    if (drawXYZ) {
        drawAxis();
    }

    // Drawing ground collisions
    client.lock();
    glLineWidth(3.0);
    glColor3f(1.0, 0.6, 0.0);
    glScalef(1000, 1000, 1000);
    if (mode == MODE_PHYSICS && client.robot) {
        for (auto point : client.robot->collisionPoints) {
            auto A = point.first;
            auto B = point.second;
            B = B.multiply(75);
            glBegin(GL_LINES);
            glVertex3f(A.x(), A.y(), A.z());
            glVertex3f(A.x()+B.x(), A.y()+B.y(), A.z()+B.z());
            glEnd();
        }
    }
    client.unlock();
}

void Viewer::keyPressEvent(QKeyEvent *keyEvent)
{
    if (keyEvent->key() == Qt::Key_Left) {
        setAutorotate(false);
        alpha -= 0.1;
    }
    if (keyEvent->key() == Qt::Key_Right) {
        setAutorotate(false);
        alpha += 0.1;
    }
    if (keyEvent->key() == Qt::Key_Up) {
        setAutorotate(false);
        beta += 0.1;
    }
    if (keyEvent->key() == Qt::Key_Down) {
        setAutorotate(false);
        beta -= 0.1;
    }
    if (keyEvent->key() == Qt::Key_PageUp) {
        radius -= 15.0;
        if (radius < 0.1) radius = 0.1;
    }
    if (keyEvent->key() == Qt::Key_PageDown) {
        radius += 15.0;
    }

    if (beta > M_PI/2-0.01) beta = M_PI/2-0.01;
    if (beta < -M_PI/2+0.01) beta = -M_PI/2+0.01;
    shouldRedraw = true;
}

Component *Viewer::getInstanceAt(int x, int y, int *id)
{
        /*
    GLbyte color[4];
    GLfloat depth;
    GLuint index;

    glReadPixels(x, window_height - y - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
    glReadPixels(x, window_height - y - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    glReadPixels(x, window_height - y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

     printf("Clicked on pixel %d, %d, color %02hhx%02hhx%02hhx%02hhx, depth %f, stencil index %u\n",
      x, y, color[0], color[1], color[2], color[3], depth, index);
     fflush(stdout);
     */

    int window_height = height();
    GLint index;
    glReadPixels(x, window_height - y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);
    if (id != NULL) {
        *id = index;
    }

    return robot->getComponentById(index);
}

void Viewer::drawBackground()
{
    glMatrixMode (GL_MODELVIEW);
    glPushMatrix ();
    glLoadIdentity ();
    glMatrixMode (GL_PROJECTION);
    glPushMatrix ();
    glLoadIdentity ();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glBegin (GL_QUADS);
    glColor3f(0.9,0.9,1.0);
    glVertex3f (-1.0f, -1.0f, -1.0f);
    glVertex3f (1.0f, -1.0f, -1.0f);

    glColor3f(0.8,0.8,1.0);
    glVertex3f (1.0f, 1.0f, -1.0f);
    glVertex3f (-1.0f, 1.0f, -1.0f);
    glEnd();

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void Viewer::drawGridLines()
{
    glBegin(GL_TRIANGLES);

    float size = 15;
    float pX1, pY1, pX2, pY2, pZ;

    if (mode == MODE_NORMAL) {
        pX1 = plateX1;
        pY1 = plateY1;
        pX2 = plateX2;
        pY2 = plateY2;
        pZ = plateZ;
    } else {
        pX1 = -5000;
        pX2 = 5000;
        pY1 = -5000;
        pY2 = 5000;
        pZ = 0;
        size = 100;
    }

    int xe=0;
    for (float x=pX1; x<=pX2; x+=size) {
        xe = (xe+1)%2;
        int ye=0;
        for (float y=pY1; y<=pY2; y+=size) {
            ye = (ye+1)%2;

            if ((xe%2) == (ye%2)) {
                glColor4f(0.8, 0.8, 0.8, 0.5);
            } else {
                glColor4f(1.0, 1.0, 1.0, 0.5);
            }

            glVertex3f(x, y, pZ);
            glVertex3f(x+size, y, pZ);
            glVertex3f(x+size, y+size, pZ);

            glVertex3f(x, y, pZ);
            glVertex3f(x+size, y+size, pZ);
            glVertex3f(x, y+size, pZ);

            glVertex3f(x+size, y+size, pZ);
            glVertex3f(x+size, y, pZ);
            glVertex3f(x, y, pZ);

            glVertex3f(x, y+size, pZ);
            glVertex3f(x+size, y+size, pZ);
            glVertex3f(x, y, pZ);
        }
    }
    glEnd();
}

void Viewer::drawAxis()
{
    glLineWidth(1.0);
    glBegin(GL_LINES);
    glColor3f(0.9, 0.0, 0.0);
    glVertex3f(0, 0, 0);
    glVertex3f(100.0, 0, 0);
    glColor3f(0.0, 0.9, 0.0);
    glVertex3f(0, 0, 0);
    glVertex3f(0.0, 100, 0);
    glColor3f(0.0, 0.0, 0.9);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 100);
    glEnd();
}
void Viewer::mousePressEvent(QMouseEvent *evt)
{
    moved = false;
    if (evt->button() == Qt::LeftButton) {
        pressed = true;
        mX = evt->x();
        mY = evt->y();
        mAlpha = alpha;
        mBeta = beta;
    }
    if (evt->button() == Qt::RightButton) {
        movePressed = true;
        mX = evt->x();
        mY = evt->y();
        mTX = tX;
        mTY = tY;
    }
}

void Viewer::mouseReleaseEvent(QMouseEvent *evt)
{
    if (evt->button() == Qt::LeftButton) {
        pressed = false;
    }
    if (evt->button() == Qt::RightButton) {
        movePressed = false;
    }

    if (!moved) {
        auto instance = getInstanceAt(evt->x(), evt->y());

        if (instance) {
            emit component_clicked(instance);
        } else {
            emit nowhere_clicked();
        }

        if (evt->button() == Qt::RightButton) {
            emit viewer_contextmenu_request(QPoint(evt->x(), evt->y()));
        }
    }
    shouldRedraw = true;
}

void Viewer::mouseMoveEvent(QMouseEvent *evt)
{
    moved = true;

    if (pressed) {
        setAutorotate(false);
        float dX = evt->x()-mX;
        float dY = evt->y()-mY;
        alpha = mAlpha - 0.01*dX;
        beta = mBeta + 0.01*dY;

        if (beta > M_PI/2-0.01) beta = M_PI/2-0.01;
        if (beta < -M_PI/2+0.01) beta = -M_PI/2+0.01;
    } else if (movePressed) {
        setAutorotate(false);
        float dX = evt->x()-mX;
        float dY = evt->y()-mY;

        tX = mTX + dY*cos(alpha) + dX*cos(alpha+M_PI/2.0);
        tY = mTY + dY*sin(alpha) + dX*sin(alpha+M_PI/2.0);
    } else {
       int id;
       auto instance = getInstanceAt(evt->x(), evt->y(), &id);
       if (id < 200 && id!=255) {
           robot->unHover();
           if (instance) {
             instance->onHover();
           }
       }
    }
    shouldRedraw = true;
}

void Viewer::mouseDoubleClickEvent(QMouseEvent *evt)
{
    int id;
    auto instance = getInstanceAt(evt->x(), evt->y(), &id);

    if (instance) {
        emit component_double_clicked(instance);
    } else {
        auto anchor = robot->getHoveredAnchor(id);
        if (anchor) {
            emit anchor_clicked(anchor);
        }
    }
}

void Viewer::wheelEvent(QWheelEvent *evt)
{
    radius -= evt->delta()*0.1;

    if (radius < 0.1) radius = 0.1;
    shouldRedraw = true;
}

void Viewer::setPlateDimension(float x1, float y1, float x2, float y2, float z)
{
    plateX1 = x1;
    plateY1 = y1;
    plateX2 = x2;
    plateY2 = y2;
    plateZ = z;
}

void Viewer::setAutorotate(bool value)
{
    autorotate = value;
    autorotate_changed(value);
}

void Viewer::dontMove()
{
    movePressed = false;
}

void Viewer::timeOutSlot()
{
    if (mode == MODE_PHYSICS || shouldRedraw || autorotate) {
        shouldRedraw = false;
        updateGL();
    }
}
