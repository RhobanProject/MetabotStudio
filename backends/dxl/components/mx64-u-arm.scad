include <../models/mx64-values.scad>;
use <../parts/mx64-u.scad>;
use <../models/mx64-with-arm.scad>;

//:Component "MX64 U Arm"
//:Parameter height
//:Parameter thickness
module mx64_u_arm(height=30, thickness=2)
{
    translate([0,0,height])
    rotate([0,90,0]) {
        //mx64_with_arm();
        //:Anchor mx64 female
    }
    mx64_u(height=height, thickness=thickness);
    
    translate([0,0,-3-thickness/2-MX64Depth/2])
    rotate([0,0,90]) {
        //mx64_with_arm();
        //:Anchor mx64 female
    }
}

mx64_u_arm();