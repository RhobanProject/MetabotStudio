// Component Motor
// Description A motor
use <../../metabot.scad>;
use <../models/motor_arm.scad>;

// Parameter Counter arm?
CounterArm=true;

metabot_anchor("motor", male=true);
motorArm(CounterArm);