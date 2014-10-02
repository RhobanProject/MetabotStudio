// Component Block
// Description Two DOF block
use <../../metabot.scad>;
include <../models/ollo.scad>;
include <../models/motor_values.scad>;
use <../models/motor_arm.scad>;
use <../models/arm.scad>;
use <../parts/tie_box.scad>;

// Parameter Thickness
Thickness = 2.2;

motorArm();
rotate([180,0,0]) {
// # motorArm();
metabot_anchor("motor", male=true);
}

rotate([0, -90, 0]) {
  // # motorArm();
  metabot_anchor("motor", male=true);
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

translate([-MotorWidth/2- (2*OlloWidth+Thickness)/2, -3.5*OlloSpacing, 0]) {
  tie_box(Thickness);
}