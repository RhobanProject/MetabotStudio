include <../../metabot.scad>;
use <../util/screws.scad>;

module bar(height=60, barRadius=3, bottomRadius=10, thickness=2.2, 
    screwsCount=8, screwsSpacing=14, screwsDiameter=2.6) {
    metabot_part("bar");
    metabot_parameter("height", height);
    metabot_parameter("barRadius", barRadius);
    metabot_parameter("bottomRadius", bottomRadius);
    metabot_parameter("thickness", thickness);
    metabot_parameter("screwsCount", screwsCount);
    metabot_parameter("screwsSpacing", screwsSpacing);
    metabot_parameter("screwsDiameter", screwsDiameter);

    union() {
        difference() {
            cylinder(r=bottomRadius, h=thickness);
        screws(screwsCount, screwsSpacing, screwsDiameter);
        }
        cylinder(r=barRadius,h=(height+thickness));
    }
}

bar();
