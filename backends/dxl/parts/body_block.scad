Collisions = false;
include<../models/mx28-values.scad>;
include<../models/mx64-values.scad>;

//:Part
//:Mass 600
module body_block(width=130, height=180, depth=80)
{
    difference() {
        union() {
            translate([-depth*2/3,-width/2-MX64Width/2,MX64Depth/2])
                cube([depth, MX64Width+width, 13+height-MX64Depth/2-MX28Height]);
            
            translate([-depth*2/3,-(width-MX28Depth)/2,height-MX28Height+13]) {
                cube([depth,width-MX28Depth,20]);
            }
        }
        
        translate([-depth*2/3+2.5,2.5-width/2-MX64Width/2,MX64Depth/2-0.1])
            cube([depth-5, MX64Width+width-5, -5+13+height-MX64Depth/2-MX28Height]);
    }
}

body_block();