
//:Part
module foot(width=142, height=82, thickness=2)
{
    cube([width, height, thickness], center=true);
    
    for (x=[-width/2+5, -5+width/2]) {
        for (y=[-height/2+5, -5+height/2]) {
            translate([x,y,-10])
            cylinder(r=5,h=20, center=true);
            //sphere(r=10);
        }
    }
}

foot();