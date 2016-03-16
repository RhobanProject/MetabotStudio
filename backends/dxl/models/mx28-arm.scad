Collisions = false;
$fn=30;

module mx28_arm_holes()
{
    for (i=[1:8]) {
                rotate([0,0,i*360/8])
                translate([0,8,0])
                    cylinder(d=2, h=10, center=true);
            }
            
            cylinder(d=3, h=10, center=true);
}

//:Model "MX-28 Arm" [0.5,0.5,0.5]
module mx28_arm()
{
    if (!Collisions) {
        color([0.5,0.5,0.5])
        difference() {
            cylinder(d=22, h=3);
            
            mx28_arm_holes();
        }
    }
}

mx28_arm();
