NoModels = false;
include <../models/motor_values.metabot.scad>;
include <../models/ollo.metabot.scad>;
use <../models/motor_arm.metabot.scad>;
use <../parts/body.metabot.scad>;

module body(Size=20, Legs=4, Thickness=2.2)
{
// metabot: Begining module body
translate([0,0,-(MotorWidth+Thickness)/2-Thickness/2])
body(size=Size, legs=Legs, thickness=Thickness);
translate([0,0,(MotorWidth+Thickness)/2-Thickness/2])
body(size=Size, legs=Legs, thickness=Thickness);

if (Legs>0) {
for (i=[1:Legs]) {
rotate([0, 0, 360*i/Legs])
translate([0,Size+MotorHeight-MotorArmOffset-OlloSpacing-2.3,0]) {
motorArm();
//:Anchor motor male
}
}
}
}
// metabot: Ending module body
