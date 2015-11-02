use <../parts/u.scad>;
use <../models/motor_arm.scad>;

//:Component "Double U"
//:Description "Two Us"
//:Parameter Height "Distances between axes"
//:Parameter Radius "Radius of the fixation"
//:Parameter Screws "Put Screws?"
//:Parameter ScrewsSpacing "Screws spacing"
//:Parameter ScrewsDiameter "Screws diameter"
//:Parameter ScrewsCount "Number of screws"
//:Parameter Thickness 
//:Parameter Angle 
module double_u(Height=45, Radius=8, Screws=true, ScrewsSpacing=14, ScrewsDiameter=2.6,
        ScrewsCount=4, Thickness=2.2, Angle=90)
{
    //:Anchor motor female

    u(Height/2.0, Radius, Thickness, ScrewsSpacing, ScrewsDiameter, ScrewsCount, Screws);
    translate([0,Height,0]) {
        rotate([0,Angle,180]) {
            u(Height/2.0, Radius, Thickness, ScrewsSpacing, ScrewsDiameter, ScrewsCount, Screws);
            //:Anchor motor female
            children();
        }
    }
}

double_u();