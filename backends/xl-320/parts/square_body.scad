include <../../metabot.scad>;
include <../models/ollo.scad>;
use <../util/rounded.scad>;

module square_body(width=40, length=70, legs=4, legsSpacing=40, thickness=2.2) {

    metabot_part("square_body");
    metabot_parameter("width", width);
    metabot_parameter("length", length);
    metabot_parameter("legs", legs);
    metabot_parameter("legsSpacing", legsSpacing);
    metabot_parameter("thickness", thickness);
	
    legsDistance = ((legs/2)-1)*legsSpacing;
    difference() {
	rounded(width, length, thickness, center=true);
	for (k=[1:legs/2]) {
	  for (side=[1,-1]) {
		translate([side*(width/2-OlloWidth),
		  -legsDistance/2+(k-1)*legsSpacing,0])
		rotate([0,0,90])
		threeOllo();
	  }
	}
    }
}

square_body(size=30);
