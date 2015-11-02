Collisions=false;
SphereTip=false;
include <../models/motor_values.scad>;
use <../models/ollo.scad>;
use <../util/rounded.scad>;

//:Part
module leg(sizeA=60, sizeB=20, sizeC=20, sizeBottom=10,
        sizeTop=15, mode="arm", fixationAngle=0, thickness=2.2, holesPosition=15, print=false)
{
    xOffset = (mode == "side") ? -10 : 0;
    spacing = (mode == "side" || mode == "bottom") ? MotorWidth : MotorWidth+6;

    module legSide()
    {
        difference() {
            union() {
                translate([0,0,sizeA/2])
                    rotate([0,90,0]) {
                        cube([sizeA, 20, thickness], center=true);
                        if (mode == "side") {
                            translate([15-sizeA/2, 10, -thickness/2])
                                rotate([0,0,+fixationAngle])
                                rounded(25,10,thickness, center=true);
                        }
                    }
            }
            translate([-thickness, 0, sizeA-holesPosition]) {
                rotate([90,0,90]) {
                    if (mode == "arm") {
                        servoArm(2*thickness);
                    }
                }
                if (mode == "side") {
                    translate([0,10,0])
                        rotate([90,90-fixationAngle,90])
                        threeOllo(2*thickness);
                }
		if (mode == "bottom") {
                    translate([0,0,0])
                        rotate([90,fixationAngle,90])
                        threeOllo(2*thickness);
                }
            }
        }
	if (Collisions) {
		translate([0,0,sizeA+sizeTop-10]) {
			rotate([45,0,0])
			rotate([0,-90,0])
			cube([14,14,thickness],center=true);
		}
	} else {
		translate([-thickness/2,0,sizeA]) {
		    rotate([90,0,90])
			linear_extrude(thickness)
			polygon([[-10,0],[-1,sizeTop],[1,sizeTop],[10,0]]);
		}
	}
    }

    module biais() {
        Dx = (spacing-sizeBottom)/2;
        Dy = sizeC;
        Dl = sqrt(pow(Dx,2)+pow(Dy,2));

        translate([-(spacing/2)-thickness,-10,-(sizeB+thickness/2)])
            rotate([0,atan2(Dy,Dx),0])
            cube([Dl,20,thickness]);
    }

    if (print) {
        translate([0,0,10-xOffset])
            rotate([90,0,0])
            leg(sizeA, sizeB, sizeC, sizeBottom,
                    sizeTop, mode, fixationAngle, thickness, false);
    } else {
        translate([0,xOffset,-sizeA+holesPosition]) {
            cube([spacing+thickness*2, 20, thickness], center=true);

            translate([spacing/2+thickness/2,0,0])
                legSide();
            translate([-(spacing/2+thickness/2),0,0])
                legSide();

		if (!SphereTip || !Collisions) {
		    translate([0,0,-sizeB])
			cube([spacing+thickness*2, 20, thickness], center=true);
		    translate([spacing/2+thickness/2,0,-sizeB/2])
			cube([thickness,20,sizeB], center=true);
		    translate([-(spacing/2+thickness/2),0,-sizeB/2])
			cube([thickness,20,sizeB], center=true);

		    translate([0,0,-(sizeB+sizeC)])
			cube([sizeBottom+thickness*2, 20, thickness], center=true);

		    biais();
		    mirror([1,0,0])
			biais();
		} else {
		    translate([0,0,-(sizeB+sizeC+thickness)+20])
			sphere(20);  
		}
		translate([0,0,-(sizeB+sizeC+thickness)]) {
                        //:Tip
			//sphere(7);
		}
        }
    }
}

leg();
