#include <iostream>
#ifdef OPENGL
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#endif
#include "GLDebugDrawer.h"

#define EQ(A,B) (fabs(A-B)<0.01)

#include <stdio.h> //printf debugging
    GLDebugDrawer::GLDebugDrawer()
:m_debugMode(1)
{
}

GLDebugDrawer::~GLDebugDrawer()
{
}

void	GLDebugDrawer::drawLine(const btVector3& from,const btVector3& to,const btVector3& fromColor, const btVector3& toColor)
{
#ifdef OPENGL
    float R = fromColor.getX();
    float G = fromColor.getY();
    float B = fromColor.getZ();
    if (EQ(R,0.7) || EQ(G,0.7) || EQ(B,0.7)) return;
    glBegin(GL_LINES);
    glColor3f(fromColor.getX(), fromColor.getY(), fromColor.getZ());
    glVertex3d(from.getX(), from.getY(), from.getZ());
    glColor3f(toColor.getX(), toColor.getY(), toColor.getZ());
    glVertex3d(to.getX(), to.getY(), to.getZ());
    glEnd();
#endif
}

void GLDebugDrawer::drawLine(const btVector3& from,const btVector3& to,const btVector3& color)
{
    drawLine(from,to,color,color);
}

void GLDebugDrawer::drawSphere (const btVector3& p, btScalar radius, const btVector3& color)
{
#ifdef OPENGL
    glColor4f (color.getX(), color.getY(), color.getZ(), btScalar(1.0f));
    glPushMatrix ();
    glTranslatef (p.getX(), p.getY(), p.getZ());

    int lats = 5;
    int longs = 5;

    int i, j;
    for(i = 0; i <= lats; i++) {
        btScalar lat0 = SIMD_PI * (-btScalar(0.5) + (btScalar) (i - 1) / lats);
        btScalar z0  = radius*sin(lat0);
        btScalar zr0 =  radius*cos(lat0);

        btScalar lat1 = SIMD_PI * (-btScalar(0.5) + (btScalar) i / lats);
        btScalar z1 = radius*sin(lat1);
        btScalar zr1 = radius*cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for(j = 0; j <= longs; j++) {
            btScalar lng = 2 * SIMD_PI * (btScalar) (j - 1) / longs;
            btScalar x = cos(lng);
            btScalar y = sin(lng);

            glNormal3f(x * zr0, y * zr0, z0);
            glVertex3f(x * zr0, y * zr0, z0);
            glNormal3f(x * zr1, y * zr1, z1);
            glVertex3f(x * zr1, y * zr1, z1);
        }
        glEnd();
    }

    glPopMatrix();
#endif
}

void	GLDebugDrawer::drawTriangle(const btVector3& a,const btVector3& b,const btVector3& c,const btVector3& color,btScalar alpha)
{
#ifdef OPENGL
    //	if (m_debugMode > 0)
    {
        const btVector3	n=btCross(b-a,c-a).normalized();
        glBegin(GL_TRIANGLES);		
        glColor4f(color.getX(), color.getY(), color.getZ(),alpha);
        glNormal3d(n.getX(),n.getY(),n.getZ());
        glVertex3d(a.getX(),a.getY(),a.getZ());
        glVertex3d(b.getX(),b.getY(),b.getZ());
        glVertex3d(c.getX(),c.getY(),c.getZ());
        glEnd();
    }
#endif
}

void	GLDebugDrawer::setDebugMode(int debugMode)
{
    m_debugMode = debugMode;
}

void	GLDebugDrawer::draw3dText(const btVector3& location,const char* textString)
{
#ifdef OPENGL
    glRasterPos3f(location.x(),  location.y(),  location.z());
    //BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),textString);
#endif
}

void	GLDebugDrawer::reportErrorWarning(const char* warningString)
{
    printf("%s\n",warningString);
}

void	GLDebugDrawer::drawContactPoint(const btVector3& pointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color)
{
#ifdef OPENGL
    {
        btVector3 to=pointOnB+normalOnB*1;//distance;
        const btVector3&from = pointOnB;
        glColor4f(color.getX(), color.getY(), color.getZ(),1.f);
        //glColor4f(0,0,0,1.f);
        glBegin(GL_LINES);
        glVertex3d(from.getX(), from.getY(), from.getZ());
        glVertex3d(to.getX(), to.getY(), to.getZ());
        glEnd();


        //		glRasterPos3f(from.x(),  from.y(),  from.z());
        //		char buf[12];
        //		sprintf(buf," %d",lifeTime);
        //BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),buf);


    }
#endif
}
