use <../models/ollo.scad>;

//:Part
module metabotv2_body_plate(thickness=2.2, type="top", print=false) {
    // The layout path
    X=45;
    Y=15;

    module octopath(X, Y, thickness, center=false)
    {
        union() {
            linear_extrude(thickness, center=center)
                polygon(points=[
                        [X, Y], [X, -Y], 
                        [Y, -X], [-Y, -X],
                        [-X, -Y], [-X, Y],
                        [-Y, X], [Y, X], 
                ]);
        }
    }

    module hole3()
    {
        cylinder(d=3.5, h=99, center=true);
    }

    module repeat4()
    {
        for (r=[0:3])
        rotate([0,0,90*r])
        children();
    }

    difference() {
        // Making the exernal path
        octopath(X, Y, thickness=thickness);

        if (type == "top") {
            // Craving the board to make it accessible
            octopath(X-8, Y-8, thickness=99, center=true);

            // Adding holes for the magnets
            repeat4()
                translate([41,0,0])
                hole3();
        } else if (type == "bottom") {
            // Adding holes for the XBoard
            repeat4()
                translate([35, 11.5, 0])
                hole3();
        }

        // Motors fixation
        repeat4()
            rotate([0,0,45])
            translate([38,0,0])
            rotate([0,0,90])
            threeOllo(thickness);
    }

    if (type == "bottom") {
        // GP-2 fixation
        translate([X,0,5.5])
            difference() {
                cube([thickness, 44, 11], center=true);

                translate([0,-37/2,2])
                    rotate([0,90,0])
                    hole3();

                translate([0,37/2,2])
                    rotate([0,90,0])
                    hole3();

            }
    }
}

metabotv2_body_plate(type="top");
