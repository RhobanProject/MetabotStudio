#ifndef VIEWER_H
#define VIEWER_H

#include <QTimer>
#include <QGLWidget>
#include <3d/Model.h>
#include <metabot/Robot.h>
#include <metabot/Component.h>
#include <metabot/TransformMatrix.h>
#include <metabot/com/Client.h>

#define MODE_NORMAL     0
#define MODE_PHYSICS    1

class Viewer : public QGLWidget
{
    Q_OBJECT
public:
    explicit Viewer(int framesPerSecond = 10, QWidget *parent = 0, char *name = 0);
    void setRobot(Metabot::Robot *robot);
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void keyPressEvent(QKeyEvent *keyEvent);

    Metabot::Component *getInstanceAt(int x, int y, int *id=NULL);

    void drawBackground();
    void drawGridLines();
    void drawAxis();

    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);
    void updateRatio();
    void updatePlate();
    void redraw();

    void setPlateDimension(float x1, float y1, float x2, float y2, float z);

    Metabot::Robot *robot;
    Metabot::TransformMatrix matrix;
    float t;
    float plateX1, plateY1;
    float plateX2, plateY2;
    float plateZ;
    bool drawGrid, drawXYZ;

    void setAutorotate(bool value);
    void dontMove();

    // Translation
    float tX, tY, tZ;
    float mTX, mTY;

    // Mode (normal? physics?)
    int mode;

public slots:
    virtual void timeOutSlot();

signals:
    void viewer_contextmenu_request(QPoint);
    void autorotate_changed(bool value);
    void nowhere_clicked();
    void anchor_clicked(Metabot::AnchorPoint *anchor);
    void component_clicked(Metabot::Component *instance);
    void component_double_clicked(Metabot::Component *instance);

private:
    bool shouldRedraw;
    bool autorotate;
    float radius;

    // Rotation
    float alpha, beta;
    float mAlpha, mBeta;

    // Mouse events
    float mX, mY;
    bool pressed, movePressed, moved;

    Metabot::Client client;
    QTimer *t_Timer;
};

#endif // VIEWER_H
