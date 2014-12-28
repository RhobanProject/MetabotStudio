NoModels = false;
include <../models/ollo.metabot.scad>;
use <../util/rounded.metabot.scad>;

module side(size=65, holesToBorder=5, thickness=2.2, print=false) {
// metabot: Begining module side
marker("metabot_part: side");
marker("metabot_parameter: holesToBorder ", holesToBorder);
marker("metabot_parameter: print ", print);
marker("metabot_parameter: size ", size);
marker("metabot_parameter: thickness ", thickness);
if (NoModels == false) {
_side(holesToBorder=holesToBorder,print=print,size=size,thickness=thickness);
}
}
module _side(holesToBorder=5, print=false, size=65, thickness=2.2) {
if (print) {
translate([0,-size+holesToBorder,0])
side(size,holesToBorder,thickness,false);
} else {
difference() {
translate([0,size/2,0])
rounded(20, size+2*holesToBorder, thickness, 5, center=true);
for (y=[0, 3*OlloSpacing, size, size-3*OlloSpacing]) {
translate([0, y, 0])
threeOllo();
}
}
}
}
// metabot: Ending module side

side();
