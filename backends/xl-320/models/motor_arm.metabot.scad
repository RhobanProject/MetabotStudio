NoModels = false;
include <../../metabot.metabot.scad>;
include <motor.metabot.scad>;
use <arm.metabot.scad>;

module motorArm(counterArm=true) {
motor();
translate([0,0,MotorDepth/2])
arm();
if (counterArm) {
translate([0,0,-MotorDepth/2-OlloWidth]) {
pulley();
}
}
}

motorArm();
