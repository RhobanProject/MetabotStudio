use <../models/motor_arm.scad>;
use <../parts/arm_side.scad>;

//:Component "Arms to side"
//:Description "Motor arms to another sides"
//:Parameter FirstPart "First part"
//:Parameter Distance "The distance"
//:Parameter Thickness "Part thickness"
module arm_to_side(FirstPart=17, Distance=45, Thickness=2.2)
{
    //:Anchor motor female

    arm_side(firstPart=FirstPart , distance=Distance, thickness=Thickness);
    rotate([0,180,0]) {
        arm_side(firstPart=FirstPart , distance=Distance, thickness=Thickness);
    }

    translate([0,Distance,0]) {
        rotate([0,90,0]) {
            motorArm();
            //:Anchor motor male
        }
    }
}
