
//:Model eye_outside [1,1,1]
module eye_outside()
{
    color([255,255,255])
    cylinder(r=8, h=2);
}

//:Model eye_inside [0,0,0]
module eye_inside()
{
    color([0,0,0])
    cylinder(r=3, h=3);
}

module eye()
{
 eye_outside();
 eye_inside();   
}

eye();