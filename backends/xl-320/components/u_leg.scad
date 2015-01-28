use <../parts/u.scad>;
use <../models/motor_arm.scad>;

//:Component "U Leg"
//:Description "U Leg"
//:Parameter Height "Height of the Us"
//:Parameter Radius "Radius of the fixation"
//:Parameter Thickness 
module u_leg(Height=15, Radius=8, Thickness=2.2)
{
    //:Anchor motor female
    u(Height, Radius, Thickness, screws=false);
}

u_leg();