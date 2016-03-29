include <../models/mx64-values.scad>;
use <../parts/mx64-u.scad>;
use <../models/mx64-with-arm.scad>;

//:Component "MX64 U Arm to side"
//:Parameter height
//:Parameter thickness
module mx64_u_arm_to_side(height=60, thickness=2)
{
    translate([0,0,height])
    rotate([0,90,0]) {
        //mx64_with_arm();
        //:Anchor mx64 female
    }
    mx64_u(height=height, thickness=thickness, mode="");
    
    translate([0,0,-thickness/2-MX64Width/2])
    rotate([90,0,90]) {
        mx64_with_arm();
        //:Anchor mx64 male
    }
}

mx64_u_arm_to_side();