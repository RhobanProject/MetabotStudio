include <../models/mx28-values.scad>;
use <../parts/mx28-u.scad>;
use <../models/mx28-with-arm.scad>;

//:Component "MX28 U Arm"
//:Parameter height
//:Parameter thickness
module mx28_u_arm(height=30, thickness=2)
{
    translate([0,0,height])
    rotate([0,90,0]) {
        //mx28_with_arm();
        //:Anchor mx28 female
    }
    mx28_u(height=height, thickness=thickness);
    
    translate([0,0,-3-thickness/2-MX28Depth/2])
    rotate([0,0,90]) {
        //mx28_with_arm();
        //:Anchor mx28 female
    }
}

mx28_u_arm();
