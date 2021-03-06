Collisions=false;
use <../models/ollo.scad>;
use <../util/screws.scad>;

//:Part
module u(height=22, radius=8, thickness=2.2, screwsSpacing=14, screwsDiameter=2.6, screwsCount=4, screws=true, olloArm=false, armAngle=0, thicknessSize=15, print=false) {
    module Ubranch() {
        union() {  
            cylinder(thickness, 10, 10);
            translate([-10,0,0])
                cube([20, height-radius, thickness]);
        }
    }

    module USide() {
        difference() {
           union() {
                Ubranch();
                translate([-10,height-thickness,radius])
                    cube([20,thickness,thicknessSize-(radius-thickness)]);
                translate([-10,height-radius,radius]) {
		    if (!Collisions) {
                    rotate([0,90,0]) {
                        difference() {
                            cylinder(20,radius,radius);
                            translate([0,0,-1])
                                cylinder(22,radius-thickness,radius-thickness);
                            translate([-10+radius-thickness,-20,-1])
                                cube([10,20,22]);
                            translate([-15,-20+radius-thickness,-1])
                                cube([15,20.01,22]);
                        }
                    }
	            }
                }
            }
            servoArm(depth=6);
        }
    }

    module UScrews() {
        rotate([270,0,0]) {
            screws(screwsCount, screwsSpacing, screwsDiameter);
        }
    }

    module UOllo() {
        rotate([-90,armAngle,0])
            servoArm(2*height);
    }

    if (print) {
        translate([0,0,10])
            rotate([0,90,0])
            u(height,radius,thickness,screwsSpacing,screwsDiameter,screwsCount,
                    screws,olloArm,armAngle,thicknessSize,false);
    } else {
        difference() {
            translate([0,0,-thicknessSize-thickness]) {
                USide();
                mirror([0,0,1]) translate([0,0,-2*thickness-2*thicknessSize])  
                    USide();
            }
            if (screws) {
                UScrews();
            }
            if (olloArm) {
                UOllo();
            }
        }
    }
}

//u(olloArm=true, screws=false, armAngle=0, print=true);
u(radius=4);