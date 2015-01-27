include <../models/motor_values.scad>;
include <../models/ollo.scad>;
use <../models/motor_arm.scad>;

$fn=20;

//:Part
module arm_side(distance=40, radius=10, firstPart=12, thickness=2.2, print=false)
{
    delta = ((MotorDepth+2*OlloWidth)-MotorWidth)/2.0;
    lastD = (distance-firstPart-thickness/2)+OlloWidth*3/2;

    if (print) {
        translate([0,0,radius])
            rotate([0,-90,0])
            translate([0,0,-(OlloWidth+MotorDepth/2)])
            arm_side(distance, radius, firstPart, thickness, false);
    } else {
        translate([0,0,OlloWidth+MotorDepth/2]) {
            difference() {
                union() {
                    cylinder(r=radius,h=thickness);
                    translate([0,firstPart/2,thickness/2]) {
                        cube([radius*2,firstPart,thickness], center=true);
                        translate([0,thickness/2+firstPart/2,thickness/2-(delta+thickness)/2]) {
                            cube([radius*2,thickness,delta+thickness], center=true);
                            translate([0,lastD/2,-delta/2]) {
                                difference() {
                                    cube([radius*2,lastD,thickness],center=true);
                                    translate([0,lastD/2-OlloWidth*3/2,0]) {
                                        translate([0,0,-thickness/2])
                                            threeOllo(thickness);
                                        translate([0,-OlloSpacing*3,-thickness/2])
                                            threeOllo(thickness);
                                    }
                                }
                            }
                        }
                    }
                }
                servoArm(thickness);
            }
        }
    }
}

/*
   distance=42;
   arm_side(firstPart=15);

   motorArm();
   translate([0,distance,0])
   rotate([0,90,0])
   motorArm();
 */