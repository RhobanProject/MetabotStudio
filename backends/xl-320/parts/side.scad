include <../../metabot.scad>;
include <../models/ollo.scad>;
use <../util/rounded.scad>;

module side(size=65, holesToBorder=5, thickness=2.2, print=false) {
    metabot_part("side");
    metabot_parameter("size", size);
    metabot_parameter("holesToBorder", holesToBorder);
    metabot_parameter("thickness", thickness);
    metabot_parameter("print", true);

    if (print) {
        translate([0,-size+holesToBorder,0])
            side(size,holesToBorder,thickness,false);
    } else {
    difference() {
	translate([0,size/2,0])
	rounded(20, size+2*holesToBorder, thickness, 5, center=true);
	for (y=[0, 3*OlloSpacing, size, size-3*OlloSpacing]) {
	    translate([0, y, 0])
		threeOllo();
	}
    }
    }
}

side();
