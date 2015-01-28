use <../models/motor_arm.scad>;

//:Component Motor
//:Description "A motor"
//:Parameter CounterArm "Counter arm?"
module motor(CounterArm=true)
{
    //:Anchor motor male
    motorArm(CounterArm);
}
