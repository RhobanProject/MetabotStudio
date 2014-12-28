include <../models/ollo.scad>;
include <../models/motor_values.scad>;
use <../models/motor_arm.scad>;

$fn=40;

//:Part
module tie_bar(thickness=2.2)
{
    W = MotorWidth+OlloWidth+thickness+OlloWidth+MotorDepth;
    H = OlloSpacing+2*OlloWidth;

    translate([-MotorDepth/2,OlloWidth,0])
        rotate([90,0,0])
        difference() {
            cube([W,thickness,H]);

            translate([MotorDepth/2,0,H/2-OlloSpacing/2])
                rotate([-90,0,0])
                threeOllo();

            translate([W-MotorWidth/2,0,H/2+OlloSpacing/2])
                rotate([-90,0,0])
                threeOllo();
        }
}

tie_bar();
