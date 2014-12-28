use <../parts/u.scad>;
use <../models/motor_arm.scad>;

//:Component Double U
//:Description Two Us
//:Parameter Height Height of the Us
//:Parameter Radius Radius of the fixation
//:ParameterBool Screws Put Screws?
//:Parameter ScrewsSpacing Screws spacing
//:Parameter ScrewsDiameter Screws diameter
//:Parameter ScrewsCount Number of screws
//:Parameter Thickness 
//:Parameter Angle 
module double_u(Height=15, Radius=8, Screws=true, ScrewsSpacing=14, ScrewsDiameter=2.6,
        ScrewCount=8, Thickness=2.2, Angle=90)
{
    //:Anchor motor female
    metabot_anchor("motor", female=true);

    u(Height, Radius, Thickness, ScrewsSpacing, ScrewsDiameter, ScrewsCount, Screws);
    translate([0,(Height+Radius)*2,0]) {
        rotate([0,Angle,180]) {
            u(Height, Radius, Thickness, ScrewsSpacing, ScrewsDiameter, ScrewsCount, Screws);
            //:Anchor motor female
            children();
        }
    }
}
