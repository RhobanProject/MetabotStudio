// Component motor
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
// Parameter Fixation angle
FixationAngle = 0;
// Parameter Thickness of the part
Thickness = 2.2;

translate([0,-(24),0]) {
    rotate([180,0,0]) {
        rotate([180,90,FixationAngle]) {
            leg(SizeA, SizeB, SizeC, SizeBottom, SizeTop, "side", FixationAngle, Thickness);
        }
    }
}
