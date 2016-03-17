Collisions = false;
include <../models/mx28-values.scad>;

//:Part
module mx28_l(mode="arm", length=100, thickness=2, armSize=20)
{
    translate([0,thickness,0])
    cube([MX28Width,MX28Width+thickness,thickness], center=true);
    
    translate([0,MX28Width/2+thickness,-length/2])
        cube([MX28Width,thickness,length], center=true);
    
    if (mode == "arm") {
        translate([0,MX28Width/2+thickness/2+thickness-armSize/2,-length])
        rotate([0,90,0])
        difference() {
            cylinder(d=armSize, h=MX28Width, center=true);
            if (!Collisions) {
                cylinder(d=armSize-thickness*2, h=1+MX28Width, center=true);
            }
        }
    }
}

mx28_l();