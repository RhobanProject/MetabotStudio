use <../../metabot.scad>;

module rivets(quantity=1)
{
    metabot_bom("Ollo rivet", quantity=quantity, price=0.05, url="www.robotis-shop-en.com/?act=shop_en.goods_view&GS=1605");
}
