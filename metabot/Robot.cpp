#include <map>
#include <vector>
#include <sstream>
#include <iostream>
#include <json/json.h>
#include "Backend.h"
#include "Robot.h"
#include "AnchorPoint.h"
#include "Component.h"
#include "Parts.h"
#include "util.h"

namespace Metabot
{
    Robot::Robot(Backend *backend_)
        : root(NULL), backend(backend_)
    {
    }

    Robot::~Robot()
    {
        clear();
    }
            
    void Robot::build(std::string directory)
    {
        if (!is_directory(directory)) {
            makedir(directory);
        }

        Parts parts = getParts();
    }
            
    Robot *Robot::clone()
    {
        Robot *robot = new Robot(backend);
        if (root) {
            robot->root = root->clone();
        }

        return robot;
    }
            
    void Robot::setRoot(Component *root_)
    {
        root = root_;
        root->root();
    }

    void Robot::clear()
    {
        delete root;
        root = NULL;
    }
            
    void Robot::loadFromFile(std::string filename)
    {
        if (file_exists(filename)) {
            std::string data = file_get_contents(filename);
            Json::Value json;
            Json::Reader reader;

            if (!reader.parse(data, json)) {
                std::stringstream ss;
                ss << "Unable to read the file " << filename << ": " << std::endl << reader.getFormatedErrorMessages();
                throw ss.str();
            }

            if (!json.isObject() || !json.isMember("backend") || !json["backend"].isString()) {
                std::stringstream ss;
                ss << "Malformed file " << filename << " (unable to read backend)";
                throw ss.str();
            }

            if (json["backend"].asString() != backend->name) {
                std::stringstream ss;
                ss << "Bad backend: " << json["backend"].asString();
                throw ss.str();
            }

            if (!json.isMember("tree")) {
                std::stringstream ss;
                ss << "Malformed file " << filename << ": no tree";
                throw ss.str();
            }

            root = backend->fromJson(json["tree"]);
        }

        number();
    }

    void Robot::saveToFile(std::string filename)
    {
        Json::Value json(Json::objectValue);
        json["backend"] = backend->name;
        json["tree"] = Json::Value(Json::objectValue);
        if (root != NULL) {
            json["tree"] = root->toJson();
        }

        std::string data;
        Json::StyledWriter writer;
        data = writer.write(json);
        file_put_contents(filename, data);
    }

    Model Robot::toModel()
    {
        if (root != NULL) {
            return root->toModel();
        } else {
            Model model;
            return model;
        }
    }

    void Robot::compile()
    {
        if (root != NULL) {
            root->compileAll();
        }
    }
            
#ifdef OPENGL
    void Robot::openGLDraw()
    {
        if (root != NULL) {
            root->openGLDraw();
        }
    }
#endif
            
    void Robot::unHighlight()
    {
        foreachComponent([](Component *instance) {
            instance->highlight = false;
            for (auto anchor : instance->anchors) {
                anchor->highlight = false;
            }
        });
    }

    void Robot::unHover()
    {
        foreachComponent([](Component *instance) {
            instance->hover = false;
            for (auto anchor : instance->anchors) {
                anchor->hover = false;
            }
        });
    }
            
    AnchorPoint *Robot::getHoveredAnchor(int id)
    {
        AnchorPoint *anchor = NULL;
        id -= 200;

        foreachComponent([&anchor, id](Component *instance) {
            if (instance->hover) {
                int n = 1;
                for (auto a : instance->anchors) {
                    if (n++ == id) anchor = a;
                }
            }
        });

        return anchor;
    }
    
    void Robot::foreachComponent(std::function<void(Component *instance)> method)
    {
        if (root != NULL) {
            root->foreachComponent(method);
        }
    }

    void Robot::foreachAnchor(std::function<void(AnchorPoint *anchor)> method)
    {
        if (root != NULL) {
            root->foreachAnchor(method);
        }
    }
            
    std::vector<float> Robot::getZeros()
    {
        std::vector<float> zeros;
        foreachAnchor([&zeros](AnchorPoint *anchor) {
           zeros.push_back(anchor->zero);
        });
        return zeros;
    }
    
    std::vector<AnchorPoint*> Robot::getAnchors()
    {
        std::vector<AnchorPoint*> anchors;
        foreachAnchor([&anchors](AnchorPoint *anchor) {
           anchors.push_back(anchor);
        });

        return anchors;
    }

    void Robot::setZeros(std::vector<float> zeros)
    {
        unsigned int i = 0;
        foreachAnchor([&i,&zeros](AnchorPoint *anchor) {
            if (i < zeros.size()) {
                anchor->zero = zeros[i++];
                anchor->computeMatrixes();
            }
        });
    }
            
    void Robot::number()
    {
        int id = 1;
        foreachComponent([&id](Component *component) {
            component->id = (id++);
        });
    }
            
    Vector Robot::getPoint(Component *instance, Vector v)
    {
        TransformMatrix m = TransformMatrix::identity();

        AnchorPoint *anchor = NULL;
        while ((anchor = instance->belowAnchor()) != NULL) {
            m = m.multiply(anchor->transformationForward());
            m = m.multiply(anchor->anchor->transformationBackward());
            instance = anchor->anchor->component;
        }

        m = m.invert();
        Vector p = m.apply(v);
        return p;
    }
            
    Component *Robot::nearest(Vector pt)
    {
        Component *bestInstance = NULL;
        float bestDistance = -1;

        foreachComponent([this, pt, &bestInstance, &bestDistance](Component *instance) {
            Vector v(0, 0, 0);
            auto partPoint = this->getPoint(instance, v);
            float distance = partPoint.distance(pt);
            if (bestDistance < 0 || distance<bestDistance) {
                bestInstance = instance;
                bestDistance = distance;
            }
        });

        return bestInstance;
    }
            
    Component *Robot::getComponentById(int id)
    {
        Component *componentInstance = NULL;
        foreachComponent([id, &componentInstance](Component *instance) {
            if (instance->id == id) {
                componentInstance = instance;
            }
        });

        return componentInstance;
    }
    
    Parts Robot::getParts()
    {
        Parts parts;
        foreachComponent([&parts](Component *instance) {
            parts.merge(instance->parts);
        });
        return parts;
    }

    BOM Robot::getBOM()
    {
        BOM bom;
        foreachComponent([&bom](Component *instance) {
            bom.merge(instance->bom);
        });

        return bom;
    }
}
