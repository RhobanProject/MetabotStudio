include <../models/ollo.scad>;
use <../util/rounded.scad>;

//:Part
module square_body_arm_plate(width=40, length=70, legs=4, legsSpacing=40, thickness=2.2)
{	
    legsDistance = ((legs/2)-1)*legsSpacing;
    difference() {
        union() {
            rounded(width, length, thickness, center=true);
            for (k=[1:legs/2]) {
                for (side=[1,-1]) {
                    translate([side*(width/2),
                            -legsDistance/2+(k-1)*legsSpacing,0])
                        cylinder(r=10, h=thickness);
                }
            }
        }
        for (k=[1:legs/2]) {
            for (side=[1,-1]) {
                translate([side*(width/2),
                        -legsDistance/2+(k-1)*legsSpacing,0])
                    rotate([0,0,90])
                    servoArm();
            }
        }
    }
}

square_body_arm(size=30);
