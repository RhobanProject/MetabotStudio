#include <map>
#include <vector>
#include <sstream>
#include <iostream>
#include <json/json.h>
#include <symbolicc++.h>
#include <3d/stl.h>
#include "Backend.h"
#include "Robot.h"
#include "AnchorPoint.h"
#include "Component.h"
#include "Ref.h"
#include "util.h"

namespace Metabot
{
    Robot::Robot(Backend *backend_)
        : root(NULL), backend(backend_), drawCollisions(false), drawCOM(false), numTips(0)
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

        auto parts = getParts();
    }
            
    RefsGrouped Robot::writeSTLs(std::string directory)
    {
        auto groups = getParts().group();
        std::stringstream ss;
        ss << "# Parts to print for the robot" << std::endl;
        ss << "# Generated by metabot" << std::endl;
        for (auto &entry : groups) {
            auto &ref = entry.second.front();
            ss << entry.first << ".stl " << entry.second.size() << std::endl;
            auto model = ref.doCompile(backend, 0, true);
            saveModelToFileBinary(std::string(directory+"/"+entry.first+".stl").c_str(), &model);
        }
        file_put_contents(directory+"/plater.conf", ss.str());

        return groups;
    }
            
    int Robot::tips()
    {
        if (root == NULL) {
            return numTips;
        } else {
            int n = 0;

            foreachComponent([&n](Component *instance, TransformMatrix m) {
                n += instance->tips.size();
            });

            return n;
        }
    }

    Kinematic Robot::computeKinematic()
    {
        Kinematic kinematic;
        Symbolic identity = TransformMatrix::identity().toSymbolic();
        if (root != NULL) {
            root->computeKinematic(kinematic, identity);
        }

        return kinematic;
    }

    void Robot::computeDynamics()
    {
        std::map<std::string, Dynamics> analysis;
        
        foreachComponent([this, &analysis](Component *instance, TransformMatrix m) {
            for (auto ref : instance->refs()) {
                if (analysis.count(ref->hash())) {
                    ref->setDynamics(analysis[ref->hash()]);
                } else {
                    ref->analyze(backend);
                    analysis[ref->hash()] = ref->getDynamics();
                }
            }

            instance->computeDynamics();
        });
    }
        
    Dynamics Robot::getDynamics()
    {
        Dynamics global;
        root->walkDynamics(global, TransformMatrix::identity(), true);

        return global;
    }
            
    void Robot::printDynamics()
    {
        std::cout << "* Analyzing parts" << std::endl << std::endl;
        computeDynamics();

        std::cout << "* Combining components" << std::endl << std::endl;
        Dynamics global;
        root->walkDynamics(global, TransformMatrix::identity(), true);

        std::cout << std::endl;
        std::cout << "* Global:" << std::endl;
        std::cout << global.toString();
        std::cout << std::endl;
    }

    void Robot::writeJS(std::string directory)
    {
        if (directory!="") {
            directory += "/";
        }
        if (!is_directory(directory)) {
            makedir(directory);
        }
 
        foreachComponent([directory, this](Component *instance, TransformMatrix m) {
            for (auto ref : instance->refs()) {
                ref->parameters.set("$fn", "7");
                ref->compile(this->backend, DEFINE_JS);
                auto model = ref->getModel();
                model.scale(1/1000.0);
                std::string fn;
                fn = directory+ref->hash()+".stl";
                saveModelToFileBinary(fn.c_str(), &model);
            }
        });

        std::stringstream ss;
        ss << "function metabot_make_robot() {" << std::endl;
        ss << "var joints=[];" << std::endl;
        if (root != NULL) {
            root->writeJS(ss);
        }
        ss << "return {'root': link_root, 'joints': joints};" << std::endl;
        ss << "}" << std::endl;
        std::string filename = directory+"robot.js";
        file_put_contents(filename, ss.str());
    }
    
    void Robot::writeURDF(std::string directory)
    {
        if (directory!="") {
            directory += "/";
        }
        directory += "urdf/";
        if (!is_directory(directory)) {
            makedir(directory);
        }

        computeDynamics();
        
        foreachComponent([directory](Component *instance, TransformMatrix m) {
            for (auto ref : instance->refs()) {
                auto model = ref->getModel();
                model.scale(1/1000.0);
                std::string fn;
                fn = directory+ref->hash()+".stl";
                saveModelToFileBinary(fn.c_str(), &model);
            }
        });

        std::stringstream ss;
        ss << "<robot name=\"metabot\">" << std::endl;
        if (root != NULL) {
            root->writeURDF(ss);
        }
        ss << "</robot>" << std::endl;
        std::string filename = directory+"robot.urdf";
        file_put_contents(filename, ss.str());
    }

    void Robot::writeSDF(std::string directory)
    {
        if (directory != "") {
            directory += "/";
        }
        if (!is_directory(directory)) {
            makedir(directory);
        }

        computeDynamics();
        
        foreachComponent([directory](Component *instance, TransformMatrix m) {
            for (auto ref : instance->refs()) {
                auto model = ref->getModel();
                model.scale(1/1000.0);
                std::string fn;
                fn = directory+ref->hash()+".stl";
                saveModelToFileBinary(fn.c_str(), &model);
            }
        });

        std::stringstream ss;
        ss << "<?xml version=\"1.0\" ?>" << std::endl;
        ss << "<sdf version=\"1.5\">" << std::endl;
        ss << "<model name=\"metabot\">" << std::endl;
        ss << "<self_collide>true</self_collide>" << std::endl;
        if (root != NULL) {
            root->writeSDF(ss);
        }
        ss << "</model>" << std::endl;
        ss << "</sdf>" << std::endl;
        std::string filename = directory+"robot.sdf";
        file_put_contents(filename, ss.str());
    }
            
    Robot *Robot::clone()
    {
        Robot *robot = new Robot(backend);
        if (root) {
            robot->root = root->clone();
        }
        robot->parameters = parameters;

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
            
    void Robot::loadFromFile(std::string filename, Parameters defines)
    {
        if (file_exists(filename)) {
            std::string data = file_get_contents(filename);
            Json::Value json;
            Json::Reader reader;

            if (!reader.parse(data, json)) {
                std::stringstream ss;
                ss << "Unable to read the file " << filename << ": " << std::endl << reader.getFormattedErrorMessages();
                throw ss.str();
            }

            fromJson(json, defines);
        } else {
            std::stringstream ss;
            ss << "Unable to read file " << filename;
            throw ss.str();
        }
    }
    
    void Robot::loadParametersFromFile(std::string filename, Parameters defines)
    {
        if (file_exists(filename)) {
            std::string data = file_get_contents(filename);
            Json::Value json;
            Json::Reader reader;

            if (!reader.parse(data, json)) {
                std::stringstream ss;
                ss << "Unable to read the file " << filename << ": " << std::endl << reader.getFormattedErrorMessages();
                throw ss.str();
            }

            parametersFromJson(json, defines);
        }
    }

    void Robot::parametersFromJson(Json::Value json, Parameters defines)
    {
        if (json.isMember("parameters")) {
            parameters = Parameters::fromJson(json["parameters"]);
            parameters.merge(defines);
        }
        if (json.isMember("tips")) {
            numTips = json["tips"].asInt();
        }
    }

    void Robot::fromJson(Json::Value json, Parameters defines)
    {
        if (!json.isObject() || !json.isMember("backend") || !json["backend"].isString()) {
            std::stringstream ss;
            ss << "Malformed (backend not specified)";
            throw ss.str();
        }

        backend = Backend::get(json["backend"].asString());

        parametersFromJson(json, defines);

        if (!json.isMember("tree")) {
            std::stringstream ss;
            ss << "Malformed json: no tree";
            throw ss.str();
        }

        root = backend->fromJson(json["tree"], this);
        number();
    }
    
    Json::Value Robot::stateToJson()
    {
        Json::Value json(Json::objectValue);
        json["components"] = Json::Value(Json::objectValue);

        foreachComponent([&json](Component *instance, TransformMatrix m) {
            std::stringstream ss;
            ss << instance->id;
            json["components"][ss.str()] = instance->getState().toJson();
        });

        json["collisions"] = Json::Value(Json::arrayValue);
        auto points = world.getGroundCollisions();
        for (auto point : points) {
            Json::Value collision(Json::objectValue);
            collision["point"] = point.first.toJson();
            collision["normal"] = point.second.toJson();

            json["collisions"].append(collision);
        }
        
        return json;
    }

    void Robot::stateFromJson(Json::Value json)
    {
        try {
            foreachComponent([&json](Component *instance, TransformMatrix m) {
                std::stringstream ss;
                ss << instance->id;
                std::string id = ss.str();
                if (json["components"].isMember(id)) {
                    instance->setState(TransformMatrix::fromJSON(json["components"][id]));
                }
            });

            collisionPoints.clear();
            for (unsigned int k=0; k<json["collisions"].size(); k++) {
                auto point = Vect::fromJson(json["collisions"][k]["point"]);
                auto normal = Vect::fromJson(json["collisions"][k]["normal"]);
                collisionPoints.push_back(std::pair<Vect, Vect>(point, normal));
            }
        } catch (std::string err) {
            std::cout << "Robot::stateFromJson error: " << err << std::endl;
        }
    }
            
    Vect Robot::getCollisionsCOP()
    {
        double total = 0;
        Vect cop(0, 0, 0);
        for (auto point : collisionPoints) {
            double w = point.second.norm();
            total += w;
            cop.values[0] += point.first.x()*w;
            cop.values[1] += point.first.y()*w;
            cop.values[2] += point.first.z()*w;
        }

        return cop.multiply(1/total);
    }

    Json::Value Robot::toJson()
    {
        Json::Value json(Json::objectValue);
        json["backend"] = backend->name;
        json["tips"] = tips();
        json["parameters"] = parameters.toJson();
        json["tree"] = Json::Value(Json::objectValue);
        if (root != NULL) {
            json["tree"] = root->toJson();
        }

        return json;
    }

    void Robot::saveToFile(std::string filename)
    {
        auto json = toJson();
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
            root->update(this);
        }
        number();
    }
            
