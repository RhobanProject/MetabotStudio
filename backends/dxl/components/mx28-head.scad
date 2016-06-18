use<../models/mx28-with-arm.scad>;

//:Component "MX28 Head"
//:Parameter cameraHeight
//:Parameter cameraDepth
module mx28_head(cameraHeight=55, cameraDepth=10)
{
    //:Anchor mx28 male
    mx28_with_arm();
    
    translate([-cameraHeight,cameraDepth,0]) {
        rotate([0,-90,0])
        rotate([0,0,90]) {
            //:Tip
        }
    }
}

mx28_head();
