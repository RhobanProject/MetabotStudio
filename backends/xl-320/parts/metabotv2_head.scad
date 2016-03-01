use <../models/ollo.scad>;

//:Part
module metabotv2_head(height=25, thickness=2.2, print=false) {
    // The layout path
    X=48;
    Y=18;

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

    if (print) {
        translate([0,0,height])
        rotate([-180,0,0])
        head(height, thickness, false);
    } else {
        difference() {
            union() {    
                difference() {    
                    octopath(X, Y, thickness=height);
                    
                    // Craving the inside
                    translate([0,0,-0.1])
                    octopath(X-thickness, Y+3, thickness=height-thickness);
                }
        
                // Spacers
                repeat4()
                translate([41,0,0])
                difference() {
                    translate([0,0,10])
                    cylinder(d=6, h=height-10);
                    translate([0,0,-1])
                    cylinder(d=4, h=height-10);
                }
            }
            
                    
            // Holes for magnets
            repeat4()
            translate([41,0,0])
            hole3();
        }
    }
}