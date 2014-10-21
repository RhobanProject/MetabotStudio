// Component U-Bar
// Description An U with a long bar
use <../../metabot.scad>;
use <../parts/u.scad>;
use <../parts/bar.scad>;

$fn=25;

// Parameter Bar length
BarLength = 60;
// Parameter Bar radius
BarRadius = 3;
// Parameter Bar bottom radius
BarBottomRadius = 10;
// Parameter U height
UHeight = 15;
// Parameter U radius
URadius = 8;
// Parameter Parts thickness
Thickness = 2.3;
// Parameter Screws spacing
ScrewsSpacing = 14; 
// Parameter Screws diameter
ScrewsDiameter = 2.6;
// Parameter Number of screws
ScrewsCount = 8;

metabot_anchor("motor", female=true);
u(height=UHeight, radius=URadius, thickness=Thickness,
 screwsSpacing=ScrewsSpacing, screwsDiameter=ScrewsDiameter,
 screwsCount=ScrewsCount, thickness=Thickness);
translate([0, UHeight+URadius, 0]) {
	rotate([-90,0,0]) {
		bar(
        height=BarLength, barRadius=BarRadius, bottomRadius=BarBottomRadius,
		screwsSpacing=ScrewsSpacing, screwsDiameter=ScrewsDiameter,
 screwsCount=ScrewsCount, thickness=Thickness
		);
		translate([0,0,Thickness+BarLength])
		sphere(r=BarRadius);
	}
}
