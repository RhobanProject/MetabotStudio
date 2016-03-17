#include <stdexcept>
#include <random>
#include "Generator.h"
#include "util.h"

    Generator::Generator(Metabot::Backend *backend_)
: backend(backend_)
{
    auto all = backend->getModules();

    for (auto &module : all) {
        std::string name = module->getName();
        Metabot::Component *instance = backend->instanciate(name);
        instance->compile();
        size_type tipsSize = instance->tips.size();
        size_type anchorsSize = instance->anchors.size();

        if (tipsSize == 1) {
            for (auto anchor : instance->anchors) {
                tips.push_back(anchor);
            }
        } else if (anchorsSize > 2) {
            bodies.push_back(instance);
        } else if (anchorsSize > 1) {
            for (auto anchor : instance->anchors) {
                segments.push_back(anchor);
            }
        } else {
            delete instance;
        }
        components.push_back(instance);
    }
}

Generator::~Generator()
{
}

Metabot::AnchorPoint *Generator::getRandom(Metabot::AnchorPoint *parent, bool tip)
{
    auto type = parent->type;
    std::vector<Metabot::AnchorPoint *> candidates;

    for (auto anchor : (tip ? tips : segments)) {
        if (parent->isCompatible(anchor)) {
            candidates.push_back(anchor);
        }
    }
    auto anchor = randVec(candidates);

    return anchor;
}
        
Metabot::AnchorPoint *Generator::duplicate(Metabot::AnchorPoint *anchor)
{
    auto component = anchor->component->clone();

    return component->getAnchor(anchor->id);
}

std::vector<Metabot::AnchorPoint *> Generator::add(std::vector<Metabot::AnchorPoint *> anchors, bool tip)
{
    auto first = anchors[0];
    auto sanchor = getRandom(first, tip);
    std::vector<Metabot::AnchorPoint *> newAnchors;

    for (auto anchor : anchors) {
        auto newAnchor = duplicate(sanchor);
        anchor->attach(newAnchor);
        for (auto tmp : newAnchor->component->anchors) {
            if (tmp->anchor == NULL) {
                newAnchors.push_back(tmp);
            }
        }
    }

    return newAnchors;
}

Metabot::Robot *Generator::generate()
{
    Metabot::Robot *robot = new Robot(backend);

    // Choosing a body
    robot->root = bodies[uniform(0, bodies.size()-1)]->clone();

    // Getting all robot anchors
    std::vector<Metabot::AnchorPoint *> anchors;
    anchors = robot->root->anchors;

    // Adding segments
    anchors = add(anchors, false);
    anchors = add(anchors, false);

    // Adding a leg tip
    anchors = add(anchors, true);

    auto json = robot->toJson();
    Json::StyledWriter writer;
    std::cout << writer.write(json) << std::endl;

    return robot;
}
