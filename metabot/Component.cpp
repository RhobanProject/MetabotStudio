#include <sstream>
#include <iostream>
#include <3d/stl.h>
#ifdef OPENGL
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#endif
#include <symbolicc++.h>
#include "Component.h"
#include "AnchorPoint.h"
#include "Backend.h"
#include "Cache.h"
#include "CSG.h"
#include "Robot.h"
#include "util.h"

namespace Metabot
{
    Component::Component(Backend *backend_, Module *module_)
        : backend(backend_), robot(NULL), module(module_), highlight(false), 
        hover(false), main(Json::Value(), TransformMatrix::identity(), DEFINE_NO_MODELS),
        body(NULL), hinge(NULL)
    {
        for (auto param : module->getParameters()) {
            values[param.second.name] = param.second.getValue();
        }
    }

    Component::~Component()
    {
        for (auto anchor : anchors) {
            if (anchor->above) {
                delete anchor;
            }
        }
    }

    Component *Component::clone()
    {
        Component *component = new Component(backend, module);
        component->values = values;

        int index = 0;
        for (auto anchor : anchors) {
            AnchorPoint *anchorPoint = anchor->clone();
            anchorPoint->component = component;
            anchorPoint->id = index++;
            component->anchors.push_back(anchorPoint);

            AnchorPoint *remote = anchor->anchor;
            if (anchor->above && remote != NULL) {
                Component *child = remote->component->clone();
                anchorPoint->attach(child->anchors[remote->id]);
            }
        }

        component->shapes = shapes;
        component->collisions = collisions;
        component->dynamics = dynamics;
        component->main = main;
        component->models = models;
        component->parts = parts;
        component->bom = bom;
        component->tips = tips;

        return component;
    }

    void Component::root()
    {
        for (auto anchor : anchors) {
            if (anchor->above == false) {
                anchor->revert();
            }
        }
    }

    void Component::computeKinematic(Kinematic &kinematic, Symbolic parent,
            Kinematic::Chain chain, AnchorPoint *above)
    {
        if (above != NULL) {
            auto alpha = kinematic.addJoint();
            chain.addMatrix(above->transformationForward());
            chain.addRotation(kinematic.alpha-1, id);
            chain.addMatrix(above->anchor->transformationBackward());
            auto myTransformation = above->symbolicTransformation(alpha);
            myTransformation *= above->anchor->transformationBackward().toSymbolic();

            std::stringstream ss;
            ss << "/*" << std::endl;
            ss << above->component->id << "_to_" << id << std::endl;
            ss << myTransformation << std::endl;
            ss << "*/" << std::endl;
            kinematic.code += ss.str();

            parent *= myTransformation;
            // std::cout << "// Sign: " << above->sign() << std::endl;
            // std::cout << "float " << ss.str() << " = 0;" << std::endl;
        }

        for (auto anchor : anchors) {
            if (anchor->above == true && anchor->anchor!=NULL) {
                anchor->anchor->component->computeKinematic(kinematic, parent, chain, anchor);
            }
        }

        for (auto tip : tips) {
            std::stringstream ss;
            ss << "/*" << std::endl;
            ss << "Tip" << std::endl;
            chain.addMatrix(tip);
            auto sym = tip.toSymbolic();
            ss << sym;
            ss << "*/" << std::endl << std::endl;
            kinematic.code += ss.str();
            Symbolic matrix = parent*sym;

            kinematic.addTip(matrix(0,3).simplify(), matrix(1,3).simplify(), matrix(2,3).simplify(), chain);
        }
    }

    Dynamics Component::getDynamics()
    {
        return dynamics;
    }

    void Component::computeDynamics()
    {
        dynamics = Dynamics();

        // std::cout << "Combining refs for " << module->getName() << std::endl;
        for (auto ref : refs()) {
            dynamics.combine(ref->getDynamics(), ref->matrix);
        }
    }

    void Component::walkDynamics(Dynamics &global, TransformMatrix matrix, bool verbose)
    {
        Dynamics my_dynamics = getDynamics();
        if (verbose) {
            std::cout << "* Combining component " << module->getName() << std::endl;
            std::cout << my_dynamics.toString() << std::endl;
        }
        global.combine(my_dynamics, matrix);

        for (auto anchor : anchors) {
            if (anchor->above && anchor->anchor) {
                auto m = matrix.multiply(anchor->transformationForward());
                m = m.multiply(anchor->anchor->transformationBackward());
                anchor->anchor->component->walkDynamics(global, m, verbose);
            }
        }
    }

