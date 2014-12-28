NoModels = false;
include <ollo.metabot.scad>;
include <motor_values.metabot.scad>;
use <../util/rounded.metabot.scad>;

module motor(thickness=2.2) {
// metabot: Begining module motor
marker("metabot_model: motor");
marker("metabot_parameter: thickness ", thickness);
if (NoModels == false) {
_motor(thickness=thickness);
}
}
module _motor(thickness=2.2) {
translate([0,-MotorArmOffset,-MotorDepth/2]) {
color([0.25,0.25,0.25]) {
difference() {
rounded(MotorWidth,MotorHeight,MotorDepth,3,center=true);

// Bottom
translate([-MotorWidth/2-0.1,-MotorHeight/2-0.1,3])
cube([MotorWidth+0.2, 5.7, MotorDepth-6]);

translate([0,-MotorHeight/2+3])
threeOllo(MotorDepth);

// Motor shaft
translate([0, MotorArmOffset])
olloHole(MotorDepth+5);

// Side holes
for (side=[-MotorWidth/2,MotorWidth/2-thickness]) {
translate([side,9,MotorDepth/2]) {
rotate([0,90,0]) {
threeOllo();
translate([0,-3*OlloSpacing,0])
threeOllo();
}
}
}
}
}
}
}
// metabot: Ending module motor

motor();
