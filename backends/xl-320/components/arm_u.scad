include <../models/motor_values.scad>;
include <../models/ollo.scad>;
use <../models/motor_arm.scad>;
use <../parts/u.scad>;

$fn=18;

//:Component "Arm U"
//:Description "An U on the arm"
//:Parameter Height "Heights of the Us"
//:Parameter Radius "Radius of fixation"
//:Parameter Thickness "Thickness"
//:Parameter Angle "Static angle"
module arm_u(Height=10, Radius=8, Thickness=2.2, Angle=0)
{
    //:Anchor motor female

    u(Height, Radius, Thickness, screws=false, olloArm=true, armAngle=Angle);

    translate([0,Height+Radius+OlloWidth+MotorDepth/2,0]) {
        rotate([90,Angle,0]) {
            //:Anchor motor female
        }
    }
}