use <../parts/leg.scad>;
use <../models/motor_arm.scad>;

//:Component "Bottom leg (angle)"
//:Description "Leg on motor bottom"
//:Parameter SizeA "Size of the first part"
//:Parameter SizeB "Size of the second part"
//:Parameter SizeC "Size of the third part"
//:Parameter SizeBottom "Size of the bottom"
//:Parameter SizeTop "Size of the top"
//:Parameter FixationAngle "Fixation angle"
//:Parameter Thickness "Thickness of the part"
module bottom_leg_angle(SizeA=60, SizeB=20, SizeC=20, SizeBottom=10,
        SizeTop=15, FixationAngle=0, Thickness=2.2)
{
    //:Anchor motor male
    motorArm();

    translate([0,-(24),0]) {
    rotate([0,-90,-FixationAngle]) {
	leg(SizeA, SizeB, SizeC, SizeBottom, SizeTop, "side", FixationAngle, Thickness);
    }
    }
}

bottom_leg_angle();
