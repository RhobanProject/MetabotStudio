#ifndef VIEWER_H
#define VIEWER_H

#include <QTimer>
#include <QGLWidget>
#include <3d/Model.h>
#include <metabot/Robot.h>
#include <metabot/ComponentInstance.h>
#include <metabot/TransformMatrix.h>

class Viewer : public QGLWidget
{
    Q_OBJECT
public:
    explicit Viewer(int framesPerSecond = 10, QWidget *parent = 0, char *name = 0);
    void setRobot(Metabot::Robot *robot);
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void keyPressEvent( QKeyEvent *keyEvent );

    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);
    void updateRatio();

    void setPlateDimension(float width, float height);

    Metabot::Robot *robot;
    Metabot::TransformMatrix matrix;
    float t;
    float plateWidth, plateHeight;

public slots:
    virtual void timeOutSlot();

private:
    bool autorotate;
    float radius;
    float alpha, beta;
    float mAlpha, mBeta;
    float mX, mY;
    bool pressed;
    QTimer *t_Timer;
};

#endif // VIEWER_H
