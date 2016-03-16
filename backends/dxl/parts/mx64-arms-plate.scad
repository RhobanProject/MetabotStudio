Collisions = false;
use <../util/rounded.scad>
use <../models/mx64-arm.scad>

//:Part
module mx64_arms_plate(length=100, thickness=2)
{
    difference() {
        rounded(length+33, 28, thickness, 12, center=true);
        
        if (!Collisions) {
            translate([length/2,0,0])
                mx64_arm_holes();
            translate([-length/2,0,0])
                mx64_arm_holes();
        }
    }
}

mx64_arms_plate();