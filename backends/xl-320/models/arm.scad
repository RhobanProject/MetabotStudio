include <ollo.scad>;

module arm(pulley=false) {
    c=pulley?[0.6,0.6,0.6]:[0.3,0.3,0.3];
    color(c) {
        difference() {
            cylinder(d=20, h=OlloWidth);
            servoArm(OlloWidth);
        }
    }
}

module pulley() {
    arm(true);
}

arm();
