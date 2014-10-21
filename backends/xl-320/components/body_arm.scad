// Component Body arms
// Description Star body arms
include <../../metabot.scad>;
use <../models/rivets.scad>
include <../models/motor_values.scad>;
include <../models/ollo.scad>;
use <../models/motor_arm.scad>;
use <../parts/body_arm.scad>;

// Parameter Size of the body
Size = 25;
// Parameter Number of legs
Legs = 4;
// Parameter Thickness
Thickness = 2.2;

translate([0,0,-OlloWidth-(MotorWidth+Thickness)/2-Thickness/2])
	body_arm(size=Size, legs=Legs, thickness=Thickness);
translate([0,0,OlloWidth+(MotorWidth+Thickness)/2-Thickness/2])
	body_arm(size=Size, legs=Legs, thickness=Thickness);

if (Legs>0) {
	for (i=[1:Legs]) {
		rotate([0, 0, 360*i/Legs])
		translate([0,Size,0]) {
			rotate([0,0,180]) {
				rivets(9);
				// motorArm();
				metabot_anchor("motor", female=true);
			}
	    }
	}
}
