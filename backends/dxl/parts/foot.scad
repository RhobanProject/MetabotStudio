Collisions=false;

//:Part
module foot(width=142, height=82, thickness=2, cleats=12)
{
    cube([width, height, thickness], center=true);
    
    factor=1;
    for (x=[-factor*width/2+5, -5+factor*width/2]) {
        for (y=[-factor*height/2+5, -5+factor*height/2]) {
            translate([x,y,-cleats/2]) {
                cylinder(r=6, h=cleats, center=true);
                translate([0,0,-cleats/2])
                sphere(r=9);
            }
        }
    }
}

foot();