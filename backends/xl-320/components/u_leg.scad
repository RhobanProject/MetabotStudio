use <../parts/u.scad>;
use <../models/motor_arm.scad>;

//:Component "U Leg"
//:Description "U Leg"
//:Parameter Height "Height of the Us"
//:Parameter Thickness 
module u_leg(Height=15, Thickness=2.2)
{
    //:Anchor motor female
    u(Height, Thickness, screws=false);
    
    translate([0,Height,0]) {
        //:Tip
    }
}

u_leg();