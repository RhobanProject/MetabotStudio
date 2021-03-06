include <../models/ollo.scad>;
include <../models/motor_values.scad>;
use <../models/motor_arm.scad>;
use <../models/arm.scad>;
use <../parts/tie_box.scad>;
use <../parts/tie_bar.scad>;

//:Component Block
//:Description "Two DOF block"
//:Parameter Thickness Thickness 
//:Parameter Internal "Internal box? (harder to mount)"
module block(Thickness=2.2, Internal=false)
{
    motorArm();
    rotate([180,0,0]) {
        //:Anchor motor male
    }

    rotate([0, -90, 0]) {
        //:Anchor motor male
    }

    translate([OlloWidth+MotorWidth/2,0,0]) {
        rotate([0, -90, 0]) {
            pulley();
        }
    }

    translate([-MotorWidth-2*OlloWidth-Thickness,0,0]) {
        rotate([0, 90, 0]) {
            motorArm();
        }
    }

    if (Internal) {
        translate([-MotorWidth/2- (2*OlloWidth+Thickness)/2, -3.5*OlloSpacing, 0]) {
            tie_box(Thickness);
        }
    } else {
        translate([-(2*OlloWidth+Thickness+OlloSpacing*2+MotorWidth/2),-OlloSpacing*3,MotorDepth/2])
            tie_bar();
        translate([-(2*OlloWidth+Thickness+OlloSpacing*2+MotorWidth/2),-OlloSpacing*3,-Thickness-MotorDepth/2])
            tie_bar();
    }
}
block();