include <../../metabot.scad>;
use <../models/ollo.scad>;

module u(height=15, radius=8, thickness=2.2, screwsSpacing=10, screwsDiameter=2.6, screws=true, olloScrew=false, thicknessSize=15, print=false) {
    metabot_part("u");
    metabot_parameter("height", height);
    metabot_parameter("radius", radius);
    metabot_parameter("thickness", thickness);
    metabot_parameter("screwsSpacing", screwsSpacing);
    metabot_parameter("screwsDiameter", screwsDiameter);
    metabot_parameter("screws", screws);
    metabot_parameter("olloScrew", olloScrew);
    metabot_parameter("thicknessSize", thicknessSize);
    metabot_parameter("print", true);

    module Ubranch() {
        union() {  
            cylinder(thickness, 10, 10);
            translate([-10,0,0])
                cube([20, height, thickness]);
        }
    }

    module USide() {
        difference() {
            union() {
                Ubranch();
                translate([-10,height+radius-thickness,radius])
                    cube([20,thickness,thicknessSize-(radius-thickness)]);
                translate([-10,height,radius]) {
                    rotate([0,90,0]) {
                        difference() {
                            cylinder(20,radius,radius);
                            translate([0,0,-1])
                                cylinder(22,radius-thickness,radius-thickness);
                            translate([-10+radius-thickness,-10,-1])
                                cube([10,10,22]);
                            translate([-10,-20+radius-thickness,-1])
                                cube([10,20.01,22]);
                        }
                    }
                }
            }
            servoArm(depth=6);
        }
    }

    module UScrews() {
        for (x=[-screwsSpacing/2,screwsSpacing/2]) {
            for (y=[-screwsSpacing/2,screwsSpacing/2]) {
                rotate([270,0,0])
                    translate([x,y,0])
                    cylinder(d=screwsDiameter, h=100);
            }
        }
    }

	module UOllo() {
		rotate(-90, [1,0,0])
			servoArm(2*height);
	}

    if (print) {
        translate([0,0,10])
            rotate([0,90,0])
            u(height,radius,thickness,screwsSpacing,screwsDiameter,
                screws,olloScrew,thicknessSize,false);
    } else {
        echo("[PART] u");
        difference() {
            translate([0,0,-thicknessSize-thickness]) {
                USide();
                mirror([0,0,1]) translate([0,0,-2*thickness-2*thicknessSize])  
                    USide();
            }
            if (screws) {
                UScrews();
            }
            if (olloScrew) {
                UOllo();
            }
        }
    }
}

u();
