include <../models/motor_values.scad>;
use <../models/motor_arm.scad>;
use <../models/eye.scad>;
use <../parts/metabotv2_body_plate.scad>;
use <../parts/metabotv2_head.scad>;

//:Component "Metabot V2 body"
//:Description "Metabot V2 3D body"
module metabotv2_body()
{
    metabotv2_body_plate(thickness=3, type="bottom");
    
    translate([0,0,MotorDepth+3])
    metabotv2_body_plate(thickness=3, type="top");
    
    rotate([0,0,135])
    for (v=[9,-9]) {
        translate([-33+v,-33-v,30])
            rotate([90,0,-45]) {
            eye_outside();
            eye_inside();
        }
    }
    
    for (motor=[1:4]) {
        rotate([0,0,45+90+90*motor])
        translate([0,62,3+MotorDepth/2]) {
            motorArm();
            //:Anchor motor male
        }
    }
    
    translate([0,0,20])
    metabotv2_head();
}

metabotv2_body();