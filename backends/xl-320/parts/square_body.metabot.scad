NoModels = false;
include <../models/ollo.metabot.scad>;
use <../util/rounded.metabot.scad>;

module square_body(width=40, length=70, legs=4, legsSpacing=40, thickness=2.2)
{
// metabot: Begining module square_body
marker("metabot_part: square_body");
marker("metabot_parameter: legs ", legs);
marker("metabot_parameter: legsSpacing ", legsSpacing);
marker("metabot_parameter: length ", length);
marker("metabot_parameter: thickness ", thickness);
marker("metabot_parameter: width ", width);
if (NoModels == false) {
_square_body(legs=legs,legsSpacing=legsSpacing,length=length,thickness=thickness,width=width);
}
}
module _square_body(legs=4, legsSpacing=40, length=70, thickness=2.2, width=40) {
legsDistance = ((legs/2)-1)*legsSpacing;
difference() {
union() {
rounded(width, length, thickness, center=true);
for (k=[1:legs/2]) {
for (side=[1,-1]) {
translate([side*(width/2),
-legsDistance/2+(k-1)*legsSpacing,0])
rounded(8,22,thickness,center=true);
}
}
}
for (k=[1:legs/2]) {
for (side=[1,-1]) {
translate([side*(width/2),
-legsDistance/2+(k-1)*legsSpacing,0])
rotate([0,0,90])
threeOllo();
}
}
}
}
// metabot: Ending module square_body

square_body(size=30);
