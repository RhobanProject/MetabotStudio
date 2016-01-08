#include <iostream>
#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include "World.h"

    World::World()
:       m_broadphase(0),
    m_dispatcher(0),
    m_solver(0),
    m_collisionConfiguration(0),
    m_dynamicsWorld(0),
    zOffset(0)
{
    // Collision configuration contains default setup for memory, collision setup
    m_collisionConfiguration = new btDefaultCollisionConfiguration();

    // Use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    m_dispatcher = new	btCollisionDispatcher(m_collisionConfiguration);

    m_broadphase = new btDbvtBroadphase();

    // The default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    m_solver = new btSequentialImpulseConstraintSolver;

    m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);

    m_dynamicsWorld->setGravity(btVector3(0, 0, -10));
    m_dynamicsWorld->setDebugDrawer(&drawer);

    m_dynamicsWorld->getSolverInfo().m_solverMode |=
        SOLVER_SIMD |
        SOLVER_USE_WARMSTARTING |
        SOLVER_CACHE_FRIENDLY |
        SOLVER_RANDMIZE_ORDER |
        SOLVER_USE_2_FRICTION_DIRECTIONS
        ;

    /*
    // Adding a box
    btRigidBody *last = NULL;
    for (int k=0; k<10; k++) {
    auto box = new btSphereShape(0.03);
    box->setMargin(0.00001);
    trans = btTransform::getIdentity();
    trans.getOrigin().setZ(k*0.08+0.03);
    trans.getOrigin().setX(k*0.01);
    auto body = createRigidBody(last == NULL ? 0.0 : 1.0, trans, box);

    if (last != NULL) {
    auto A = btVector3(0.0, 0.0, 0.04);
    auto B = btVector3(0.0, 0.0, -0.04);
    auto X = btVector3(0.0, 1.0, 0.0);
    auto hinge = new btHingeConstraint(*last, *body, A, B, X, X);
    hinge->enableAngularMotor(true, 100.0, 1000.0);
    m_dynamicsWorld->addConstraint(hinge, false);
    hinges.push_back(hinge);
    }
    last = body;
    }
    */

    clear();
}

World::~World()
{
    clear(false);

    delete m_dynamicsWorld;
    m_dynamicsWorld=0;

    delete m_solver;
    m_solver=0;

    delete m_broadphase;
    m_broadphase=0;

    delete m_dispatcher;
    m_dispatcher=0;

    delete m_collisionConfiguration;
    m_collisionConfiguration=0;
}

void World::stepSimulation(float deltaTime)
{
    if (m_dynamicsWorld) {
        m_dynamicsWorld->stepSimulation(deltaTime, 1, deltaTime);
    }
    /*
       static float t = 0.0;
       t += deltaTime;
       int k = 0;
       for (auto hinge : hinges) {
       auto angle = hinge->getHingeAngle();
       angle -= sin(t);
       std::cout << "[" << (k++) << "] ERR: " << angle << std::endl;
       hinge->enableAngularMotor(true, -100*angle, 1.0);

       }
       */
}

std::vector<std::pair<Vect, Vect>> World::getGroundCollisions()
{
    std::vector<std::pair<Vect, Vect>> points;

    if (m_dynamicsWorld) {
        int numManifolds = m_dynamicsWorld->getDispatcher()->getNumManifolds();
        for (int i=0;i<numManifolds;i++) {
            btPersistentManifold* contactManifold =  m_dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
            auto A = contactManifold->getBody0();
            auto B = contactManifold->getBody1();

            if (A == ground || B == ground) {
                int nb = contactManifold->getNumContacts();
                for (int k=0; k<nb; k++) {
                    auto contactPoint = contactManifold->getContactPoint(k);
                    auto force = fabs(contactPoint.m_appliedImpulse);
                    auto lat1 = contactPoint.m_lateralFrictionDir1;
                    auto lat2 = contactPoint.m_lateralFrictionDir2;
                    auto friction = Vect::fromBullet(lat1).multiply(contactPoint.m_appliedImpulseLateral1);
                    // if (lat1 != lat2) {
                        friction = friction.add(
                                Vect::fromBullet(lat2).multiply(contactPoint.m_appliedImpulseLateral2)
                                );
                    // }

                    auto point = Vect::fromBullet(contactPoint.m_positionWorldOnB);
                    auto normal = Vect::fromBullet(contactPoint.m_normalWorldOnB).multiply(force);

                    points.push_back(std::pair<Vect, Vect>(point, normal.add(friction)));
                }
            }
        }
    }

    return points;
}

