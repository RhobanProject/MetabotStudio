// Component Arm leg
// Description Leg on arm
use <../../metabot.scad>;
use <../parts/leg.scad>;

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

translate([40,0,0]) {
metabot_anchor("motor", female=true);

rotate([90,90,0]) {
    leg(SizeA, SizeB, SizeC, SizeBottom, SizeTop, "arm", 0, Thickness);
}
}