Collisions=false;

// Diameter of plastic ollo holes
OlloHoles = 4.3;

// Width of Ollo "real" plastic
OlloWidth = 3;

// Distance between two ollo holes
OlloSpacing = 6;

module rivet()
{
    //:BOM "Ollo rivet" 0.05 http://www.robotis-shop-en.com/?act=shop_en.goods_view&GS=1605
}

// Do an Ollo hole on the part
module olloHole(depth=2.2, rivet=true)
{
    if (!Collisions) {
        if (rivet) {
	    rivet();
        }
        translate([0,0,-0.1])
            cylinder(depth+0.2, OlloHoles/2, OlloHoles/2);
    }	    
}

// Do 3 ollo holes
module threeOllo(depth=2.2, rivets=true)
{
    for (xy=[[-OlloSpacing,0],[0,0],[OlloSpacing,0]]) {
        translate([xy[0], xy[1], 0])
            olloHole(depth, rivets);
    }
}

// Do a servo arm holes
module servoArm(depth=2.2, rivets=true)
{
    if (rivets) {
        for (i=[1:5]) {
	    rivet();
	}
    }
    threeOllo(depth, false);
    rotate([0,0,90])
        threeOllo(depth, false);
}
