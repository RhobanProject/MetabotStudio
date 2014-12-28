include <../models/ollo.scad>;
use <../util/rounded.scad>;

//:Part
module arm_to_arm(distance=50, thickness=2.2)
{	
    translate([0,distance/2,0])
        difference() {
            rounded(OlloSpacing*4, distance+OlloSpacing*3, thickness, r=10, center=true);
            translate([0,distance/2,0])
                servoArm();
            translate([0,-distance/2,0])
                servoArm();
        }
}

arm_to_arm();