    void Component::writeJS(std::stringstream &ss, std::string parent,
            TransformMatrix parentPreTransform, AnchorPoint *above)
    {
        std::stringstream linkNameSS;
        linkNameSS << "link_" << module->getName() << "_" << id;
        auto linkName = linkNameSS.str();
        if (above == NULL) {
            linkName = "link_root";
        }

        auto preTransform = TransformMatrix::identity();
        if (above != NULL) {
            preTransform = above->transformationBackward();
        }
        ss << "var " << linkName << " = new Joint();" << std::endl;
        for (auto ref : refs()) {
            unsigned int color =
                (((int)(ref->r*255))<<16)
                |(((int)(ref->g*255))<<8)
                |(((int)(ref->b*255))<<0)
                ;
            ss << linkName << ".addSTL(\"" << ref->hash() << ".stl\"," << color << ",";
            ss << preTransform.multiply(ref->matrix).toJS() << ");" << std::endl;
        }

        if (above != NULL) {
            auto jointName = linkName + "_joint";
            ss << "var " << jointName << " = new Joint();" << std::endl;
            ss << parent << ".addChild(" << jointName << ", " << parentPreTransform.multiply(above->anchor->transformationForward()).toJS() << ");" << std::endl;
            ss << jointName << ".addChild(" << linkName << ", [0,0,0], [0,0,0]);" << std::endl;
            ss << linkName << ".setSign(" << above->sign() << ");" << std::endl;
            ss << "joints.push(" << linkName << ");" << std::endl;
        } 

        for (auto anchor : anchors) {
            if (anchor->above && anchor->anchor!=NULL) {
                anchor->anchor->component->writeJS(ss, linkName, preTransform, anchor->anchor);
            }
        }
    }

