include <../models/mx64-values.scad>;
use <../parts/mx64-u.scad>;
use <../parts/foot.scad>;
use <../models/mx64-with-arm.scad>;

//:Component "MX64 Foot"
//:Parameter height
//:Parameter thickness
//:Parameter excentricity
module mx64_foot(height=30, thickness=2, excentricity=0, cleats=12)
{
    translate([0,0,height])
        rotate([0,90,0]) {
            //mx64_with_arm();
            //:Anchor mx64 female
        }
    mx64_u(height=height, thickness=thickness, mode="none");

    translate([0,0,-thickness]) {
        translate([0,excentricity,0]) {
            foot(thickness=thickness, cleats=cleats);
        }

        translate([0,0,-cleats-9]) {
            //:Tip
        }
    }
}

mx64_foot();
