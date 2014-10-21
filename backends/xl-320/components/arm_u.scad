// Component Arm U
// Description An U on the arm
use <../../metabot.scad>;
use <../models/rivets.scad>
include <../models/motor_values.scad>;
include <../models/ollo.scad>;
use <../models/motor_arm.scad>;
use <../parts/u.scad>;

$fn=18;

// Parameter Height of the Us
Height = 10;
// Parameter Radius of the fixation
Radius = 8;
// Parameter Thickness
Thickness = 2.2;
// Parameter Static angle
Angle = 0;

rivets(13);

metabot_anchor("motor", female=true);
// motorArm();

u(Height, Radius, Thickness, screws=false, olloArm=true, armAngle=Angle);

translate([0,Height+Radius+OlloWidth+MotorDepth/2,0]) {
    rotate([90,Angle,0]) {
	    metabot_anchor("motor", female=true);
	    // motorArm();
  }
}