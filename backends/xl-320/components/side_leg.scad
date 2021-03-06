use <../parts/leg.scad>;
use <../models/motor_arm.scad>;

//:Component "Side leg"
//:Description "Leg on motor side"
//:Parameter SizeA "Size of the first part"
//:Parameter SizeB "Size of the second part"
//:Parameter SizeC "Size of the third part"
//:Parameter SizeBottom "Size of the bottom"
//:Parameter SizeTop "Size of the top"
//:Parameter FixationAngle "Fixation angle"
//:Parameter Thickness "Thickness of the part"
module side_leg(SizeA=60, SizeB=20, SizeC=20, SizeBottom=10,
        SizeTop=15, FixationAngle=0, Thickness=2.2)
{
    //:Anchor motor male
    motorArm();

    translate([0,-(18),0]) {
    rotate([-FixationAngle,0,0]) {
	leg(SizeA, SizeB, SizeC, SizeBottom, SizeTop, "side", FixationAngle, Thickness);
    }
    }
}

side_leg();
