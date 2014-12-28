NoModels = false;
include <../models/motor_values.metabot.scad>;
use <../models/motor_arm.metabot.scad>;
use <../parts/side.metabot.scad>;

module side_to_side(Size=65, HolesToBorder=5, Thickness=2.2)
{
// metabot: Begining module side_to_side
translate([0,Size/2,0]) {
//:Anchor motor male
motorArm();

for (side=[MotorWidth/2+Thickness,-MotorWidth/2]) {
translate([side,0,0]) {
rotate([180,90,0]) {
color(col)
side(Size, HolesToBorder, Thickness);
}
}
}
translate([0,-Size,0]) {
rotate([0,0,180]) {
motorArm();
//:Anchor motor male
}
}
}
}
// metabot: Ending module side_to_side
