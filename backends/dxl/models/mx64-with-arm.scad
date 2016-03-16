Collisions = false;
include<mx64-values.scad>;
use<mx64.scad>;
use<mx64-arm.scad>;

module mx64_with_arm()
{
    mx64();
    
    if (!Collisions) {
        translate([0,0,MX64Depth/2])
            mx64_arm();
        translate([0,0,-3-MX64Depth/2])
            mx64_arm();
    }
}

mx64_with_arm();