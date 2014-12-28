NoModels = false;
include <../models/motor_values.metabot.scad>;
include <../models/ollo.metabot.scad>;
use <../models/motor_arm.metabot.scad>;
use <../parts/u.metabot.scad>;

$fn=18;

module arm_u(Height=10, Radius=8, Thickness=2.2, Angle=0)
{
// metabot: Begining module arm_u
//:Anchor motor female

u(Height, Radius, Thickness, screws=false, olloArm=true, armAngle=Angle);

translate([0,Height+Radius+OlloWidth+MotorDepth/2,0]) {
rotate([90,Angle,0]) {
//:Anchor motor female
}
}
}
// metabot: Ending module arm_u