    void Component::writeSDF(std::stringstream &ss, std::string parent, TransformMatrix transform, AnchorPoint *above)
    {
        Dynamics dynamics;

        ss << std::endl << "  <!-- Component " << module->getName();
        ss << "#" << id << " -->" << std::endl << std::endl;

        // Adding the component and linking it to the parent
        std::stringstream tmp;
        tmp << module->getName() << "_" << id;
        std::string name = tmp.str();
        ss << "  <link name=\"" << name << "\">" << std::endl;

        // Adding parts and models, linked to component
        int refid = 0;
        if (above != NULL) {
            //preTransform = above->transformationBackward();
            // ss << parentPreTransform.multiply(above->anchor->transformationForward()).invert().toSDF() << std::endl;
            // ss << above->anchor->transformationBackward().toSDF() << std::endl;
            //ss << "<pose>0.2 0 0 0 0 0</pose>" << std::endl;
            ss << transform.toSDF() << std::endl;
        }
        for (auto ref : refs()) {
            dynamics.combine(ref->getDynamics(), ref->matrix);
            tmp.str("");
            tmp << module->getName() << "_" << ref->name << "_" << id << "_" << (refid++);
            auto refName = tmp.str();

            ss << "    <visual name=\"" << refName << "_visual\">" << std::endl;
            ss << "      <geometry>" << std::endl;
            // XXX: This forces the file to be in an urdf/ folder
            ss << "        <mesh><uri>model://metabot/" << ref->hash() << ".stl</uri></mesh>" << std::endl;
            ss << "      </geometry>" << std::endl;
            ss << "      <material>" << std::endl;
            // ss << "            <name>" << refName << "_material</name>" << std::endl;
            ss << "            <ambient>" << ref->r/2 << " " << ref->g/2 << " " << ref->b/2 << " 1.0</ambient>" << std::endl;
            ss << "            <specular>" << ref->r/2 << " " << ref->g/2 << " " << ref->b/2 << " 1.0</specular>" << std::endl;
            ss << "            <diffuse>" << ref->r/2 << " " << ref->g/2 << " " << ref->b/2 << " 1.0</diffuse>" << std::endl;
            ss << "            <emissive>" << ref->r/2 << " " << ref->g/2 << " " << ref->b/2 << " 1.0</emissive>" << std::endl;
            ss << "      </material>" << std::endl;
            // ss << "    " << preTransform.multiply(ref->matrix).toSDF() << std::endl;
            ss << "    " << ref->matrix.toSDF() << std::endl;
            ss << "    </visual>" << std::endl;
        }

        // Adding dynamics
        ss << "  <inertial>" << std::endl;

        auto com = dynamics.com; // preTransform.apply(dynamics.com);;
        ss << "    <pose>";
        ss << (com.x()/1000) << " " << (com.y()/1000) << " " << (com.z()/1000) << " 0 0 0";
        ss << "</pose>" << std::endl;
        ss << "    <mass>" << (dynamics.mass/1000.0) << "</mass>" << std::endl;
        ss << "    <inertia>" << std::endl;
        ss << "        <ixx>" << dynamics.ixx << "</ixx>" << std::endl;
        ss << "        <ixy>" << dynamics.ixy << "</ixy>" << std::endl;
        ss << "        <ixz>" << dynamics.ixz << "</ixz>" << std::endl;
        ss << "        <iyy>" << dynamics.iyy << "</iyy>" << std::endl;
        ss << "        <iyz>" << dynamics.iyz << "</iyz>" << std::endl;
        ss << "        <izz>" << dynamics.izz << "</izz>" << std::endl;
        ss << "    </inertia>" << std::endl;
        ss << "  </inertial>" << std::endl;

        // Adding collisions
        int colid = 0;
        for (auto shape : shapes) {
            std::stringstream tmp;
            tmp << module->getName() << "_" << id << "_" << (colid++);
            auto colName = tmp.str();
            ss << "  <collision name=\"" << colName << "_col\">" << std::endl;
            // ss << shape.toSDF(preTransform) << std::endl;
            ss << shape.toSDF(TransformMatrix::identity()) << std::endl;
            ss << "  </collision>" << std::endl;
        }

        ss << "  </link>" << std::endl;


        // Linking it to the parent
        if (above != NULL) {
            ss << "  <joint name=\"joint_" << id << "\" type=\"revolute\">" << std::endl;
            ss << "    <parent>" << parent << "</parent>" << std::endl;
            ss << "    <child>" << name << "</child>" << std::endl;
            ss << above->transformationForward().toSDF() << std::endl;
            ss << "    <axis>" << std::endl;
            ss << "         <xyz>0 0 1</xyz>" << std::endl;
            ss << "          <limit>" << std::endl;
            // XXX: This limits should be configurable
            ss << "              <lower>" << -M_PI << "</lower>" << std::endl;
            ss << "              <upper>" << M_PI << "</upper>" << std::endl;
            ss << "              <effort>0.5</effort>" << std::endl;
            ss << "          </limit>" << std::endl;
            ss << "    </axis>" << std::endl;
            ss << "  </joint>" << std::endl;
        }

        // Drawing sub-components
        for (auto anchor : anchors) {
            if (anchor->above && anchor->anchor!=NULL) {
                TransformMatrix t = transform.multiply(anchor->transformationForward());
                // t = anchor->anchor->transformationBackward().multiply(t);
                t = t.multiply(anchor->anchor->transformationBackward());
                anchor->anchor->component->writeSDF(ss, name, t, anchor->anchor);
            }
        }
    }

    TransformMatrix Component::getState()
    {
        if (body != NULL) {
            btTransform trans;
            auto state = body->getMotionState();
            state->getWorldTransform(trans);
            return TransformMatrix::fromBullet(trans);
        } else {
            return TransformMatrix::identity();
        }
    }

    void Component::setState(TransformMatrix matrix)
    {
        if (body != NULL) {
            auto trans = matrix.toBullet();
            body->getMotionState()->setWorldTransform(trans);
        }
    }

