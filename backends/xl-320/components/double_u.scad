// Component motor
include <../../metabot.scad>;
use <../parts/u.scad>;
use <../models/motor_arm.scad>;

// Parameter Height of the Us
Height = 15;
// Parameter Radius of the fixation
Radius = 8;
// Parmaeter Put Screws?
Screws = true;
// Parameter Screws spacing
ScrewsSpacing = 10;
// Parameter Screws diameter
ScrewsDiameter = 2.6;
// Parameter Thickness
Thickness = 2.2;

u(Height, Radius, Thickness, ScrewsSpacing, ScrewsDiameter, Screws);
translate([0,(Height+Radius)*2,0]) {
    rotate([0,90,180]) {
        u(Height, Radius, Thickness, ScrewsSpacing, ScrewsDiameter, Screws);
        metabot_anchor("motor");
        children();
    }
}
