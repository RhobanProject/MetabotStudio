NoModels = false;
include <../models/motor_values.metabot.scad>;
include <../models/ollo.metabot.scad>;

module tie_box(thickness=2.2, print=false)
{
// metabot: Begining module tie_box
marker("metabot_part: tie_box");
marker("metabot_parameter: print ", print);
marker("metabot_parameter: thickness ", thickness);
if (NoModels == false) {
_tie_box(print=print,thickness=thickness);
}
}
module _tie_box(print=false, thickness=2.2) {
SX = OlloWidth*2+thickness;
SY = OlloSpacing+4*OlloWidth;
SZ = 3*OlloSpacing+3*OlloWidth;
T = print ? [0,0,0] : [-SX/2, -SY/2, -SZ/2];
translate(T)
difference() {
cube([SX, SY, SZ], center);
translate([0, SY/2-OlloSpacing/2, SZ/2]) {
rotate([0,90,0])
threeOllo();
}
translate([SX-thickness, SY/2+OlloSpacing/2, SZ/2]) {
rotate([0,90,0])
threeOllo();
}
translate([0, SY/2, thickness]) {
cube([thickness, (SY-2*thickness)/2, SZ-2*thickness]);
}
translate([SX-thickness, thickness, thickness]) {
cube([thickness, (SY-2*thickness)/2, SZ-2*thickness]);
}

translate([thickness, thickness, 0]) {
cube([SX-2*thickness, SY-2*thickness, SZ]);
}
echo((SY-2*thickness)/2);
}
}
// metabot: Ending module tie_box

tie_box();
