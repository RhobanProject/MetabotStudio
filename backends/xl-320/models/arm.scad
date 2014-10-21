include <ollo.scad>;
include <../../metabot.scad>;

NoModels = false;

module arm(pulley=false) {
    if (pulley) {
        metabot_bom("Ollo pulley", price=0.2, url="http://www.robotis-shop-en.com/?act=shop_en.goods_view&GS=1590");
    }

    c = pulley?[0.6,0.6,0.6]:[0.3,0.3,0.3];
    if (NoModels) {
        metabot_model("arm", col=c);
    } else {
        color(c) {
            difference() {
                cylinder(d=20, h=OlloWidth);
                servoArm(OlloWidth);
            }
        }
    }
}

module pulley() {
    arm(true);
}

arm();
