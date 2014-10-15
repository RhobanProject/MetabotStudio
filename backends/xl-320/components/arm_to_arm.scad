// Component Arm to arm
// Description Can links two servos
use <../../metabot.scad>;
use <../parts/arm_to_arm.scad>;
include <../models/ollo.scad>;
include <../models/motor_values.scad>
use <../models/motor_arm.scad>

// Parameter Distance
Distance = 50;
// Parameter Thickness
Thickness = 2.2;

translate([0,-Distance/2,0]) {
	// motorArm();
	metabot_anchor("motor", female=true);

	translate([0,0,OlloWidth+MotorDepth/2]) {
	    arm_to_arm(distance=Distance,thickness=Thickness);
	}
	translate([0,0,-Thickness-OlloWidth-MotorDepth/2]) {
	    arm_to_arm(distance=Distance,thickness=Thickness);
	}

	translate([0,Distance,0]) {
		rotate([0,0,180]) {
			// motorArm();
			metabot_anchor("motor", female=true);
		}
	}
}