use <../parts/leg.scad>;
use <../models/motor_arm.scad>;

//:Component "Bottom leg"
//:Description "Leg on motor bottom"
//:Parameter SizeA "Size of the first part"
//:Parameter SizeB "Size of the second part"
//:Parameter SizeC "Size of the third part"
//:Parameter SizeBottom "Size of the bottom"
//:Parameter SizeTop "Size of the top"
//:Parameter FixationAngle "Fixation angle"
//:Parameter Thickness "Thickness of the part"
module bottom_leg(SizeA=20, SizeB=10, SizeC=20, SizeBottom=10,
        SizeTop=3, FixationAngle=0, Thickness=2.2)
{
    //:Anchor motor male
    motorArm();

    translate([0,-(24),0]) {
    rotate([0,90,90-FixationAngle]) {
	leg(SizeA, SizeB, SizeC, SizeBottom, SizeTop, "bottom", FixationAngle, Thickness, holesPosition=5);
    }
    }
}

bottom_leg();
