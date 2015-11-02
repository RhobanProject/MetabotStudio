
use <../models/motor_arm.scad>;
use <../models/eye.scad>;
use <../stl/head.scad>;
use <../stl/fix_board.scad>;
use <../stl/charniere.scad>;
use <../stl/clapet.scad>;

//:Component Integrated body
//:Description Metabot V2 body
module integrated_body()
{
    /**
     * Now, this part is not an actual OpenSCAD design
     * but just an assembly of STLs.
     */
    for (k=[1:4]) {
        rotate([0,0,k*90]) {
        translate([62.5,0,15])
            rotate([0,0,270]) {
                motorArm();
                //:Anchor motor male
            }
        }
    }

    translate([0,0,40])
    rotate([270,0,0])
        head();

    rotate([90,00,0])
        fix_board();

    translate([0,0,40])
    rotate([-90,0,0])
        clapet();

    translate([3-20,-20,40])
    rotate([90,0,-45])
        charniere();

    translate([23-8,23,40])
    rotate([90,0,180-45])
        charniere();
    
    for (v=[9,-9]) {
        translate([-33+v,-33-v,30])
        rotate([90,0,-45]) {
            eye_outside();
            eye_inside();
        }
    }
}

integrated_body();