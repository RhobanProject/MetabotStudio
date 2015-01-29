Collisions=false;
include <ollo.scad>;

module arm_pulley(pulley=false)
{
	c = pulley?[0.6,0.6,0.6]:[0.2,0.2,0.2];
    if (!Collisions) {
	    color(c) {
		difference() {
		    cylinder(d=20, h=OlloWidth);
		    servoArm(OlloWidth, false);
		}
	    }
    }
}

//:Model "Arm" [0.1,0.1,0.1]
module arm(pulley=false) {
    arm_pulley();
}

//:Model "Pulley" [0.6,0.6,0.6]
module pulley() {
    //:BOM "Ollo pulley" 0.2 http://www.robotis-shop-en.com/?act=shop_en.goods_view&GS=1590
    arm_pulley(true);
}

arm();
