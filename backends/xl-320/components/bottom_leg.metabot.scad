NoModels = false;
use <../parts/leg.metabot.scad>;
use <../models/motor_arm.metabot.scad>;

module bottom_leg(SizeA=60, SizeB=20, SizeC=20, SizeBottom=10,
SizeTop=15, FixationAngle=0, Thickness=2.2)
{
// metabot: Begining module bottom_leg
//:Anchor motor male
motorArm();

translate([0,-(24),0]) {
rotate([180,0,0]) {
rotate([180,90,FixationAngle]) {
leg(SizeA, SizeB, SizeC, SizeBottom, SizeTop, "side", FixationAngle, Thickness);
}
}
}
}
// metabot: Ending module bottom_leg
