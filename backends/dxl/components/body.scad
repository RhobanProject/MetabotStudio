include <../models/mx28-values.scad>;
include <../models/mx64-values.scad>;
use <../models/mx64-with-arm.scad>;
use <../models/mx28-with-arm.scad>;
use <../parts/body_block.scad>;
use <../models/fit.scad>;
use <../models/lipo.scad>;

//:Component "Body"
//:Parameter width
//:Parameter height
//:Parameter depth
//:Parameter headHeight
module body(width=120, height=150, headHeight=185, depth=80)
{
    rotate([0,0,180])
    body_block(width=width, height=height, depth=depth);
    
    translate([-25,0,3+MX64Depth]) {
        rotate([0,0,90]) {
            fit();
        }
    }
    
    translate([20,0,3+MX64Depth]) {
        rotate([0,0,90]) {
            lipo();
        }  
    }
    
    translate([MX64ShaftOffset,0,MX64Depth/2]) {
        
        for (x=[-width/2,width/2]) {
            translate([0,x,0]) {
                translate([-MX64ShaftOffset,0,0])
                rotate([180,0,180]) {
                    mx64_with_arm();
                    //:Anchor mx64 male
                }

                translate([-MX64ShaftOffset,0,height])
                rotate([0,0,x>0?180:0])
                rotate([0,270,90]) {
                    mx28_with_arm();
                    //:Anchor mx28 male
                }
            }
        }
    }
            
    translate([18,0,-MX28Depth/2+headHeight]) {
        mx28_with_arm();
        //:Anchor mx28 male
    }
}

body(width=93, height=135, headHeight=185);