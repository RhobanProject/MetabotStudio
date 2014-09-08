// Component root
include <../models/motor.scad>;
include <../models/ollo.scad>;
use <../models/motor_arm.scad>;
use <../parts/body.scad>;

// Parameter Size of the body
Size = 40;
// Parameter Number of legs
Legs = 4;
// Parameter Thickness
Thickness = 2.2;

translate([0,0,-(MotorWidth+Thickness)/2-Thickness/2])
	body(size=Size, legs=Legs, width=Thickness);
translate([0,0,(MotorWidth+Thickness)/2-Thickness/2])
	body(size=Size, legs=Legs, width=Thickness);

for (i=[1:Legs]) {
	rotate([0, 0, 360*i/Legs])
	translate([0,Size+MotorHeight-MotorArmOffset-OlloSpacing-Thickness,0])
		motorArm();
}
