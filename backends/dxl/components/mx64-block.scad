include <../models/mx64-values.scad>;
use <../parts/mx64-arms-plate.scad>;
use <../models/mx64-with-arm.scad>;
use <../models/mx64-arm.scad>;

//:Component "MX64 Block"
module mx64_block()
{
    mx64_with_arm();
    
    translate([0,0,MX64Depth+6]) {
        rotate([90,0,0]) {
            //:Anchor mx64 male
            mx64_with_arm();
        }
        
        //:Anchor mx64 male
        //#mx64_with_arm();
        
        translate([0,0,MX64Width/2])
            mx64_arm();
    }
}

mx64_block();