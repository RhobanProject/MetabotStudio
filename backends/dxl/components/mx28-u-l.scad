include <../models/mx28-values.scad>;
use <../parts/mx28-u.scad>;
use <../parts/mx28-l.scad>;
use <../models/mx28-with-arm.scad>;

//:Component "MX28 U L"
//:Parameter length
//:Parameter height
//:Parameter thickness
module mx28_u_l(length=80, height=30, thickness=2)
{
    mx28_l(mode="none", length=length-MX28Width+12, thickness=thickness);
    
    translate([0,MX28Width/2,-length+MX28Height-12]) 
    rotate([90,0,0]) {
        mx28_u(mode="none", height=height, thickness=thickness);
        
        translate([0,0,height])
        rotate([0,90,0]) {
            //mx28_with_arm();
            //:Anchor mx28 female
        }
    }
    
    translate([0,0,MX28Height/2+13])
    rotate([90,-90,0]) {
        mx28_with_arm();
        //:Anchor mx28 male
    }
}

mx28_u_l(length=100);