#ifdef OPENGL
    void Robot::openGLDraw(bool bullet, float alpha)
    {
        if (root != NULL) {
            root->openGLDraw(bullet, drawCollisions, alpha);

            if (drawCOM) {
                Dynamics global;
                root->walkDynamics(global);

                glPushMatrix();
                glTranslated(global.com.x(), global.com.y(), global.com.z());
                auto m = backend->getModel("com");
                m.r = 1;
                m.g = 0.1;
                m.b = 0.1;
                m.openGLDraw(alpha);
                glPopMatrix();
            }
        }
    }
#endif
            
    void Robot::unHighlight()
    {
        foreachComponent([](Component *instance, TransformMatrix m) {
            instance->highlight = false;
            for (auto anchor : instance->anchors) {
                anchor->highlight = false;
            }
        });
    }

    void Robot::unHover()
    {
        foreachComponent([](Component *instance, TransformMatrix m) {
            instance->hover = false;
            for (auto anchor : instance->anchors) {
                anchor->hover = false;
            }
        });
    }
            
    btVector3 Robot::getBulletCOM()
    {
        btVector3 com(0, 0, 0);
        double total = 0;

        foreachComponent([&com, &total](Component *instance, TransformMatrix m) {
            if (instance->body) {
                com += instance->body->getCenterOfMassPosition()*instance->mass;
                total += instance->mass;
            }
        });

        if (total > 0) {
            return com/total;
        } else {
            return btVector3(0, 0, 0);
        }
    }
            
    std::string Robot::getValue(std::string name)
    {
        return parameters.get(name);
    }
            
    AnchorPoint *Robot::getHoveredAnchor(int id)
    {
        AnchorPoint *anchor = NULL;
        id -= 200;

        foreachComponent([&anchor, id](Component *instance, TransformMatrix m) {
            if (instance->hover) {
                int n = 1;
                for (auto a : instance->anchors) {
                    if (n++ == id) anchor = a;
                }
            }
        });

        return anchor;
    }
    
    void Robot::foreachComponent(std::function<void(Component *instance, TransformMatrix m)> method)
    {
        if (root != NULL) {
            root->foreachComponent(method, TransformMatrix::identity());
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
                anchor->setZero(zeros[i++]);
            }
        });
    }
            
    void Robot::number()
    {
        int id = 1;
        foreachComponent([&id](Component *component, TransformMatrix m) {
            component->id = (id++);
        });
    }
            
    Vect Robot::getPoint(Component *instance, Vect v)
    {
        TransformMatrix m = TransformMatrix::identity();

        AnchorPoint *anchor = NULL;
        while ((anchor = instance->belowAnchor()) != NULL) {
            m = m.multiply(anchor->transformationForward());
            m = m.multiply(anchor->anchor->transformationBackward());
            instance = anchor->anchor->component;
        }

        m = m.invert();
        Vect p = m.apply(v);
        return p;
    }
            
    Component *Robot::nearest(Vect pt)
    {
        Component *bestInstance = NULL;
        float bestDistance = -1;

        foreachComponent([this, pt, &bestInstance, &bestDistance](Component *instance, TransformMatrix m) {
            Vect v(0, 0, 0);
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
        foreachComponent([id, &componentInstance](Component *instance, TransformMatrix m) {
            if (instance->id == id) {
                componentInstance = instance;
            }
        });

        return componentInstance;
    }
    
    Refs Robot::getParts()
    {
        Refs parts;
        foreachComponent([&parts](Component *instance, TransformMatrix m) {
            parts.merge(instance->parts);
        });
        return parts;
    }

    BOM Robot::getBOM()
    {
        BOM bom;
        foreachComponent([&bom](Component *instance, TransformMatrix m) {
            bom.merge(instance->bom);
        });

        return bom;
    }
            
    void Robot::setDrawCollisions(bool draw)
    {
        drawCollisions = draw;
    }

    void Robot::setDrawCOM(bool draw)
    {
        drawCOM = draw;
    }
            
    void Robot::toBullet()
    {
        world.clear();
        if (root) {
            // Getting the bounding box
            auto model = toModel();
            auto pmin = model.min();
            auto lowerZ = pmin.z;
            world.zOffset = ((lowerZ-10)/1000.0);

            // Exporting to bullet
            root->toBullet(&world);
        }
        world.freeze();
        world.zOffset = 0;
    }
            
    TransformMatrix Robot::getState()
    {
        if (root) {
            return root->getState();
        } else {
            return TransformMatrix::identity();
        }
    }
}
