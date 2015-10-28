include <../models/motor_values.scad>;
include <../models/ollo.scad>;
use <../models/motor_arm.scad>;
use <../parts/body_plate.scad>;

//:Component "Body Side"
//:Description "Star body"
//:Parameter Size "Size of the body"
//:Parameter Legs "Number of legs"
//:Parameter Thickness Thickness
module body_side(Size=30, Legs=4, Thickness=2.2)
{
    rotate([0,0,-90+360/(2*Legs)]) {
        translate([0,0,-(MotorWidth+Thickness)/2-Thickness/2])
            body_plate(size=Size, legs=Legs, thickness=Thickness, double=true);
        translate([0,0,(MotorWidth+Thickness)/2-Thickness/2])
            body_plate(size=Size, legs=Legs, thickness=Thickness, double=true);

        if (Legs>0) {
            for (i=[1:Legs]) {
                rotate([0, 90, 360*i/Legs])
                    translate([0,Size+MotorHeight-MotorArmOffset-OlloSpacing*2-2.3,0]) {
                        motorArm();
                        //:Anchor motor male
                    }
            }
        }
    }
}

body_side();