include <../models/mx28-values.scad>;
use <../parts/mx28-u.scad>;
use <../parts/mx28-l.scad>;
use <../models/mx28-with-arm.scad>;

//:Component "MX28 Elbow"
//:Parameter length
//:Parameter thickness
module mx28_elbow(length=80, thickness=2)
{
    rotate([0,0,90])
    mx28_l(mode="arm", length=length-MX28Height+12, thickness=thickness);
    
    translate([0,0,MX28Height/2+13])
    rotate([90,-90,0]) {
        mx28_with_arm();
        //:Anchor mx28 male
    }
}

mx28_elbow();
