NoModels = false;
include <ollo.metabot.scad>;

module arm(pulley=false) {
// metabot: Begining module arm
marker("metabot_model: arm");
marker("metabot_parameter: pulley ", pulley);
if (NoModels == false) {
_arm(pulley=pulley);
}
}
module _arm(pulley=false) {
c = pulley?[0.6,0.6,0.6]:[0.3,0.3,0.3];
color(c) {
difference() {
cylinder(d=20, h=OlloWidth);
servoArm(OlloWidth);
}
}
}
// metabot: Ending module arm

module pulley() {
arm(true);
}

arm();
