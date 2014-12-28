NoModels = false;
include <../models/ollo.metabot.scad>;
include <../models/motor_values.metabot.scad>;
use <../models/motor_arm.metabot.scad>;
use <../models/arm.metabot.scad>;
use <../parts/tie_box.metabot.scad>;
use <../parts/tie_bar.metabot.scad>;

module block(Thickness=2.2, Internal=false)
{
// metabot: Begining module block
motorArm();
rotate([180,0,0]) {
//:Anchor motor male
}

rotate([0, -90, 0]) {
//:Anchor motor male
}

translate([OlloWidth+MotorWidth/2,0,0]) {
rotate([0, -90, 0]) {
pulley();
}
}

translate([-MotorWidth-2*OlloWidth-Thickness,0,0]) {
rotate([0, 90, 0]) {
motorArm();
}
}

if (Internal) {
translate([-MotorWidth/2- (2*OlloWidth+Thickness)/2, -3.5*OlloSpacing, 0]) {
tie_box(Thickness);
}
} else {
translate([-(2*OlloWidth+Thickness+OlloSpacing*2+MotorWidth/2),-OlloSpacing*3,MotorDepth/2])
tie_bar();
translate([-(2*OlloWidth+Thickness+OlloSpacing*2+MotorWidth/2),-OlloSpacing*3,-Thickness-MotorDepth/2])
tie_bar();
}
}
// metabot: Ending module block