    btRigidBody *Component::toBullet(World *world, AnchorPoint *above, TransformMatrix matrix)
    {
        // Creating shapes
        auto *compound = world->createCompound();
        for (auto shape : shapes) {
            btCollisionShape *colShape = NULL;
            switch (shape.type) {
                case SHAPE_BOX:
                    colShape = world->createBox(shape.a/1000.0, shape.b/1000.0, shape.c/1000.0);
                    break;
                case SHAPE_SPHERE:
                    colShape = world->createSphere(shape.r/1000.0);
                    break;
                case SHAPE_CYLINDER:
                    colShape = world->createCylinder(shape.r/1000.0, shape.h/1000.0);
                    break;
            }

            if (colShape) {
                auto matrix = shape.matrix;
                if (shape.type == SHAPE_CYLINDER) {
                    matrix = matrix.multiply(TransformMatrix::rotationX(M_PI/2));
                }
                colShape->setMargin(0.0);
                compound->addChildShape(matrix.toBullet(), colShape);
            }
        }

        // Creating rigid body
        body = world->createRigidBody(dynamics.mass/1000.0, matrix.toBullet(), compound,
                btVector3(dynamics.ixx, dynamics.iyy, dynamics.izz));

        // Child
        for (auto anchor : anchors) {
            if (anchor->above && anchor->anchor) {
                // From body frame to anchor frame
                auto toAnchor = anchor->transformationForward();
                // Rotation for cone
                auto rot = TransformMatrix::rotationZ(M_PI/2);
                // From world frame to child
                auto worldToChild = matrix.multiply(toAnchor);
                auto worldToDummy = worldToChild;
                worldToChild = worldToChild.multiply(anchor->anchor->transformationBackward());

                // Recursion
                auto child = anchor->anchor->component->toBullet(world, anchor->anchor, worldToChild);
                
                // Creating dummy body for backlash simulation
                auto empty = world->createEmpty();
                auto dummy = world->createRigidBody(0.001, worldToDummy.toBullet(), empty, 
                        btVector3(1e-5, 1e-5, 1e-5));
              
                // Creating hinge
                /*
                anchor->anchor->component->hinge = world->createHinge(body, child,
                        anchor->transformationForward().toBullet(),
                        anchor->anchor->transformationBackward().toBullet()
                        );
                */
              
                // Creating hinge
                anchor->anchor->component->hinge = world->createHinge(body, dummy,
                        anchor->transformationForward().toBullet(),
                        btTransform::getIdentity()
                        );
            
                // Creating cone
                auto cone = world->createCone(dummy, child,
                        rot.toBullet(),
                        anchor->anchor->transformationForward().multiply(rot).toBullet()
                        );
                cone->setLimit(0.03, 0.04, 0.02);
                // cone->setLimit(0.0, 0.0, 0.0);
            }
        }

        vel = 0;
        targetForce = 0;
        lastPos = 0;

        return body;
    }

    std::string Component::fullName()
    {
        std::stringstream ss;
        ss << module->getName() << " #" << id;
        return ss.str();
    }

    AnchorPoint *Component::findCompatible(AnchorPoint *anchor)
    {
        for (auto my : anchors) {
            if (my->isCompatible(anchor) && my->anchor==NULL) {
                return my;
            }
        }

        return NULL;
    }

    bool Component::isCompatible(AnchorPoint *anchor)
    {
        return findCompatible(anchor) != NULL;
    }

#ifdef OPENGL
    void Component::openGLDraw(bool bullet, bool drawCollisions, float alpha)
    {
        glPushMatrix();
        if (bullet) {
            btTransform trans;
            body->getMotionState()->getWorldTransform(trans);
            TransformMatrix::fromBullet(trans).openGLMult();
        }

        glStencilFunc(GL_ALWAYS, id, -1);

        if (drawCollisions) {
            if (highlight) {
                collisions.r = 0.4;
                collisions.g = 1.0;
                collisions.b = 0.3;
            } else {
                collisions.r = collisions.g = collisions.b = 0.6;
            }
            collisions.openGLDraw(1.0, alpha);
        }

        // Rendering models & parts
        if (!drawCollisions) {
            for (auto ref : refs()) {
                openGLDrawRef(ref, alpha);
            }
        }
        glPopMatrix();

        // Rendering sub-components
        int anchorId = 1;
        for (auto anchor : anchors) {
            glPushMatrix();
            if (anchor->above) {
                anchor->openGLDraw(anchorId, bullet, drawCollisions, alpha);
            }
            glPopMatrix();
            anchorId++;
        }
    }

    void Component::openGLDrawRef(Ref *ref, float alpha)
    {
        glPushMatrix();
        ref->matrix.openGLMult();
        auto model = ref->getModel();
        if (highlight) {
            model.r = 0.4;
            model.g = 1.0;
            model.b = 0.3;
        } else {
            model.r = ref->r;
            model.g = ref->g;
            model.b = ref->b;
        }
        model.openGLDraw(1.0, alpha);
        glPopMatrix();
    }
#endif

    Model Component::toModel()
    {
        Model model;

        // Rendering models
        for (auto ref : refs()) {
            Model m = ref->getModel();
            m.apply(ref->matrix);
            model.merge(m);
        }

        // Rendering sub-components
        for (auto anchor : anchors) {
            if (anchor->above) {
                Model component = anchor->toModel();
                model.merge(component);
            }
        }

        return model;
    }

    std::string Component::get(std::string name)
    {
        return values[name];
    }

    void Component::set(std::string name, std::string value)
    {
        values[name] = value;
    }

    void Component::update(Robot *robot)
    {
        compile(robot, true);

        for (auto anchor : anchors) {
            if (anchor->above && anchor->anchor && anchor->anchor->component != NULL) {
                anchor->anchor->component->update(robot);
            }
        }
    }

