
module screws(screwsCount=8, screwsSpacing=14, screwsDiameter=2.6)
{
    for (screw=[1:screwsCount]) {
        rotate([0,0,screw*360/screwsCount])
            translate([screwsSpacing/2,0,0])
            cylinder(d=screwsDiameter, h=100);
    }
}
