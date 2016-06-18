Collisions = false;
include<../models/mx28-values.scad>;
include<../models/mx64-values.scad>;

//:Part
module body_block(width=130, height=180, depth=80)
{
    plateWidth=width+41;
    p1 = MX64Depth;
    p2 = height+MX64Depth/2-MX28ShaftOffset-MX28Height/2;
    
    translate([MX64Height/2-MX64ShaftOffset,-plateWidth/2,0]) {
        cube([45,plateWidth,3]);
    }
    
    translate([-40,-plateWidth/2,p1]) {
        cube([100,plateWidth,3]);
    }
    
    translate([-40,-plateWidth/2,p2]) {
        cube([65,plateWidth,3]);
    }
    
    dX = 25;
    dY = width/2+12;
    translate([-8,0,0])
    for (x=[-dX,dX]) 
        for (y=[-dY,dY])
    translate([x,y,MX64Depth]) {
        cylinder(r=8, h=p2-p1);
    }
}

body_block();