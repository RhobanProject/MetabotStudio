include <../../metabot.scad>;
include <../models/ollo.scad>;
use <../util/rounded.scad>;

module arm_to_arm(distance=50, thickness=2.2)
{
    metabot_part("arm_to_arm");
    metabot_parameter("distance", distance);
    metabot_parameter("thickness", thickness);
	
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