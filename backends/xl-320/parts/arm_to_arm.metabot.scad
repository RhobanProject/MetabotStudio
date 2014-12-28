NoModels = false;
include <../models/ollo.metabot.scad>;
use <../util/rounded.metabot.scad>;

module arm_to_arm(distance=50, thickness=2.2)
{
// metabot: Begining module arm_to_arm
marker("metabot_part: arm_to_arm");
marker("metabot_parameter: distance ", distance);
marker("metabot_parameter: thickness ", thickness);
if (NoModels == false) {
_arm_to_arm(distance=distance,thickness=thickness);
}
}
module _arm_to_arm(distance=50, thickness=2.2) {
translate([0,distance/2,0])
difference() {
rounded(OlloSpacing*4, distance+OlloSpacing*3, thickness, r=10, center=true);
translate([0,distance/2,0])
servoArm();
translate([0,-distance/2,0])
servoArm();
}
}
// metabot: Ending module arm_to_arm

arm_to_arm();
