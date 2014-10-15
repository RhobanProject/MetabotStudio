// Component Square Body
// Description Square body
include <../../metabot.scad>;
include <../models/motor_values.scad>;
include <../models/ollo.scad>;
use <../models/motor_arm.scad>;
use <../parts/square_body.scad>;

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

translate([0,0,MotorDepth/2])
square_body(width=Width, length=Length, legsSpacing=LegsSpacing, legs=Legs, thickness=Thickness);

translate([0,0,-Thickness-MotorDepth/2])
square_body(width=Width, length=Length, legsSpacing=LegsSpacing, legs=Legs, thickness=Thickness);

LegsDistance = LegsSpacing*(Legs/2-1);
for (leg=[1:(Legs/2)]) {
    for (side=[-1,1]) {
	translate([-side*(Width/2),LegsDistance/2-(leg-1)*LegsSpacing,0])
	rotate([0,0,side*90])
	translate([0,OlloSpacing*4,0]) {
		motorArm();
		metabot_anchor("motor", male=true);
	}
    }
}