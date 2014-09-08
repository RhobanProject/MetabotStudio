
// Tells metabot that a model should be rendered at this point, with its
// name as parameter
module metabot_model(name) {
    marker("metabot_model ", name);
}

// Tell that a part is needed for this component to be created
// or 3D-printed
module metabot_part(name) {
    marker("metabot_part", name);
}

// Used to put an anchor point in the file
module metabot_anchor(name) {
    marker("metabot_anchor", name);
}
