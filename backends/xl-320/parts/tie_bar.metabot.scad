NoModels = false;
include <../models/ollo.metabot.scad>;
include <../models/motor_values.metabot.scad>;
use <../models/motor_arm.metabot.scad>;

$fn=40;

module tie_bar(thickness=2.2)
{
// metabot: Begining module tie_bar
marker("metabot_part: tie_bar");
marker("metabot_parameter: thickness ", thickness);
if (NoModels == false) {
_tie_bar(thickness=thickness);
}
}
module _tie_bar(thickness=2.2) {
W = MotorWidth+OlloWidth+thickness+OlloWidth+MotorDepth;
H = OlloSpacing+2*OlloWidth;

translate([-MotorDepth/2,OlloWidth,0])
rotate([90,0,0])
difference() {
cube([W,thickness,H]);

translate([MotorDepth/2,0,H/2-OlloSpacing/2])
rotate([-90,0,0])
threeOllo();

translate([W-MotorWidth/2,0,H/2+OlloSpacing/2])
rotate([-90,0,0])
threeOllo();
}
}
// metabot: Ending module tie_bar

tie_bar();
