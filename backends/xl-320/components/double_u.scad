// Component Double U
// Description Two Us
include <../../metabot.scad>;
use <../parts/u.scad>;
use <../models/motor_arm.scad>;

// Parameter Height of the Us
Height = 15;
// Parameter Radius of the fixation
Radius = 8;
// Parameter Put Screws?
Screws = true;
// Parameter Screws spacing
ScrewsSpacing = 14;
// Parameter Screws diameter
ScrewsDiameter = 2.6;
// Parameter Number of screws
ScrewsCount = 8;
// Parameter Thickness
Thickness = 2.2;
// Parameter Angle
Angle = 90;

metabot_anchor("motor", female=true);

u(Height, Radius, Thickness, ScrewsSpacing, ScrewsDiameter, ScrewsCount, Screws);
translate([0,(Height+Radius)*2,0]) {
    rotate([0,Angle,180]) {
        u(Height, Radius, Thickness, ScrewsSpacing, ScrewsDiameter, ScrewsCount, Screws);
        metabot_anchor("motor", female=true);
        children();
    }
}
