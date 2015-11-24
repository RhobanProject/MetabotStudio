#include <stdio.h>
#include <math.h>
#include <Component.h>
#include "kinematic.h"
#include "Controller.h"

Controller::Controller()
{
    gait = GAIT_TROT;
    alt = 15;
    freq = 1.0;
    h = -55;
    r = 125;
    dx = 0;
    dy = 0;
    turn = 0;
    setupFunctions();
}

void Controller::setupFunctions()
{
    rise.clear();
    step.clear();

    if (gait == GAIT_WALK) {
        // Rising the legs
        rise.addPoint(0.0, 0.0);
        rise.addPoint(0.1, 1.0);
        rise.addPoint(0.3, 1.0);
        rise.addPoint(0.35, 0.0);
        rise.addPoint(1.0, 0.0);

        // Taking the leg forward
        step.addPoint(0.0, -0.5);
        step.addPoint(0.12, -0.5);
        step.addPoint(0.3, 0.5);
        step.addPoint(0.35, 0.5);
        step.addPoint(1.0, -0.5);
    }

    if (gait == GAIT_TROT) {
        // Rising the legs
        rise.addPoint(0.0, 1.0);
        rise.addPoint(0.3, 1.0);
        rise.addPoint(0.4, 0.0);
        rise.addPoint(0.9, 0.0);
        rise.addPoint(1.0, 1.0);

        // Taking the leg forward
        step.addPoint(0.0, -0.5);
        step.addPoint(0.1, -0.5);
        step.addPoint(0.3, 0.5);
        step.addPoint(0.5, 0.5);
        step.addPoint(0.85, -0.5);
        step.addPoint(1.0, -0.5);

        /*
         // Rising the legs
         rise.addPoint(0.0, 0.0);
         rise.addPoint(0.1, 1.0);
         rise.addPoint(0.4, 1.0);
         rise.addPoint(0.5, 0.0);
         rise.addPoint(1.0, 0.0);
 
         // Taking the leg forward
         step.addPoint(0.0, -0.5);
         step.addPoint(0.1, -0.5);
         step.addPoint(0.5, 0.5);
         step.addPoint(1.0, -0.5);
         */
    }
}

Controller::Angles Controller::compute(float t_)
{
    float t = freq*t_;
    Angles angles;
    float turnRad = DEG2RAD(turn);

    for (int i=0; i<4; i++) {
        // Defining in which group of diagonaly opposite legs this leg is
        bool group = ((i&1)==1);

        // This defines the phase of the gait
        float legPhase;

        if (gait == GAIT_WALK) {
            float phases[] = {0.0, 0.5, 0.75, 0.25};
            legPhase = t + phases[i];
        }
        if (gait == GAIT_TROT) {
            legPhase = t + group*0.5;
        }

        float x, y, z, a, b, c;

        // Computing the order in the referencial of the body
        float stepping = step.getMod(legPhase);

        // Set X and Y to the moving vector
        float X = stepping*dx;
        float Y = stepping*dy;

        // Add the radius to the leg, in the right direction

        switch (i) {
            case 0:
                X += cos(M_PI/4)*r;
                Y += cos(M_PI/4)*r;
                break;
            case 1:
                X += cos(M_PI/4)*r;
                Y -= cos(M_PI/4)*r;
                break;
            case 2:
                X -= cos(M_PI/4)*r;
                Y -= cos(M_PI/4)*r;
                break;
            case 3:
                X -= cos(M_PI/4)*r;
                Y += cos(M_PI/4)*r;
                break;
        }

        // Rotate around the center of the robot
        float xOrder = cos(stepping*turnRad)*X - sin(stepping*turnRad)*Y;
        float yOrder = sin(stepping*turnRad)*X + cos(stepping*turnRad)*Y;

        // Move to the leg frame
        float vx, vy;
        legFrame(xOrder, yOrder, &vx, &vy, i, L0);

        // Avoid moving legs when dynamic parameters are low
        float enableRise = (fabs(dx)>0.5 || fabs(dy)>0.5 || fabs(turn)>5) ? 1 : 0;

        // This is the x,y,z order in the referencial of the leg
        x = vx;
        y = vy;
        z = h + rise.getMod(legPhase)*alt*enableRise;
        // if (i < 2) z += frontH;

        // Computing inverse kinematics
        if (computeIK(x, y, z, &a, &b, &c, L1, L2, L3)) {
            angles.l1[i] = a;
            angles.l2[i] = b;
            angles.l3[i] = c;
        }
    }

    return angles;
}
        
void Controller::update(float t, Metabot::Robot &robot)
{
    auto angles = compute(t);
    for (int k=0; k<4; k++) {
        int leg = (k+2)%4;
        robot.getComponentById(k*3+2)->setTarget(angles.l1[leg]);
        robot.getComponentById(k*3+3)->setTarget(-angles.l2[leg]);
        robot.getComponentById(k*3+4)->setTarget(angles.l3[leg]);
    }
}
