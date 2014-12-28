include <ollo.scad>;

//:Model 
//:BOM "Ollo pulley" 0.2 http://www.robotis-shop-en.com/?act=shop_en.goods_view&GS=1590
module arm(pulley=false) {
    c = pulley?[0.6,0.6,0.6]:[0.3,0.3,0.3];
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
