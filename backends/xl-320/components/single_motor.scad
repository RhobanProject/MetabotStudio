use <../models/motor_arm.scad>;

//:Component "Single motor"
//:Description "A motor"
//:Parameter CounterArm "Counter arm?"
module single_motor(CounterArm=true)
{
    //:Anchor motor male
    motorArm(CounterArm);
}
