#pragma once

#include <vector>
#include <string>
#include <Backend.h>
#include <Robot.h>
#include <AnchorPoint.h>

class Generator
{
    public:
        Generator(Metabot::Backend *backend);
        Metabot::Robot *generate();

        std::vector<Metabot::AnchorPoint *> add(std::vector<Metabot::AnchorPoint *> anchors, bool tip);
        Metabot::AnchorPoint *duplicate(Metabot::AnchorPoint *anchor);
        Metabot::AnchorPoint *getRandom(Metabot::AnchorPoint *parent, bool tip);

    protected:
        Metabot::Backend *backend;
        std::vector<Metabot::Component *> bodies;
        std::vector<Metabot::AnchorPoint *> segments;
        std::vector<Metabot::AnchorPoint *> tips;
};