    void Component::compile(Robot *robot, bool update)
    {
        Component *old = NULL;
        if (update) {
            old = clone();
        }
        AnchorPoint *parentAnchor = NULL;
        int index = 0;
        int parentId = -1;
        for (auto anchor : anchors) {
            if (!anchor->above && anchor->anchor) {
                parentAnchor = anchor->anchor;
                parentAnchor->detach();
                parentId = index;
            }
            index++;
        }

        // Creating CSG 
        std::string csg = module->openscad("csg", parameters(robot));
        // Main reference
        main.name = module->getName();
        main.parameters = parameters(robot);
        // Parsing the CSG document
        CSG *document = CSG::parse(csg);
        anchors = document->anchors;
        parts = document->parts;
        models = document->models;
        bom = document->bom;
        tips = document->tips;
        delete document;

        // Collision CSG & STL
        std::string collisionsCsg = module->openscad("csg", parameters(robot), DEFINE_COLLISIONS);
        collisions = loadModelSTL_string(stl(robot, true));
        CSG *collisionsDocument = CSG::parse(collisionsCsg);
        shapes = collisionsDocument->shapes;
        delete collisionsDocument;

        main.compile(backend);
        for (auto &ref : parts) {
            ref.compile(backend);
        }
        for (auto &ref : models) {
            ref.compile(backend);
        }

        index = 0;
        for (auto anchor : anchors) {
            anchor->component = this;
            anchor->id = index;
            index++;
        }

        if (update) {
            moveAnchors(old);
            if (parentAnchor) {
                parentAnchor->attach(anchors[parentId]);
            }
            delete old;
        }
    }

    void Component::moveAnchors(Component *other)
    {
        // First step:
        // Trying to take the items that matches *exactly* the anchor from the
        // old component
        for (unsigned int i=0; i<anchors.size(); i++) {
            if (i < other->anchors.size()) {
                AnchorPoint *myAnchor = anchors[i];
                AnchorPoint *otherAnchor = other->anchors[i];

                if (otherAnchor->anchor && myAnchor->isCompatible(otherAnchor->anchor)) {
                    myAnchor->copyData(otherAnchor);
                    if (otherAnchor->above) {
                        myAnchor->attach(otherAnchor->anchor);
                    } else {
                        otherAnchor->anchor->attach(myAnchor);
                    }
                    otherAnchor->detach(false);
                }
            }
        }

        // Second step:
        // Trying to get the anchors 
        for (auto anchor : other->anchors) {
            AnchorPoint *remote = anchor->anchor;
            if (remote != NULL) {
                AnchorPoint *candidate = findCompatible(remote);

                if (candidate != NULL) {
                    candidate->copyData(remote);
                    candidate->attach(remote);
                    anchor->detach(false);
                }
            }
        }
    }

    void Component::detachDiffAnchors(Component *other)
    {
        std::map<AnchorPoint *, bool> otherAnchors;

        for (auto anchor : other->anchors) {
            if (anchor->anchor) {
                otherAnchors[anchor->anchor] = true;
            }
        }

        for (auto anchor : anchors) {
            if (otherAnchors.count(anchor)) {
                anchor->detach();
            }
        }
    }

    void Component::restore()
    {
        for (auto anchor : anchors) {
            if (anchor->anchor) {
                anchor->anchor->anchor = anchor;
            }
        }
    }

    void Component::detachAll()
    {
        for (auto anchor : anchors) {
            anchor->detach(false);
        }
    }

    AnchorPoint *Component::belowAnchor()
    {
        for (auto anchor : anchors) {
            if (anchor->above == false && anchor->anchor) {
                return anchor;
            }
        }

        return NULL;
    }

    AnchorPoint *Component::aboveAnchor()
    {
        AnchorPoint *below = belowAnchor();
        if (below != NULL) {
            return below->anchor;
        }

        return NULL;
    }

    std::string Component::getValue(std::string name)
    {
        if (values.count(name)) {
            return values[name];
        }

        return "";
    }

    Parameters Component::parameters(Robot *robot)
    {
        Parameters params = module->getParameters();

        for (auto entry : values) {
            auto key = entry.first;
            auto value = entry.second;
            if (value.size()>0 && value[0]=='$' && robot!=NULL) {
                params.update(key, robot->getValue(value.substr(1)));
            } else {
                params.update(key, value);
            }
        }

        return params;
    }

