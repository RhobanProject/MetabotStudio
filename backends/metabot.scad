
// Tells metabot that a model should be rendered at this point, with its
// name as parameter
module metabot_model(name, col=[0.6,0.6,0.6]) {
    marker(str("metabot_model: {'name':'", name, "','color':", col,"}"));
}

// Tell that a part is needed for this component to be created
// or 3D-printed
module metabot_part(name) {
    marker(str("metabot_part: ", name));
}

module metabot_parameter(name, value) {
    marker(str("metabot_parameter: -D", name, "=", value));
}

// Used to put an anchor point in the file
module metabot_anchor(type, male=false, female=false) {
    marker(str("metabot_anchor: {'type':'", type, "', 'male': ", male,", 'female':", female,"}"));
}
