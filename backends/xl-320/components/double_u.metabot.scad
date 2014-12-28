NoModels = false;
use <../parts/u.metabot.scad>;
use <../models/motor_arm.metabot.scad>;

module double_u(Height=15, Radius=8, Screws=true, ScrewsSpacing=14, ScrewsDiameter=2.6,
ScrewCount=8, Thickness=2.2, Angle=90)
{
// metabot: Begining module double_u
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
// metabot: Ending module double_u
