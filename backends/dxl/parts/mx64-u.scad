Collisions = false;
include <../models/mx64-values.scad>;
use <../util/rounded.scad>
use <../models/mx64-arm.scad>

//:Part
module mx64_u(thickness=2, height=35, mode="arm")
{
    difference() {
        union() {
            for (i=[-1,1]) {
                translate([i*(thickness/2+(MX64Depth+6)/2),0,0]) {
                translate([-thickness/2,-15,0])
                    cube([thickness,30,height]);
                    translate([-thickness/2,0,height])
                    rotate([0,90,0])
                    cylinder(d=30, h=thickness);
                }
            }
            
            cube([(MX64Depth+6)+2*thickness,30,thickness], center=true);
        }
        
        if (!Collisions) {
            if (mode == "arm") {
                mx64_arm_holes();
            }
            
            for (i=[-1,1]) {
                translate([i*(thickness/2+(MX64Depth+6)/2),0,0])
                translate([0,0,height]) {
                    rotate([0,90,0])
                    mx64_arm_holes();
                }
            }
        }
    }
}

mx64_u();