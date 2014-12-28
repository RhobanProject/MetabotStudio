NoModels = false;
use <../models/motor_arm.metabot.scad>;
use <../parts/arm_side.metabot.scad>;

module arm_to_side(FirstPart=17, Distance=45, Thickness=2.2)
{
// metabot: Begining module arm_to_side
//:Anchor motor female

arm_side(firstPart=FirstPart , distance=Distance, thickness=Thickness);
rotate([0,180,0]) {
arm_side(firstPart=FirstPart , distance=Distance, thickness=Thickness);
}

translate([0,Distance,0]) {
rotate([0,90,0]) {
motorArm();
//:Anchor motor male
}
}
}
// metabot: Ending module arm_to_side