Vect World::getGroundForce(bool frictions)
{
    Vect total(0, 0, 0);
    if (m_dynamicsWorld) {
        int numManifolds = m_dynamicsWorld->getDispatcher()->getNumManifolds();
        for (int i=0;i<numManifolds;i++) {
            btPersistentManifold* contactManifold =  m_dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
            auto A = contactManifold->getBody0();
            auto B = contactManifold->getBody1();

            if (A == ground || B == ground) {
                int nb = contactManifold->getNumContacts();
                for (int k=0; k<nb; k++) {
                    auto contactPoint = contactManifold->getContactPoint(k);
                    auto force = contactPoint.m_appliedImpulse;
                    auto lat1 = contactPoint.m_lateralFrictionDir1;
                    auto lat2 = contactPoint.m_lateralFrictionDir2;
                    auto friction = Vect::fromBullet(lat1).multiply(contactPoint.m_appliedImpulseLateral1);
                    if (lat1 != lat2) {
                        friction = friction.add(
                                Vect::fromBullet(lat2).multiply(contactPoint.m_appliedImpulseLateral2)
                                );
                    }

                    auto normal = Vect::fromBullet(contactPoint.m_normalWorldOnB).multiply(force);
                   
                    total = total.add(normal);
                    if (frictions) {
                        total = total.add(friction);
                    }
                }
            }
        }
    }

    return total;
}

btRigidBody* World::createRigidBody(float mass, btTransform startTransform, btCollisionShape* shape,  btVector3 inertia)
{
    btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));

    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool isDynamic = (mass != 0.f);

    if (isDynamic && inertia==btVector3(0,0,0)) {
        shape->calculateLocalInertia(mass, inertia);
    }

    auto origin = startTransform.getOrigin();
    origin.setZ(origin.getZ()-zOffset);
    startTransform.setOrigin(origin);
    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects

#define USE_MOTIONSTATE 1
#ifdef USE_MOTIONSTATE
    btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

    btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, shape, inertia);
    cInfo.m_friction = 0.65;

    btRigidBody* body = new btRigidBody(cInfo);
    //body->setContactProcessingThreshold(m_defaultContactProcessingThreshold);
#else
    btRigidBody* body = new btRigidBody(mass, 0, shape, inertia);
    body->setWorldTransform(startTransform);
#endif//

    body->setUserIndex(-1);
    m_dynamicsWorld->addRigidBody(body);
    bodies.push_back(body);

    return body;
}

btCompoundShape *World::createCompound()
{
    auto shape = new btCompoundShape();
    shapes.push_back(shape);

    return shape;
}

btCollisionShape *World::createBox(btScalar a, btScalar b, btScalar c)
{
    auto shape = new btBoxShape(btVector3(a/2, b/2, c/2));
    shapes.push_back(shape);

    return shape;
}

btCollisionShape *World::createSphere(btScalar r)
{
    auto shape = new btSphereShape(r);
    shapes.push_back(shape);

    return shape;
}

btCollisionShape *World::createCylinder(btScalar r, btScalar h)
{
    auto shape = new btCylinderShape(btVector3(r, h/2, 0));
    shapes.push_back(shape);

    return shape;
}

btCollisionShape *World::createEmpty()
{
    auto shape = new btEmptyShape();
    shapes.push_back(shape);

    return shape;
}

btHingeConstraint *World::createHinge(btRigidBody *A, btRigidBody *B, btTransform AFrame, btTransform BFrame)
{
    auto hinge = new btHingeConstraint(*A, *B, AFrame, BFrame);
    m_dynamicsWorld->addConstraint(hinge, true);

    hinges.push_back(hinge);
    return hinge;
}

btConeTwistConstraint *World::createCone(btRigidBody *A, btRigidBody *B, btTransform AFrame, btTransform BFrame)
{
    auto cone = new btConeTwistConstraint(*A, *B, AFrame, BFrame);
    m_dynamicsWorld->addConstraint(cone, true);

    cones.push_back(cone);
    return cone;
}

void World::clear(bool makeGround)
{
    zOffset = 0;
    for (auto cone : cones) {
        m_dynamicsWorld->removeConstraint(cone);
        delete cone;
    }
    for (auto hinge : hinges) {
        m_dynamicsWorld->removeConstraint(hinge);
        delete hinge;
    }
    for (auto body : bodies) {
        if (body && body->getMotionState()) {
            delete body->getMotionState();
        }
        m_dynamicsWorld->removeRigidBody(body);
        delete body;
    }
    for (auto shape : shapes) {
        delete shape;
    }

    bodies.clear();
    cones.clear();
    hinges.clear();
    shapes.clear();

    if (makeGround) {
        // Adding a ground
        auto plane = new btBoxShape(btVector3(1000, 1000, 10));
        shapes.push_back(plane);
        plane->setMargin(0.0);
        auto trans = btTransform::getIdentity();
        trans.getOrigin().setZ(-10);
        ground = createRigidBody(0.0, trans, plane);
    }
}

void World::debugDraw()
{
    m_dynamicsWorld->debugDrawWorld();
}

void World::freeze()
{
    for (auto body : bodies) {
        body->clearForces();
        body->setLinearVelocity(btVector3(0, 0, 0));
    }
}
