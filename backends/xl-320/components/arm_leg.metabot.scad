NoModels = false;
use <../../metabot.metabot.scad>;
use <../parts/leg.metabot.scad>;

SizeA = 60;
SizeB = 20;
SizeC = 20;
SizeBottom = 10;
SizeTop = 15;
Thickness = 2.2;


rotate([90,90,0]) {
leg(SizeA, SizeB, SizeC, SizeBottom, SizeTop, "arm", 0, Thickness);
}
