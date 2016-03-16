Collisions = false;
include <mx64-values.scad>;
include <../util/rounded.scad>;

//:Model "MX-64" [0.15,0.2,0.15]
//:Mass 126
module mx64()
{
    // See 
    // http://support.robotis.com/en/baggage_files/dynamixel/rx64dimension.pdf
    rotate([0,0,90])
    translate([0,0,-41/2])
    color([0.15,0.2,0.15])
    difference() {
        translate([0, 18, 0])
            rounded(MX64Width, MX64Height, MX64Depth, center=true);
        
        if (!Collisions) {
            translate([0,46,0])
                cube([31,10,100], center=true);
            
            cylinder(d=5, h=100, center=true);
        }
    }
}

mx64();
