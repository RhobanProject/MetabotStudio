Collisions = false;
include <mx28-values.scad>;
include <../util/rounded.scad>;

//:Model "MX-28" [0.15,0.2,0.15]
//:Mass 72
module mx28()
{
    // See 
    // http://support.robotis.com/en/baggage_files/dynamixel/rx28dimension.pdf
    rotate([0,0,90])
    translate([0,0,-MX28Depth/2])
    color([0.15,0.2,0.15])
    difference() {
        translate([0, 12.5, 0])
            rounded(MX28Width, MX28Height, MX28Depth, center=true);
        
        if (!Collisions) {
            translate([-50,33,3])
                cube([100,10,MX28Depth-6], center=false);
            
            cylinder(d=5, h=100, center=true);
        }
    }
}

mx28();
