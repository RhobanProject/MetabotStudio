NoModels = false;
use <../models/ollo.metabot.scad>;
use <../util/rounded.metabot.scad>;

module body(size=20, legs=4, thickness=2.2) {
    // metabot: Begining module body
    marker("metabot_part: body");
    marker("metabot_parameter: legs ", legs);
    marker("metabot_parameter: size ", size);
    marker("metabot_parameter: thickness ", thickness);
    if (NoModels == false) {
        _body(legs=legs,size=size,thickness=thickness);
    }
}
module _body(legs=4, size=20, thickness=2.2) {
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
// metabot: Ending module body

body(size=30);
