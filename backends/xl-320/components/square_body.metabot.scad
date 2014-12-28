NoModels = false;
include <../models/motor_values.metabot.scad>;
include <../models/ollo.metabot.scad>;
use <../models/motor_arm.metabot.scad>;
use <../parts/square_body.metabot.scad>;

module square_body(Width=40, Length=70, LegsSpacing=40, Legs=4, Thickness=2.2)
{
// metabot: Begining module square_body
translate([0,0,MotorDepth/2])
square_body(width=Width, length=Length, legsSpacing=LegsSpacing, legs=Legs, thickness=Thickness);

translate([0,0,-Thickness-MotorDepth/2])
square_body(width=Width, length=Length, legsSpacing=LegsSpacing, legs=Legs, thickness=Thickness);

LegsDistance = LegsSpacing*(Legs/2-1);
for (leg=[1:(Legs/2)]) {
for (side=[-1,1]) {
translate([-side*(Width/2),LegsDistance/2-(leg-1)*LegsSpacing,0])
rotate([0,0,side*90])
translate([0,OlloSpacing*4,0]) {
motorArm();
metabot_anchor("motor", male=true);
}
}
}
}
// metabot: Ending module square_body
