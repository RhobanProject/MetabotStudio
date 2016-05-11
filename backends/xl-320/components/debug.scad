use <../models/eye.scad>;

//:Component Debug
module debug()
{
    rotate([0,29,0])
    cylinder(r=10, h=500, center=true);
}

debug();