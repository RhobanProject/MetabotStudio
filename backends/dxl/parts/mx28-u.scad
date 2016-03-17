Collisions = false;
include <../models/mx28-values.scad>;
use <../util/rounded.scad>
use <../models/mx28-arm.scad>
use <../models/mx28-with-arm.scad>

//:Part
module mx28_u(thickness=2, height=35, mode="arm")
{
    difference() {
        union() {
            for (i=[-1,1]) {
                translate([i*(thickness/2+(MX28Depth+6)/2),0,0])
                translate([-thickness/2,-15,0])
                    cube([thickness,30,height+14]);
            }
            
            cube([(MX28Depth+6)+2*thickness,30,thickness], center=true);
        }
        
        if (!Collisions) {
            if (mode == "arm") {
                mx28_arm_holes();
            }
            
            for (i=[-1,1]) {
                translate([i*(thickness/2+(MX28Depth+6)/2),0,0])
                translate([0,0,height]) {
                    rotate([0,90,0])
                    mx28_arm_holes();
                }
            }
        }
    }
}

mx28_u();