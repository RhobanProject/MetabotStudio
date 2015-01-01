include <ollo.scad>;

//:Model "Arm" [0.0,0.0,0.0]
module arm(pulley=false) {
    //:BOM "Ollo pulley" 0.2 http://www.robotis-shop-en.com/?act=shop_en.goods_view&GS=1590
    c = pulley?[0.6,0.6,0.6]:[0.2,0.2,0.2];
    color(c) {
        difference() {
            cylinder(d=20, h=OlloWidth);
            servoArm(OlloWidth);
        }
    }
}

//:Model "Pulley" [0.6,0.6,0.6]
module pulley() {
    arm(true);
}

arm();
