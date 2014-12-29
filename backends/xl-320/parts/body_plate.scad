use <../models/ollo.scad>;
use <../util/rounded.scad>;

//:Part
module body_plate(size=20, legs=4, thickness=2.2) {
    module bodyPart() {
        translate([-10,0,0])
            rounded(20, size, thickness, 5);
    }

    module bodyHoles() {
        translate([0, size-5, 0]) {
            threeOllo(thickness*2);
        }
    }

    difference() {
        union() {
            cylinder(d=size*1.8, h=thickness);
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

body(size=30);
