// Component Body
// Description Star body
include <../../metabot.scad>;
include <../models/motor_values.scad>;
include <../models/ollo.scad>;
use <../models/motor_arm.scad>;
use <../parts/body.scad>;

// Parameter Size of the body
Size = 20;
// Parameter Number of legs
Legs = 4;
// Parameter Thickness
Thickness = 2.2;

translate([0,0,-(MotorWidth+Thickness)/2-Thickness/2])
	body(size=Size, legs=Legs, thickness=Thickness);
translate([0,0,(MotorWidth+Thickness)/2-Thickness/2])
	body(size=Size, legs=Legs, thickness=Thickness);

if (Legs>0) {
	for (i=[1:Legs]) {
		rotate([0, 0, 360*i/Legs])
		translate([0,Size+MotorHeight-MotorArmOffset-OlloSpacing-2.3,0]) {
			motorArm();
		metabot_anchor("motor", male=true);
	    }
	}
}
