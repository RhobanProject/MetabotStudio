Collisions=false;

// Do a rounded cube
module rounded(x=10, y=10, z=10, r=3, center=false) {
    if (Collisions) {
        translate([0,0,center?(z/2):0])
            cube([x,y,z], center=center);
    } else {
        if (center) {
            translate([-(x/2),-(y/2),0])
                rounded(x, y, z, r);
        } else {
            translate([r,r,0])
                minkowski() {
                    cube([x-2*r,y-2*r,z/2]);
                    cylinder(h=z/2, r=r);
                }
        }
    }
}
