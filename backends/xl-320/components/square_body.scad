include <../models/motor_values.scad>;
include <../models/ollo.scad>;
use <../models/motor_arm.scad>;
use <../parts/square_body_plate.scad>;

//:Component "Square Body"
//:Description "Square body"
//:Parameter Width "Width of the body"
//:Parameter Length "Length of the body"
//:Parameter LegsSpacing "Spacing between legs"
//:Parameter Legs "Number of legs (multiple of 2)"
//:Parameter Thickness
module square_body(Width=40, Length=70, LegsSpacing=40, Legs=4, Thickness=2.2)
{
    translate([0,0,MotorDepth/2]) {
        square_body_plate(width=Width, length=Length, legsSpacing=LegsSpacing, legs=Legs, thickness=Thickness);
    }

    translate([0,0,-Thickness-MotorDepth/2]) {
        square_body_plate(width=Width, length=Length, legsSpacing=LegsSpacing, legs=Legs, thickness=Thickness);
    }

    LegsDistance = LegsSpacing*(Legs/2-1);
    for (leg=[1:(Legs/2)]) {
        for (side=[-1,1]) {
            translate([-side*(Width/2),LegsDistance/2-(leg-1)*LegsSpacing,0])
                rotate([0,0,side*90])
                translate([0,OlloSpacing*4,0]) {
                    motorArm();
                    //:Anchor motor male
                }
        }
    }
}