    std::string Component::stl(Robot *robot, bool drawCollisions)
    {
        int defines = drawCollisions ? DEFINE_COLLISIONS : DEFINE_NO_MODELS;
        return module->openscad("stl", parameters(robot), defines);
    }

    AnchorPoint *Component::getAnchor(int id)
    {
        for (auto anchor : anchors) {
            if (anchor->id == id) {
                return anchor;
            }
        }

        return NULL;
    }

    void Component::parametersFromJson(Json::Value json)
    {
        values = Values::fromJson(json);
    }

    Json::Value Component::toJson()
    {
        Json::Value json(Json::objectValue);

        json["component"] = module->getName();
        json["parameters"] = values.toJson();
        json["anchors"] = Json::Value(Json::objectValue);

        for (auto anchor : anchors) {
            if (anchor->anchor != NULL && anchor->above) {
                std::stringstream ss;
                ss << anchor->id;
                std::string id = ss.str();
                json["anchors"][id] = Json::Value(Json::objectValue);
                json["anchors"][id]["zero"] = anchor->zero;
                json["anchors"][id]["orientationX"] = anchor->orientationX;
                json["anchors"][id]["orientationY"] = anchor->orientationY;
                json["anchors"][id]["orientationZ"] = anchor->orientationZ;
                json["anchors"][id]["remote"] = anchor->anchor->id;
                json["anchors"][id]["component"] = anchor->anchor->component->toJson();
            }
        }

        return json;
    }

    void Component::foreachComponent(std::function<void(Component *component)> method)
    {
        method(this);

        for (auto anchor : anchors) {
            if (anchor->anchor != NULL && anchor->above) {
                anchor->anchor->component->foreachComponent(method);
            }
        }
    }

    void Component::foreachAnchor(std::function<void(AnchorPoint *component)> method)
    {
        for (auto anchor : anchors) {
            if (anchor->anchor != NULL && anchor->above) {
                method(anchor);
                anchor->anchor->component->foreachAnchor(method);
            }
        }
    }

    void Component::onHover()
    {
        hover = true;
        for (auto anchor : anchors) {
            anchor->hover = true;
        }
    }

    std::vector<Ref*> Component::refs()
    {
        std::vector<Ref*> refs;

        refs.push_back(&main);
        for (auto &ref : models) {
            refs.push_back(&ref);
        }
        for (auto &ref : parts) {
            refs.push_back(&ref);
        }

        return refs;
    }

    double Component::getVelocity()
    {
        if (!hinge) {
            return 0;
        }
        double result = 0;
        Vect axis;

        btVector3 vec =
            hinge->getRigidBodyA().getCenterOfMassTransform().getBasis() *
            hinge->getFrameOffsetA().getBasis().getColumn(2);
        axis = Vect::fromBullet(vec);

        result += axis.dot(Vect::fromBullet(hinge->getRigidBodyB().getAngularVelocity()));
        result -= axis.dot(Vect::fromBullet(hinge->getRigidBodyA().getAngularVelocity()));

        return -result;
    }

    double Component::setTarget(float alpha)
    {
        float maxVel = 4*M_PI;
        float maxForce = 0.5;
        auto pos = hinge->getHingeAngle();
        // vel = vel*0.9 + 0.1*getVelocity();
        vel = vel*0.8 + 0.2*(pos-lastPos)/0.001;

        float error = alpha-pos;
        float targetVel = error*35;
        if (targetVel > maxVel) targetVel = maxVel;
        if (targetVel < -maxVel) targetVel = -maxVel;

        float errorVel = targetVel-vel;
        targetForce = errorVel*0.05;
        if (targetForce > maxForce) targetForce = maxForce;
        if (targetForce < -maxForce) targetForce = -maxForce;

        btVector3 hingeAxisLocal = hinge->getAFrame().getBasis().getColumn(2); // z-axis of constraint frame
        btVector3 hingeAxisWorld = hinge->getRigidBodyA().getWorldTransform().getBasis() * hingeAxisLocal;
        btVector3 hingeTorque = targetForce * hingeAxisWorld;
// #define METHOD_MOTOR
#ifdef METHOD_MOTOR
        hinge->enableAngularMotor(true, targetVel, 0.001*maxForce);
#else
        hinge->getRigidBodyA().applyTorque(hingeTorque);
        hinge->getRigidBodyB().applyTorque(-hingeTorque);
#endif
        lastPos = pos;

        // This can be used to add some frictions
        // hinge->enableAngularMotor(true, 0.0, 0.00001);

        return fabs(targetForce*0.001);
    }
}
