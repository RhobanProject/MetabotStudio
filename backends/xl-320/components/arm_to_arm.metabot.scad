NoModels = false;
use <../parts/arm_to_arm.metabot.scad>;
include <../models/ollo.metabot.scad>;
include <../models/motor_values.metabot.scad>
use <../models/motor_arm.metabot.scad>

module arm_to_arm(Distance=50, Thickness=2.2)
{
// metabot: Begining module arm_to_arm
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
// metabot: Ending module arm_to_arm
