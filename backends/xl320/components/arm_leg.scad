// Component motor
use <../parts/leg.scad>;

// Parameter
SizeA = 60;
// Parameter
SizeB = 20;
// Parameter
SizeC = 20;
// Parmaeter
SizeBottom = 10;
// Parameter
SizeTop = 15;
// Parameter
Thickness = 2.2;

rotate([90,90,0]) {
    leg(SizeA, SizeB, SizeC, SizeBottom, SizeTop, "arm", 0, Thickness);
}
