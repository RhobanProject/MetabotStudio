use <../../metabot.scad>;
include <../models/motor_values.scad>;
include <../models/ollo.scad>;

module tie_box(thickness=2.2, print=false)
{
  metabot_part("tie_box");
  metabot_parameter("thickness", thickness);
  metabot_parameter("print", true);
	
  SX = OlloWidth*2+thickness;
  SY = OlloSpacing+4*OlloWidth;
  SZ = 3*OlloSpacing+3*OlloWidth;
  T = print ? [0,0,0] : [-SX/2, -SY/2, -SZ/2];
  translate(T)
  difference() {
	cube([SX, SY, SZ], center);
	translate([0, SY/2-OlloSpacing/2, SZ/2]) {
	  rotate([0,90,0])
	      threeOllo();
	}
	translate([SX-thickness, SY/2+OlloSpacing/2, SZ/2]) {
	  rotate([0,90,0])
		threeOllo();
	}
	translate([0, SY/2, thickness]) {
	  cube([thickness, (SY-2*thickness)/2, SZ-2*thickness]);
	}
	translate([SX-thickness, thickness, thickness]) {
	  cube([thickness, (SY-2*thickness)/2, SZ-2*thickness]);
	}
	
	translate([thickness, thickness, 0]) {
	  cube([SX-2*thickness, SY-2*thickness, SZ]);
	}
	 echo((SY-2*thickness)/2);
  }
}

tie_box();