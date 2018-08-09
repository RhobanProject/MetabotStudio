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
        : 
            main(Json::Value(), TransformMatrix::identity(), DEFINE_NO_MODELS),
            backend(backend_), 
            module(module_), 
            highlight(false), 
            hover(false), 
            body(NULL),
            hinge(NULL),
            posHinge(NULL)
    {
        // std::cout << "Instanciating a component, type " << module->getName() << std::endl;
        for (auto param : module->getParameters()) {
            parameters.set(param.second.name, param.second.getValue());
        }
    }

    Component::~Component()
    {
        // std::cout << "Deleting a component, type " << module->getName() << std::endl;
        for (auto anchor : anchors) {
            if (anchor->above) {
                delete anchor;
            }
        }
        if (posHinge) {
            delete posHinge;
        }
        parameters.clear();
    }

    Component *Component::clone()
    {
        Component *component = new Component(backend, module);
        component->parameters = parameters;

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

        component->name = name;
        component->tipName = tipName;
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
            Kinematic::Tip tip, AnchorPoint *above)
    {
        if (above != NULL) {
            auto alpha = kinematic.addJoint();

            // Adding transformations to the kinematic chain
            tip.addMatrix(above->transformationForward());
            tip.addRotation(alpha, id, above->minimum, above->maximum, above->sign());
            tip.addMatrix(above->anchor->transformationBackward());

            // Adding transformation to symbolic equations
            auto myTransformation = above->symbolicTransformation(alpha);
            myTransformation *= above->anchor->transformationBackward().toSymbolic();

            // Adding transformation to the comments
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
                // Recursive
                anchor->anchor->component->computeKinematic(kinematic, parent, tip, anchor);
            }
        }

        for (auto t : tips) {
            // Adding the tip transformation to the comments
            std::stringstream ss;
            ss << "/*" << std::endl;
            ss << "Tip" << std::endl;
            tip.addMatrix(t);
            auto sym = t.toSymbolic();
            ss << sym;
            ss << "*/" << std::endl << std::endl;
            kinematic.code += ss.str();

            // Computing x, y and z for this tip
            Symbolic matrix = parent*sym;
            tip.x = std::make_shared<Symbolic>(matrix(0,3).simplify());
            tip.y = std::make_shared<Symbolic>(matrix(1,3).simplify());
            tip.z = std::make_shared<Symbolic>(matrix(2,3).simplify());

            // Adding this tip
            kinematic.addTip(tip);
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

        TransformMatrix preTransform = TransformMatrix::identity();
        preTransform = TransformMatrix::rotationX(-M_PI/2);
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

    // Simplifies small numbers (for <axis>)
    static float smp(float s) {
        if (fabs(s) < 1e-3) return 0;
        else return s;
    }
    
    void Component::writeURDF(std::stringstream &ss, std::string parent, TransformMatrix parentPreTransform, 
            AnchorPoint *above)
    {
        Dynamics dynamics;

        ss << std::endl << "  <!-- Component " << module->getName();
        ss << "#" << id << " -->" << std::endl << std::endl;

        // Adding the component and linking it to the parent
        std::stringstream tmp;
        if (name != "") {
            tmp << name;
        } else {
            tmp << module->getName() << "_" << id;
        }
        std::string name = tmp.str();
        ss << "  <link name=\"" << name << "\">" << std::endl;

        // Adding parts and models, linked to component
        int refid = 0;
        auto preTransform = TransformMatrix::identity();

        auto jointFrame = TransformMatrix::identity();
        auto jointBasis = TransformMatrix::identity();
        auto jointAxis = TransformMatrix::identity();
        if (above != NULL) {
            preTransform = above->transformationBackward();
           
            jointFrame = parentPreTransform.multiply(above->anchor->transformationForward());
            jointBasis = jointFrame;
            jointBasis.setX(0);
            jointBasis.setY(0);
            jointBasis.setZ(0);
            jointAxis = jointBasis.multiply(TransformMatrix::translation(0, 0, above->anchor->sign()));

            jointFrame = jointFrame.multiply(jointBasis.invert());
            preTransform = jointBasis.multiply(preTransform);
        }

        for (auto ref : refs()) {
            dynamics.combine(ref->getDynamics(), ref->matrix);
            tmp.str("");
            tmp << module->getName() << "_" << ref->name << "_" << id << "_" << (refid++);
            auto refName = tmp.str();
            auto jointName = refName+"_joint";
            if (this->name != "") {
                jointName = this->name;
            }

            ss << "    <visual>" << std::endl;
            ss << "      <geometry>" << std::endl;
            // XXX: This forces the file to be in an urdf/ folder
            ss << "        <mesh filename=\"package://urdf/" << ref->hash() << ".stl\"/>" << std::endl;
            ss << "      </geometry>" << std::endl;
            ss << "      <material name=\"" << refName << "_material\">" << std::endl;
            ss << "        <color rgba=\"" << ref->r << " " << ref->g << " " << ref->b << " 1.0\"/>" << std::endl;
            ss << "      </material>" << std::endl;
            ss << "    " << preTransform.multiply(ref->matrix).toURDF() << std::endl;
            ss << "    </visual>" << std::endl;
        }

        // Adding dynamics
        ss << "  <inertial>" << std::endl;
        auto com = preTransform.apply(dynamics.com);;
        ss << "    <origin xyz=\"" << (com.x()/1000) << " " 
            << (com.y()/1000) << " " << (com.z()/1000) << "\" rpy=\"0 0 0\"/>" << std::endl;
        ss << "    <mass value=\"" << (dynamics.mass/1000.0) << "\"/>" << std::endl;
        ss << "    <inertia ixx=\"" << dynamics.ixx/1e9 << 
            "\"  ixy=\"" << dynamics.ixy/1e9 << 
            "\"  ixz=\"" << dynamics.ixz/1e9 << 
            "\" iyy=\"" << dynamics.iyy/1e9 << 
            "\" iyz=\"" << dynamics.iyz/1e9 << 
            "\" izz=\"" << dynamics.izz/1e9 << 
            "\" />" << std::endl;
        ss << "  </inertial>" << std::endl;

        // Adding collisions
        for (auto shape : shapes) {
            ss << "  <collision>" << std::endl;
            ss << shape.toURDF(preTransform) << std::endl;
            ss << "  </collision>" << std::endl;
        }
            
        ss << "  </link>" << std::endl;

        // Linking it to the parent
        if (above!=NULL) {
            ss << "  <joint name=\"" << name << "\" type=\"revolute\">" << std::endl;
            ss << "    <parent link=\"" << parent << "\"/>" << std::endl;
            ss << "    <child link=\"" << name << "\"/>" << std::endl;
            ss << "    <axis xyz=\"" << smp(jointAxis.x()) << " " << smp(jointAxis.y()) << " " << smp(jointAxis.z()) << "\"/>" << std::endl;
            ss << jointFrame.toURDF() << std::endl;

            auto motor = backend->config.motors[above->anchor->type];
            ss << "    <limit effort=\"" << motor.maxTorque << "\" velocity=\"" << motor.maxSpeed << "\" lower=\"" << -M_PI << "\" upper=\"" << M_PI << "\"/>" << std::endl;
            ss << "  </joint>" << std::endl;
        }

        // Drawing tips
        int tipNum = 0;
        /*
        for (auto tip : tips) {
            std::stringstream tmp;

            if (this->tipName == "") {
                tmp << name << "_tip";
            } else {
                tmp << this->tipName;
            }
            if (tipNum > 0) tmp << "_" << tipNum;

            std::string tipName = tmp.str();

            ss << "<link name=\"" << tipName << "\">" << std::endl;
            ss << "    <inertial>" << std::endl;
            ss << "    <origin xyz=\"0 0 0\" rpy=\"0 0 0\" />" << std::endl;
            ss << "    <mass value=\"0\" />" << std::endl;
            ss << "    <inertia ixx=\"0\" ixy=\"0\" ixz=\"0\" iyy=\"0\" iyz=\"0\" izz=\"0\" />" << std::endl;
            ss << "    </inertial>" << std::endl;
            ss << "</link>" << std::endl;
            ss << "<joint name=\"" << tipName << "\" type=\"fixed\">" << std::endl;
            auto m = preTransform.multiply(tip);
            ss << m.toURDF() << std::endl;
            ss << "    <parent link=\"" << name << "\" />" << std::endl;
            ss << "    <child link=\"" << tipName << "\" />" << std::endl;
            ss << "    <axis xyz=\"0 0 0\" />" << std::endl;
            ss << "</joint>" << std::endl;
        }
        */

        // Drawing sub-components
        for (auto anchor : anchors) {
            if (anchor->above && anchor->anchor!=NULL) {
                anchor->anchor->component->writeURDF(ss, name, preTransform, anchor->anchor);
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
        ss << "        <ixx>" << dynamics.ixx/1e9 << "</ixx>" << std::endl;
        ss << "        <ixy>" << dynamics.ixy/1e9 << "</ixy>" << std::endl;
        ss << "        <ixz>" << dynamics.ixz/1e9 << "</ixz>" << std::endl;
        ss << "        <iyy>" << dynamics.iyy/1e9 << "</iyy>" << std::endl;
        ss << "        <iyz>" << dynamics.iyz/1e9 << "</iyz>" << std::endl;
        ss << "        <izz>" << dynamics.izz/1e9 << "</izz>" << std::endl;
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
            auto state = (btDefaultMotionState*)body->getMotionState();
            trans = state->m_graphicsWorldTrans;
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
        // Computing com matrix
        btMatrix3x3 inertiaM(
                dynamics.ixx/1e9, dynamics.ixy/1e9, dynamics.ixz/1e9,
                dynamics.ixy/1e9, dynamics.iyy/1e9, dynamics.iyz/1e9,
                dynamics.ixz/1e9, dynamics.iyz/1e9, dynamics.izz/1e9
                );
        btMatrix3x3 rot;

        // Getting inertia principal axes
        inertiaM.diagonalize(rot, 0.00001, 20);
        auto nInertia = inertiaM*rot;
        btVector3 localInertia(
                nInertia[0][0], nInertia[1][1], nInertia[2][2]
                );
        // std::cout << module->getName() << std::endl;
        // std::cout << localInertia.x() << " " << localInertia.y() << " " << localInertia.z() << std::endl;

        // Center of mass transformation
        com = btTransform::getIdentity();
        com.setOrigin(dynamics.com.multiply(1/1000.0).toBullet());
        com.setBasis(rot);

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
                auto trans = matrix.toBullet();
                trans = com.inverse()*trans;
                compound->addChildShape(trans, colShape);
            }
        }

        // Creating rigid body
        // if (above == NULL) dynamics.mass = 0;
        inertia = localInertia;
        mass = dynamics.mass/1000.0;
        body = world->createRigidBody(mass, matrix.toBullet(), compound,
                inertia, com.inverse());
        body->setUserPointer(this);

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
                auto childComponent = anchor->anchor->component;
                auto child = childComponent->toBullet(world, anchor->anchor, worldToChild);

                // Creating dummy body for backlash simulation
                auto empty = world->createEmpty();
                auto empty2 = world->createEmpty();
                /*
                   auto dummy2 = world->createRigidBody(1e-3, worldToDummy.toBullet(), empty2, 
                   btVector3(1e-5, 1e-5, 1e-5));
                   */

                if (!backend->config.backlash) {
                    anchor->anchor->component->posHinge = new btHingeConstraint(*body, *child,
                            com.inverse()*anchor->transformationForward().toBullet(),
                            childComponent->com.inverse()*anchor->anchor->transformationForward().toBullet()
                            );

                    /////////////// MODE SIMPLE
                    // Creating hinge
                    anchor->anchor->component->hinge = world->createHinge(body, child,
                            com.inverse()*anchor->transformationForward().toBullet(),
                            childComponent->com.inverse()*anchor->anchor->transformationForward().toBullet()
                            );
                }

                if (backend->config.backlash) {
                    auto dummy = world->createRigidBody(1e-3, worldToDummy.toBullet(), empty, 
                            btVector3(1e-5, 1e-5, 1e-5));

                    //////////////// MODE CONE
                    anchor->anchor->component->posHinge = new btHingeConstraint(*body, *dummy,
                            com.inverse()*anchor->transformationForward().toBullet(),
                            btTransform::getIdentity()
                            );

                    // Creating hinge
                    anchor->anchor->component->hinge = world->createHinge(body, dummy,
                            com.inverse()*anchor->transformationForward().toBullet(),
                            btTransform::getIdentity()
                            );

                    // Creating cone
                    auto cone = world->createCone(dummy, child,
                            rot.toBullet(),
                            childComponent->com.inverse()*anchor->anchor->transformationForward().multiply(rot).toBullet()
                            );
                    //             Axe   Latéral Torsion
                    cone->setLimit(0.04, 0.02, 0.02);
                    //cone->setLimit(0.01, 0.01, 0.01);
                    // cone->setLimit(0.0, 0.0, 0.0);
                }

#if 0
                auto dummy = world->createRigidBody(1e-3, worldToDummy.toBullet(), empty, 
                        btVector3(1e-6, 1e-6, 1e-6));

                ////////////////// MODE GEAR
                if (anchor->anchor->component->posHinge != NULL) {
                    delete anchor->anchor->component->posHinge;
                }
                anchor->anchor->component->posHinge = new btHingeConstraint(*body, *dummy2,
                        anchor->transformationForward().toBullet(),
                        btTransform::getIdentity()
                        );

                anchor->anchor->component->hinge = world->createHinge(body, dummy,
                        anchor->transformationForward().toBullet(),
                        btTransform::getIdentity()
                        );

                auto hinge2 = world->createHinge(dummy, dummy2,
                        btTransform::getIdentity(),
                        btTransform::getIdentity()
                        );
                auto gear = world->createGear(dummy, dummy2,
                        btVector3(0, 0, 1),
                        btVector3(0, 0, 1),
                        -50
                        );

                world->createFixed(dummy2, child,
                        btTransform::getIdentity(),
                        anchor->anchor->transformationForward().toBullet()
                        );

                /*
                   auto cone = world->createCone(dummy2, child,
                   rot.toBullet(),
                   anchor->anchor->transformationForward().multiply(rot).toBullet()
                   );
                   cone->setLimit(0.03, 0.04, 0.02);
                   */
#endif
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
        if (name != "") {
            ss << name;
        } else {
            ss << module->getName();
        }
        ss << " #" << id;
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
        return parameters.get(name, "");
    }

    void Component::set(std::string name, std::string value)
    {
        parameters.set(name, value);
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
        auto oldAnchors = anchors;
        AnchorPoint *parentAnchor = NULL;
        int index = 0;
        int parentId = -1;
        for (auto anchor : anchors) {
            if (!anchor->above && anchor->anchor) {
                parentAnchor = anchor->anchor;
                parentAnchor->detach(false);
                parentId = index;
            }
            index++;
        }

        // Creating CSG 
        std::string csg = module->openscad("csg", compiledParameters(robot));
        // Main reference
        main.name = module->getName();
        main.parameters = compiledParameters(robot);
        // Parsing the CSG document
        CSG *document = CSG::parse(csg);
        anchors = document->anchors;
        parts = document->parts;
        models = document->models;
        bom = document->bom;
        tips = document->tips;
        delete document;

        // Collision CSG & STL
        std::string collisionsCsg = module->openscad("csg", compiledParameters(robot), DEFINE_COLLISIONS);
        collisions = loadModelSTL_string(stl(robot, true));
        CSG *collisionsDocument = CSG::parse(collisionsCsg);
        shapes = collisionsDocument->shapes;
        for (auto anchor : collisionsDocument->anchors) {
            delete anchor;
        }
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
            moveAnchors(oldAnchors);
            if (parentAnchor) {
                parentAnchor->attach(anchors[parentId]);
            }
        }
        for (auto anchor : oldAnchors) {
            delete anchor;
        }
    }

    void Component::moveAnchors(std::vector<AnchorPoint*> otherAnchors)
    {
        // First step:
        // Trying to take the items that matches *exactly* the anchor from the
        // old component
        for (unsigned int i=0; i<anchors.size(); i++) {
            if (i < otherAnchors.size()) {
                AnchorPoint *myAnchor = anchors[i];
                AnchorPoint *otherAnchor = otherAnchors[i];

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
        for (auto anchor : otherAnchors) {
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

    AnchorPoint *Component::freeAnchor()
    {
        for (auto anchor : anchors) {
            if (anchor->anchor == NULL) {
                return anchor;
            }
        }

        return NULL;
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

    Parameters Component::compiledParameters(Robot *robot)
    {
        Parameters params = module->getParameters();

        for (auto entry : parameters) {
            auto key = entry.first;
            auto param = entry.second;
            if (param.value.size()>0 && param.value[0]=='$' && robot!=NULL) {
                params.update(key, robot->getValue(param.value.substr(1)));
            } else {
                params.update(key, param.value);
            }
        }

        return params;
    }

    std::string Component::stl(Robot *robot, bool drawCollisions)
    {
        int defines = drawCollisions ? DEFINE_COLLISIONS : DEFINE_NO_MODELS;
        return module->openscad("stl", compiledParameters(robot), defines);
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
        parameters = Parameters::fromJson(json);
    }

    Json::Value Component::toJson()
    {
        Json::Value json(Json::objectValue);

        json["name"] = name;
        json["tipName"] = tipName;
        json["component"] = module->getName();
        json["parameters"] = parameters.toJson();
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
                json["anchors"][id]["inverted"] = anchor->inverted;
                json["anchors"][id]["minimum"] = anchor->minimum;
                json["anchors"][id]["maximum"] = anchor->maximum;
                json["anchors"][id]["remote"] = anchor->anchor->id;
                json["anchors"][id]["component"] = anchor->anchor->component->toJson();
            }
        }

        return json;
    }

    void Component::foreachComponent(std::function<void(Component *component, TransformMatrix m)> method, TransformMatrix m)
    {
        method(this, m);

        for (auto anchor : anchors) {
            if (anchor->anchor != NULL && anchor->above) {
                TransformMatrix t = m.multiply(anchor->transformationForward());
                t = t.multiply(anchor->anchor->transformationBackward());
                anchor->anchor->component->foreachComponent(method, t);
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

    double Component::setTarget(float alpha, float dt)
    {
        if (hinge == NULL) {
            return 0;
        }

        if (!hinge->getJointFeedback()) {
            auto fb = new btJointFeedback();
            fb->m_appliedForceBodyA.setZero();
            fb->m_appliedTorqueBodyA.setZero();
            fb->m_appliedForceBodyB.setZero();
            fb->m_appliedTorqueBodyB.setZero();
            hinge->enableFeedback(true);
            hinge->setJointFeedback(fb);
        }

        auto above = aboveAnchor();
        auto motor = backend->config.motors[above->type];
        double maxSpeed = motor.maxSpeed;
        double maxTorque = motor.maxTorque;

        alpha = -alpha*above->sign();
        auto pos = posHinge->getHingeAngle();
        //auto pos = hinge->getHingeAngle();

        // vel = vel*0.9 + 0.1*getVelocity();
        vel = vel*0.8 + 0.2*(pos-lastPos)/dt;
        //vel = getVelocity();

        // Speed servoing
        float error = alpha-pos;
        float targetVel = bound(error*motor.speedGain, -maxSpeed, maxSpeed);

        // Limiting torque in function of current speed
        float errorVel = targetVel-vel;
        float coef = bound(fabs(vel/maxSpeed), 0, 1);
        if ((targetVel < 0 && vel > 0)
                || (targetVel > 0 && vel < 0)) coef = 0;
        float maxForce = maxTorque*(1-coef);

        // Torque servoing
        targetForce = bound(errorVel*motor.torqueGain, -maxForce, maxForce);

        double cost = 0;
        if (backend->config.mode == MODE_MOTORS) {
            hinge->enableAngularMotor(true, targetVel, dt*maxForce);

            if (hinge->getJointFeedback()) {
                auto fb = hinge->getJointFeedback();
                cost += fb->m_appliedTorqueBodyA.norm()*dt;
                cost += fb->m_appliedTorqueBodyB.norm()*dt;
            }
        } else if (backend->config.mode == MODE_TORQUE) {
            btVector3 hingeAxisLocalA =
                hinge->getFrameOffsetA().getBasis().getColumn(2);
            btVector3 hingeAxisLocalB =
                hinge->getFrameOffsetB().getBasis().getColumn(2);

            btVector3 hingeAxisWorldA =
                hinge->getRigidBodyA().getWorldTransform().getBasis() *
                hingeAxisLocalA;
            btVector3 hingeAxisWorldB =
                hinge->getRigidBodyB().getWorldTransform().getBasis() *
                hingeAxisLocalB;

            btVector3 hingeTorqueA = targetForce * hingeAxisWorldA;
            btVector3 hingeTorqueB = targetForce * hingeAxisWorldB;

            hinge->getRigidBodyA().applyTorque(hingeTorqueA);
            hinge->getRigidBodyB().applyTorque(-hingeTorqueB);

            // This can be used to add some frictions
            // hinge->enableAngularMotor(true, 0.0, dt*0.02);

            cost = fabs(targetForce*dt);
        }

        if (body) body->activate();
        lastPos = pos;

        return cost;
    }
}
