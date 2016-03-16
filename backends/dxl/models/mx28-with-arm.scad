Collisions = false;
include<mx28-values.scad>;
use<mx28.scad>;
use<mx28-arm.scad>;
use<tag.scad>;

module mx28_with_arm()
{
    mx28();
    
    if (!Collisions) {
        translate([0,0,MX28Depth/2])
            mx28_arm();
        translate([0,0,-3-MX28Depth/2])
            mx28_arm();
        translate([-25,0,MX28Depth/2])
            rotate([0,0,90])
            tag();
    }
}

mx28_with_arm();