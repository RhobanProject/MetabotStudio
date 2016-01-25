use <../parts/u.scad>;
use <../parts/bar.scad>;

$fn=25;

//:Component U-Bar
//:Description "An U with a long bar"
//:Parameter BarLength "Bar length"
//:Parameter BarRadius "Bar radius"
//:Parameter BarBottomRadius "Bar bottom radius"
//:Parameter UHeight "U height"
//:Parameter Thickness "Parts thickness"
//:Parameter ScrewsSpacing "Screws spacing"
//:Parameter ScrewsDiameter "Screws diameter"
//:Parameter ScrewsCount "Number of screws"
module u_bar(BarLength=60, BarRadius=3, BarBottomRadius=10, UHeight=25,
        Thickness=2.3, ScrewsSpacing=14, ScrewsDiameter=2.6, ScrewsCount=8)
{
    //:Anchor motor female
    u(height=UHeight, thickness=Thickness,
            screwsSpacing=ScrewsSpacing, screwsDiameter=ScrewsDiameter,
            screwsCount=ScrewsCount, thickness=Thickness);
    translate([0, UHeight, 0]) {
        rotate([-90,0,0]) {
            bar(
                    height=BarLength, barRadius=BarRadius, bottomRadius=BarBottomRadius,
                    screwsSpacing=ScrewsSpacing, screwsDiameter=ScrewsDiameter,
                    screwsCount=ScrewsCount, thickness=Thickness
               );
            translate([0,0,Thickness+BarLength])
                sphere(r=BarRadius);
        }
    }
}