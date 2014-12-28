NoModels = false;
use <../parts/u.metabot.scad>;
use <../parts/bar.metabot.scad>;

$fn=25;

module u_bar(BarLength=60, BarRadius=3, BarBottomRadius=10, UHeight=15, URadius=8,
Thickness=2.3, ScrewsSpacing=14, ScrewsDiameter=2.6, ScrewsCount=8)
{
// metabot: Begining module u_bar
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
}
// metabot: Ending module u_bar
