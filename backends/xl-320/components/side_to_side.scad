include <../models/motor_values.scad>;
use <../models/motor_arm.scad>;
use <../parts/side.scad>;

//:Component "Side to side"
//:Description "Two motors tied on side"
//:Parameter Size Size
//:Parameter HolesToBorder "Holes to border distance"
//:Parameter Thickness
module side_to_side(Size=65, HolesToBorder=5, Thickness=2.2)
{
    translate([0,Size/2,0]) {
        //:Anchor motor male
        motorArm();

        for (side=[MotorWidth/2+Thickness,-MotorWidth/2]) {
            translate([side,0,0]) {
                rotate([180,90,0]) {
                    color(col)
                        side(Size, HolesToBorder, Thickness);
                }
            }
        }
        translate([0,-Size,0]) {
            rotate([0,0,180]) {
                motorArm();
                //:Anchor motor male
            }
        }
    }
}

side_to_side();