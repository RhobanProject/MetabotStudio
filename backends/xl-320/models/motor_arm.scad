include <../../metabot.scad>;
include <motor.scad>;
use <arm.scad>;

MotorMark = false;

module motorArm() {
    if (MotorMark) {
        metabot_anchor("motor");
        metabot_model("motor_arm");
    } else {
        motor();
        translate([0,0,MotorDepth/2])
            arm(true);
        translate([0,0,-MotorDepth/2-OlloWidth])
            arm();
    } 
}

motorArm();
