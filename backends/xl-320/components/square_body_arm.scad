// Component Square Body arm
// Description Square body arm
use <../models/rivets.scad>
include <../../metabot.scad>;
include <../models/motor_values.scad>;
include <../models/ollo.scad>;
use <../models/motor_arm.scad>;
use <../parts/square_body_arm.scad>;

// Parameter Width of the body
Width = 40;
// Parameter Length of the body
Length = 70;
// Parameter Spacing between legs
LegsSpacing = 40;
// Parameter Number of legs (multiple of 2)
Legs = 4;
// Parameter Thickness
Thickness = 2.2;

translate([0,0,MotorDepth/2+OlloWidth])
square_body_arm(width=Width, length=Length, legsSpacing=LegsSpacing, legs=Legs, thickness=Thickness);

translate([0,0,-Thickness-MotorDepth/2])
square_body_arm(width=Width, length=Length, legsSpacing=LegsSpacing, legs=Legs, thickness=Thickness);

LegsDistance = LegsSpacing*(Legs/2-1);
for (leg=[1:(Legs/2)]) {
    for (side=[-1,1]) {
	translate([-side*(Width/2),LegsDistance/2-(leg-1)*LegsSpacing,0])
	rotate([0,0,90+(side==1?180:0)]) {
		// motorArm();
		rivets(9);
		metabot_anchor("motor", female=true);
	}
    }
}