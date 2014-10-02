include <../../metabot.scad>;
include <motor.scad>;
use <arm.scad>;

NoModels = false;

module motorArm(counterArm=true) {
    if (NoModels) {
        metabot_model("motor", col=[0.4, 0.45, 0.4]);
        translate([0,0,MotorDepth/2]) {
            metabot_model("arm", col=[0.3, 0.3, 0.3]);
        }
        translate([0,0,-MotorDepth/2-OlloWidth]) {
            metabot_model("arm", col=[0.6, 0.6, 0.6]);
        }
    } else {
        motor();
        translate([0,0,MotorDepth/2])
            arm();
        if (counterArm) {
            translate([0,0,-MotorDepth/2-OlloWidth])
                pulley();
        }
    } 
}

motorArm();
