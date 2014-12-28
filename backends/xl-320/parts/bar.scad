use <../util/screws.scad>;

//:Part
module bar(height=60, barRadius=3, bottomRadius=10, thickness=2.2, 
        screwsCount=8, screwsSpacing=14, screwsDiameter=2.6) {

    union() {
        difference() {
            cylinder(r=bottomRadius, h=thickness);
            screws(screwsCount, screwsSpacing, screwsDiameter);
        }
        cylinder(r=barRadius,h=(height+thickness));
    }
}

bar();
