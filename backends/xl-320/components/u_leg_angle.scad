Collisions=false;
use <../parts/u.scad>;
use <../parts/leg.scad>;
use <../models/motor_arm.scad>;

//:Component "U Leg angle"
//:Description "U Leg angle"
//:Parameter Angle "Fixation angle"
//:Parameter Height "Height of the Us"
//:Parameter Thickness 
module u_leg_angle(Height=25, Thickness=2.2, Angle=0)
{
    //:Anchor motor female
    u(Height, Thickness, screws=false, radius=0);
    translate([0,Height-Thickness/2])
        cube([30+2*Thickness,Thickness,30+2*Thickness], center=true);
    
    translate([0,sin(Angle)*12+Height,0]) {
        rotate([Angle,0,0])
        translate([0,10,0])
        leg(mode="", sizeTop=0, bevelCorner=true);
    }
    
    translate([0,Height,0])
    rotate([0,0,180])
    rotate([90,180,90])
    linear_extrude(30+2*Thickness, center=true)
    polygon(points=[[0,15],[0,-15],[sin(Angle)*40,15]], paths=[[0,1,2]]);
}

u_leg_angle(Angle=0);