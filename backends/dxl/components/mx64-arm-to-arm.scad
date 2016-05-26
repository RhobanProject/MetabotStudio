include <../models/mx64-values.scad>;
use <../parts/mx64-arms-plate.scad>;
use <../models/mx64-with-arm.scad>;

//:Component "MX64 Arm to arm"
//:Parameter length
module mx64_arms(length=100)
{
    translate([length/2,0,0]) {
        rotate([0,0,180]) {
            //:Anchor mx64 female
            //mx64_with_arm();
        }
    }
    translate([-length/2,0,0]) {
        //:Anchor mx64 female
        //mx64_with_arm();
    }
    
    translate([0,0,3+MX64Depth/2])
        mx64_arms_plate(length=length);
    translate([0,0,-(3+3+MX64Depth/2)])
        mx64_arms_plate(length=length);
}

mx64_arms(length=100);