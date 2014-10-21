// Component Arm leg
// Description Leg on arm
use <../../metabot.scad>;
use <../parts/leg.scad>;
use <../models/rivets.scad>

// Parameter Size of the first part
SizeA = 60;
// Parameter Size of the second part
SizeB = 20;
// Parameter Size of the third part
SizeC = 20;
// Parmaeter Size of the bottom
SizeBottom = 10;
// Parameter Size of the top
SizeTop = 15;
// Parameter Thickness of the part
Thickness = 2.2;

metabot_anchor("motor", female=true);
rivets(9);

rotate([90,90,0]) {
    leg(SizeA, SizeB, SizeC, SizeBottom, SizeTop, "arm", 0, Thickness);
}
