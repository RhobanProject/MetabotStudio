#pragma once

#include <utility>
#include <vector>
#include <Vect.h>
#include <btBulletDynamicsCommon.h>
#include "GLDebugDrawer.h"

using namespace Metabot;

class World
{
    public:
	World();
        virtual ~World();
	
        // Step the simulation by a given time
        virtual void stepSimulation(float deltaTime);

        // Clear (empty the world)
        void clear(bool makeGround=true);
        void freeze();

        // Creating shapes
        btCompoundShape *createCompound();
        btCollisionShape *createBox(btScalar a, btScalar b, btScalar c);
        btCollisionShape *createSphere(btScalar r);
        btCollisionShape *createCylinder(btScalar r, btScalar h);
        btCollisionShape *createEmpty();

        // Creating hinges
        btHingeConstraint *createHinge(btRigidBody *A, btRigidBody *B, btTransform AFrame, btTransform BFrame);
        btHingeConstraint *createHinge(btRigidBody *A, btVector3 pivot, btVector3 axis, bool AFrame=false);
        btConeTwistConstraint *createCone(btRigidBody *A, btRigidBody *B, btTransform AFrame, btTransform BFrame);
        btGearConstraint *createGear(btRigidBody *A, btRigidBody *B, btVector3 AFrame, btVector3 BFrame, btScalar ratio);
        btFixedConstraint *createFixed(btRigidBody *A, btRigidBody *B, btTransform AFrame, btTransform BFrame);
	
        // Creating a rigid body
        btRigidBody* createRigidBody(float mass, btTransform startTransform, btCollisionShape* shape, btVector3 inertia = btVector3(0, 0, 0), btTransform com = btTransform::getIdentity());

        double getGroundNonTipCollisions();
        double getAutoCollisions();
        Vect getGroundForce(bool friction=true);
        std::vector<std::pair<Vect, Vect>> getGroundCollisions();

        void debugDraw();
        float zOffset;
        float friction;
        GLDebugDrawer drawer;
	
        btDiscreteDynamicsWorld* m_dynamicsWorld;

    protected:
        // Elements
        std::vector<btCollisionShape *> shapes;
        std::vector<btRigidBody *> bodies;
        btRigidBody *ground;
        std::vector<btTypedConstraint *> constraints;

        //keep the collision shapes, for deletion/cleanup
	// btAlignedObjectArray<btCollisionShape*>	m_collisionShapes;
	btBroadphaseInterface* m_broadphase;
	btCollisionDispatcher* m_dispatcher;
	btConstraintSolver* m_solver;
	btDefaultCollisionConfiguration* m_collisionConfiguration;
};
