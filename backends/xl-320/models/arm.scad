include <ollo.scad>;

module arm(black=false) {
    c=black?[0.3,0.3,0.3]:[0.6,0.6,0.6];
    color(c) {
        difference() {
            cylinder(d=20, h=OlloWidth);
            servoArm(OlloWidth);
        }
    }
}

arm();