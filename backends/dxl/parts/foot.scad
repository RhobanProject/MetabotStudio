Collisions=false;

//:Part
module foot(width=142, height=82, thickness=2)
{
    cube([width, height, thickness], center=true);
    
    factor=1;
    for (x=[-factor*width/2+5, -5+factor*width/2]) {
        for (y=[-factor*height/2+5, -5+factor*height/2]) {
            translate([x,y,-5])
            sphere(r=13);
            //sphere(r=10);
        }
    }
}

foot();