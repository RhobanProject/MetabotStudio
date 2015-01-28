include <../models/motor_values.scad>;
include <../models/ollo.scad>;
use <../models/motor_arm.scad>;
use <../parts/body_arm.scad>;

//:Component "Body arms"
//:Description "Star body arms"
//:Parameter Size "Size of the body"
//:Parameter Legs "Number of legs"
//:Parameter Thickness Thickness
module body_arms(Size=25, Legs=4, Thickness=2.2)
{

    translate([0,0,-OlloWidth-(MotorWidth+Thickness)/2-Thickness/2])
        body_arm(size=Size, legs=Legs, thickness=Thickness);
    translate([0,0,OlloWidth+(MotorWidth+Thickness)/2-Thickness/2])
        body_arm(size=Size, legs=Legs, thickness=Thickness);

    if (Legs>0) {
        for (i=[1:Legs]) {
            rotate([0, 0, 360*i/Legs])
                translate([0,Size,0]) {
                    rotate([0,0,180]) {
                        //:Anchor motor female
                    }
                }
        }
    }
}

body_arms();