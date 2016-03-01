Collisions=false;
include <ollo.scad>;
include <motor_values.scad>;
use <../util/rounded.scad>;

//:Model "Motor" [0.15,0.2,0.15]
//:Mass 16.7
module motor(thickness=2.2) {
    //:BOM "XL-320 Servo" 21.9 http://www.robotis-shop-en.com/?act=shop_en.goods_view&GS=1611
    translate([0,-MotorArmOffset,-MotorDepth/2]) {
        color([0.25,0.25,0.25]) {
            difference() {
                rounded(MotorWidth,MotorHeight,MotorDepth,3,center=true);

		if (!Collisions) {
			// Bottom
			translate([-MotorWidth/2-0.1,-MotorHeight/2-0.1,3])
			    cube([MotorWidth+0.2, 5.7, MotorDepth-6]);

			translate([0,-MotorHeight/2+3])
			    threeOllo(MotorDepth, false);

			// Motor shaft
			translate([0, MotorArmOffset])
			    olloHole(MotorDepth+5, false);

			// Side holes
			for (side=[-MotorWidth/2,MotorWidth/2-thickness]) {
			    translate([side,9,MotorDepth/2]) {
				rotate([0,90,0]) {
				    threeOllo(thickness, false);
				    translate([0,-3*OlloSpacing,0])
					threeOllo(thickness, false);
				}
			    }
			}
		}
            }
        }
    }
}

motor();
