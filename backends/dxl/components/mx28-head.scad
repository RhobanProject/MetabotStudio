use<../models/mx28-with-arm.scad>;

//:Component "MX28 Head"
module mx28_head()
{
    //:Anchor mx28 male
    mx28_with_arm();
    
    translate([-50,0,0]) {
        rotate([0,-90,0])
        rotate([0,0,90]) {
            //:Tip
        }
    }
}

mx28_head();
