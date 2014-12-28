NoModels = false;
include <../models/motor_values.metabot.scad>;
include <../models/ollo.metabot.scad>;
use <../models/motor_arm.metabot.scad>;
use <../parts/square_body_arm.metabot.scad>;

module square_body_arm(Width=40, Length=70, LegsSpacing=40, Legs=4, Thickness=2.2)
{
// metabot: Begining module square_body_arm

translate([0,0,MotorDepth/2+OlloWidth])
square_body_arm(width=Width, length=Length, legsSpacing=LegsSpacing, legs=Legs, thickness=Thickness);

translate([0,0,-Thickness-MotorDepth/2])
square_body_arm(width=Width, length=Length, legsSpacing=LegsSpacing, legs=Legs, thickness=Thickness);

LegsDistance = LegsSpacing*(Legs/2-1);
for (leg=[1:(Legs/2)]) {
for (side=[-1,1]) {
translate([-side*(Width/2),LegsDistance/2-(leg-1)*LegsSpacing,0])
rotate([0,0,90+(side==1?180:0)]) {
//:Anchor motor female
}
}
}
}
// metabot: Ending module square_body_arm
