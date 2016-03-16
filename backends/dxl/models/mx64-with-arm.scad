Collisions = false;
use<mx64.scad>;
use<mx64-arm.scad>;

module mx64_with_arm()
{
    mx64();
    
    if (!Collisions) {
        translate([0,0,41/2])
            mx64_arm();
        translate([0,0,-3-41/2])
            mx64_arm();
    }
}

mx64_with_arm();