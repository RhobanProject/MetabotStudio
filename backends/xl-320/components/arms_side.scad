// Component Arms to side
// Description Motor arms to another sides
use <../../metabot.scad>;
use <../models/motor_arm.scad>
use <../parts/arm_side.scad>;

// Parameter First part
FirstPart = 17;
// Parameter Motors distance
Distance = 45;
// Parameter Thickness
Thickness = 2.2;

// motorArm();
metabot_anchor("motor", female=true);

arm_side(firstPart=FirstPart , distance=Distance, thickness=Thickness);
rotate([0,180,0]) {
  arm_side(firstPart=FirstPart , distance=Distance, thickness=Thickness);
}

translate([0,Distance,0]) {
  rotate([0,90,0]) {
    motorArm();
    metabot_anchor("motor", male=true);
  }
}
