use <../parts/arm_to_arm.scad>;
include <../models/ollo.scad>;
include <../models/motor_values.scad>
use <../models/motor_arm.scad>

//:Component "Arm to arm"
//:Description "Links two servos by arms"
//:Parameter Distance "Distance between arms"
//:Parameter Thickness "Part thickness"
module arm_to_arm(Distance=50, Thickness=2.2)
{
    translate([0,-Distance/2,0]) {
        //:Anchor motor female

        translate([0,0,OlloWidth+MotorDepth/2]) {
            arm_to_arm(distance=Distance,thickness=Thickness);
        }
        translate([0,0,-Thickness-OlloWidth-MotorDepth/2]) {
            arm_to_arm(distance=Distance,thickness=Thickness);
        }

        translate([0,Distance,0]) {
            rotate([0,0,180]) {
                //:Anchor motor female
            }
        }
    }
}
