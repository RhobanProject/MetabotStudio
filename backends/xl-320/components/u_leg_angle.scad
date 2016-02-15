Collisions=false;
use <../parts/u.scad>;
use <../parts/leg.scad>;
use <../models/motor_arm.scad>;

//:Component "U Leg angle"
//:Description "U Leg angle"
//:Parameter Angle "Fixation angle"
//:Parameter SizeA "Lenght of the first part"
//:Parameter SizeB "Lenght of the first part"
//:Parameter SizeC "Lenght of the first part"
//:Parameter Height "Height of the Us"
//:Parameter Thickness 
module u_leg_angle(Height=25, Thickness=2.2, Angle=0, SizeA=60, SizeB=20, SizeC=20)
{
    //:Anchor motor female
    u(Height, Thickness, screws=false, radius=0);
    translate([0,Height-Thickness/2])
        cube([30+2*Thickness,Thickness,30+2*Thickness], center=true);
    
    translate([0,abs(sin(Angle))*12+Height,0]) {
        rotate([Angle,0,0])
        translate([0,10,0])
        leg(mode="", sizeTop=0, bevelCorner=true, sizeA=SizeA, sizeB=SizeB, sizeC=SizeC);
    }
    
    translate([0,Height,0])
    rotate([0,Angle>0?0:180,Angle>0?180:0])
    rotate([90,180,90])
    linear_extrude(30+2*Thickness, center=true)
    polygon(points=[[0,15],[0,-15],[sin(Angle)*40,15]], paths=[[0,1,2]]);
}

u_leg_angle(Angle=0);