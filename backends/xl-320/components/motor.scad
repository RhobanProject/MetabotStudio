use <../models/motor_arm.scad>;

//:Component Motor
//:Description "A motor"
//:ParameterBool CounterArm "Counter arm?"
module motor(CounterArm=true)
{
    //:Anchor motor male
    motorArm(CounterArm);
}
