NoModels = false;
use <../util/screws.metabot.scad>;

module bar(height=60, barRadius=3, bottomRadius=10, thickness=2.2,
screwsCount=8, screwsSpacing=14, screwsDiameter=2.6) {
// metabot: Begining module bar
marker("metabot_part: bar");
marker("metabot_parameter: barRadius ", barRadius);
marker("metabot_parameter: bottomRadius ", bottomRadius);
marker("metabot_parameter: height ", height);
marker("metabot_parameter: screwsCount ", screwsCount);
marker("metabot_parameter: screwsDiameter ", screwsDiameter);
marker("metabot_parameter: screwsSpacing ", screwsSpacing);
marker("metabot_parameter: thickness ", thickness);
if (NoModels == false) {
_bar(barRadius=barRadius,bottomRadius=bottomRadius,height=height,screwsCount=screwsCount,screwsDiameter=screwsDiameter,screwsSpacing=screwsSpacing,thickness=thickness);
}
}
module _bar(barRadius=3, bottomRadius=10, height=60, screwsCount=8, screwsDiameter=2.6, screwsSpacing=14, thickness=2.2) {

union() {
difference() {
cylinder(r=bottomRadius, h=thickness);
screws(screwsCount, screwsSpacing, screwsDiameter);
}
cylinder(r=barRadius,h=(height+thickness));
}
}
// metabot: Ending module bar

bar();
