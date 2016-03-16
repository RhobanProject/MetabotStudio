Collisions = false;
include<mx64-values.scad>;
use<mx64.scad>;
use<mx64-arm.scad>;
use<tag.scad>;

module mx64_with_arm()
{
    mx64();
    
    if (!Collisions) {
        translate([0,0,MX64Depth/2])
            mx64_arm();
        translate([0,0,-3-MX64Depth/2])
            mx64_arm();
        translate([-35,0,MX64Depth/2])
            rotate([0,0,90])
            tag();
    }
}

mx64_with_arm();