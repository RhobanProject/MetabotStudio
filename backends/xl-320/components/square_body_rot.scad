include <../models/motor_values.scad>;
include <../models/ollo.scad>;
use <../models/motor_arm.scad>;
use <../parts/square_body_rot_plate.scad>;

//:Component "Square Body Rot"
//:Description "Square body"
//:Parameter Width "Width of the body"
//:Parameter LegsSpacing "Spacing between legs"
//:Parameter Legs "Number of legs (multiple of 2)"
//:Parameter Thickness
module square_body_rot(Width=40, LegsSpacing=90, Legs=4, Thickness=2.2)
{
    rotate([0,0,90]) {
    Length = LegsSpacing*(Legs/2-1)+24;
    translate([0,0,MotorDepth/2]) {
        square_body_rot_plate(width=Width, length=Length, legsSpacing=LegsSpacing, legs=Legs, thickness=Thickness, double=true);
    }

    translate([0,0,-Thickness-MotorDepth/2]) {
        square_body_rot_plate(width=Width, length=Length, legsSpacing=LegsSpacing, legs=Legs, thickness=Thickness, double=true);
    }

    LegsDistance = LegsSpacing*(Legs/2-1);
    for (leg=[1:(Legs/2)]) {
        for (side=[-1,1]) {
            X=LegsDistance/2-(leg-1)*LegsSpacing;
            translate([-side*(Width/2),X,0])
                rotate([0,-side*90,side*90])
                translate([0,0,-OlloSpacing+(X<0?2*OlloSpacing:0)]) {
                    rotate([X<0?90:-90,0,X<0?180:0]) {
                        motorArm();
                        //:Anchor motor male
                    }
                }
        }
    }
}
}

square_body_rot();