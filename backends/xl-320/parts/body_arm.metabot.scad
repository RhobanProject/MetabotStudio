NoModels = false;
use <../models/ollo.metabot.scad>;
use <../util/rounded.metabot.scad>;

module body_arm(size=20, legs=4, thickness=2.2) {
// metabot: Begining module body_arm
marker("metabot_part: body_arm");
marker("metabot_parameter: legs ", legs);
marker("metabot_parameter: size ", size);
marker("metabot_parameter: thickness ", thickness);
if (NoModels == false) {
_body_arm(legs=legs,size=size,thickness=thickness);
}
}
module _body_arm(legs=4, size=20, thickness=2.2) {
module bodyPart() {
translate([-10,0,0])
rounded(20, size+10, thickness, 9);
}

module bodyHoles() {
translate([0, size, 0]) {
servoArm(thickness*2);
}
}

difference() {
union() {
cylinder(d=size*1.6, h=thickness);
for (leg=[1:legs]) {
if (legs > 0) {
rotate([0,0,360*leg/legs])
bodyPart();
}
}
}
for (leg=[1:legs]) {
if (legs > 0) {
rotate([0,0,360*leg/legs])
bodyHoles();
}
}
}
}
// metabot: Ending module body_arm

body_arm(size=30);
