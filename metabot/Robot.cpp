#include <map>
#include <vector>
#include <sstream>
#include <iostream>
#include <json/json.h>
#include "Backend.h"
#include "Robot.h"
#include "AnchorPoint.h"
#include "ComponentInstance.h"
#include "Part.h"
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

        std::map<std::string, std::vector<Part*> > allParts;
        foreach([&allParts](ComponentInstance *instance) {
            for (auto part : instance->parts) {
                allParts[part->hash()].push_back(part);
            }
        });
        std::map<std::string, std::vector<Part*> > parts;
        for (auto sameParts : allParts) {
            Part *part = sameParts.second.front();
            part->quantity = sameParts.second.size();

            parts[part->name].push_back(part);
        }
        for (auto part : parts) {
            std::cout << part.first << ": " << std::endl;
            for (auto spart : part.second) {
                std::cout << spart->params << " quantity:"<< spart->quantity << std::endl;
            }
        }
    }
            
    Robot *Robot::clone()
    {
        Robot *robot = new Robot(backend);
        if (root) {
            robot->root = root->clone();
        }

        return robot;
    }
            
    void Robot::setRoot(ComponentInstance *root_)
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
        if (root != NULL) {
            root->unHighlight();
        }
    }
    
    void Robot::foreach(std::function<void(ComponentInstance *instance)> method)
    {
        if (root != NULL) {
            root->foreach(method);
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
            
    void Robot::highlightNth(int nth)
    {
        unHighlight();
        foreachAnchor([&nth](AnchorPoint *anchor) {
            if (nth == 0) {
                anchor->highlight = true;
            }
            nth--;
        });
    }

    void Robot::number()
    {
        int id = 1;
        foreach([&id](ComponentInstance *component) {
            component->id = (id++);
        });
    }
            
    Vector Robot::getPoint(ComponentInstance *instance, Vector v)
    {
        TransformMatrix m = TransformMatrix::identity();

        AnchorPoint *anchor = NULL;
        while ((anchor = instance->belowAnchor()) != NULL) {
            m = m.multiply(anchor->transformationForward());
            m = m.multiply(anchor->anchor->transformationBackward());
            instance = anchor->anchor->instance;
        }

        m = m.invert();
        Vector p = m.apply(v);
        return p;
    }
            
    ComponentInstance *Robot::nearest(Vector pt)
    {
        ComponentInstance *bestInstance = NULL;
        float bestDistance = -1;

        foreach([this, pt, &bestInstance, &bestDistance](ComponentInstance *instance) {
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
            
    ComponentInstance *Robot::getComponentById(int id)
    {
        ComponentInstance *componentInstance = NULL;
        foreach([id, &componentInstance](ComponentInstance *instance) {
            if (instance->id == id) componentInstance = instance;
        });

        return componentInstance;
    }
}
