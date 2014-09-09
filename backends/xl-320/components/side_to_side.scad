// Component motor
include <../../metabot.scad>;
include <../models/motor.scad>;
use <../models/motor_arm.scad>;
use <../parts/side.scad>;

// Parameter Size
Size = 40;
// Parameter Holes to border distance
HolesToBorder = 5;
// Thickness
Thickness = 2.2;

motorArm();

for (side=[MotorWidth/2+Thickness,-MotorWidth/2]) {
    translate([side,0,0]) {
        rotate([180,90,0]) {
            color(col)
                side(Size, HolesToBorder, Thickness);
        }
    }
}
translate([0,-(Size*2-2*HolesToBorder),0]) {
    rotate([0,0,180]) {
        motorArm();
        metabot_anchor("motor");
    }
}
