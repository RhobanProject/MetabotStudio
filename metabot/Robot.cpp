#include <sstream>
#include <iostream>
#include <json/json.h>
#include "Backend.h"
#include "Robot.h"
#include "ComponentInstance.h"
#include "util.h"

namespace Metabot
{
    Robot::Robot(Backend *backend_)
        : root(NULL), backend(backend_)
    {
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
            
    void Robot::openGLDraw()
    {
        if (root != NULL) {
            root->openGLDraw();
        }
    }
            
#ifdef OPENGL
    void Robot::unHighlight()
    {
        if (root != NULL) {
            root->unHighlight();
        }
    }
#endif
}
