include <../models/mx64-values.scad>;
use <../parts/mx64-u.scad>;
use <../models/mx64-with-arm.scad>;

//:Component "MX64 U Arm to side"
//:Parameter height
//:Parameter thickness
module mx64_u_arm_to_side(height=70, thickness=2)
{
    u_bottom_to_axis = -thickness/2-MX64Width/2;
    u_height = height + u_bottom_to_axis;
    
    translate([0,0,u_height])
    rotate([0,90,0]) {
        //mx64_with_arm();
        //:Anchor mx64 female
    }
    mx64_u(height=u_height, thickness=thickness, mode="");
    
    translate([0,0,u_bottom_to_axis])
    rotate([90,0,90]) {
        mx64_with_arm();
        //:Anchor mx64 male
    }
}

mx64_u_arm_to_side();