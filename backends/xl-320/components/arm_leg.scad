use <../../metabot.scad>;
use <../parts/leg.scad>;

//:Component Arm leg
//:Description Leg on arm
//:Parameter SizeA Size of the first part
//:Parameter SizeB Size of the second part
//:Parameter SizeC Size of the third part
//:Parmaeter SizeBottom Size of the bottom
//:Parameter SizeTop Size of the top
//:Parameter Thickness Thickness of the part
module arm_leg(SizeA=60, SizeB=20, SizeC=20, SizeBottom=10, SizeTop=15, Thickness=2.2)
{
    //:Anchor motor female
    rotate([90,90,0]) {
        leg(SizeA, SizeB, SizeC, SizeBottom, SizeTop, "arm", 0, Thickness);
    }
}
