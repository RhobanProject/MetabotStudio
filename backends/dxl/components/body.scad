include <../models/mx28-values.scad>;
include <../models/mx64-values.scad>;
use <../models/mx64-with-arm.scad>;
use <../models/mx28-with-arm.scad>;
use <../parts/body_block.scad>;

//:Component "Body"
//:Parameter width
//:Parameter height
//:Parameter depth
module body(width=120, height=150, depth=80)
{
    translate([-15,0,0])
    body_block(width=width, height=height, depth=depth);
    
    for (x=[-width/2,width/2]) {
        translate([0,x,0]) {
            translate([-18,0,0])
            rotate([180,0,180]) {
                mx64_with_arm();
                //:Anchor mx64 male
            }

            translate([-18,0,height])
            rotate([0,0,x>0?180:0])
            rotate([0,270,90]) {
                mx28_with_arm();
                //:Anchor mx28 male
            }
        }
    }
    
    translate([0,0,height]) {
        mx28_with_arm();
        //:Anchor mx28 male
    }
}

body();