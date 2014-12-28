NoModels = false;
use <../models/ollo.metabot.scad>;
use <../util/screws.metabot.scad>;

module u(height=15, radius=8, thickness=2.2, screwsSpacing=14, screwsDiameter=2.6, screwsCount=8, screws=true, olloArm=false, armAngle=0, thicknessSize=15, print=false) {
// metabot: Begining module u
marker("metabot_part: u");
marker("metabot_parameter: armAngle ", armAngle);
marker("metabot_parameter: height ", height);
marker("metabot_parameter: olloArm ", olloArm);
marker("metabot_parameter: print ", print);
marker("metabot_parameter: radius ", radius);
marker("metabot_parameter: screws ", screws);
marker("metabot_parameter: screwsCount ", screwsCount);
marker("metabot_parameter: screwsDiameter ", screwsDiameter);
marker("metabot_parameter: screwsSpacing ", screwsSpacing);
marker("metabot_parameter: thickness ", thickness);
marker("metabot_parameter: thicknessSize ", thicknessSize);
if (NoModels == false) {
_u(armAngle=armAngle,height=height,olloArm=olloArm,print=print,radius=radius,screws=screws,screwsCount=screwsCount,screwsDiameter=screwsDiameter,screwsSpacing=screwsSpacing,thickness=thickness,thicknessSize=thicknessSize);
}
}
module _u(armAngle=0, height=15, olloArm=false, print=false, radius=8, screws=true, screwsCount=8, screwsDiameter=2.6, screwsSpacing=14, thickness=2.2, thicknessSize=15) {
module Ubranch() {
union() {
cylinder(thickness, 10, 10);
translate([-10,0,0])
cube([20, height, thickness]);
}
}

module USide() {
difference() {
union() {
Ubranch();
translate([-10,height+radius-thickness,radius])
cube([20,thickness,thicknessSize-(radius-thickness)]);
translate([-10,height,radius]) {
rotate([0,90,0]) {
difference() {
cylinder(20,radius,radius);
translate([0,0,-1])
cylinder(22,radius-thickness,radius-thickness);
translate([-10+radius-thickness,-10,-1])
cube([10,10,22]);
translate([-10,-20+radius-thickness,-1])
cube([10,20.01,22]);
}
}
}
}
servoArm(depth=6);
}
}

module UScrews() {
rotate([270,0,0]) {
screws(screwsCount, screwsSpacing, screwsDiameter);
}
}

module UOllo() {
rotate([-90,armAngle,0])
servoArm(2*height);
}

if (print) {
translate([0,0,10])
rotate([0,90,0])
u(height,radius,thickness,screwsSpacing,screwsDiameter,
screws,olloArm,armAngle,thicknessSize,false);
} else {
difference() {
translate([0,0,-thicknessSize-thickness]) {
USide();
mirror([0,0,1]) translate([0,0,-2*thickness-2*thicknessSize])
USide();
}
if (screws) {
UScrews();
}
if (olloArm) {
UOllo();
}
}
}
}
// metabot: Ending module u

u(olloArm=true, screws=false, armAngle=10);
