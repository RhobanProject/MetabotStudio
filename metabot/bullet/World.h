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
        btConeTwistConstraint *createCone(btRigidBody *A, btRigidBody *B, btTransform AFrame, btTransform BFrame);
	
        // Creating a rigid body
        btRigidBody* createRigidBody(float mass, btTransform startTransform, btCollisionShape* shape, btVector3 inertia = btVector3(0, 0, 0));

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
        std::vector<btHingeConstraint *> hinges;
        std::vector<btConeTwistConstraint *> cones;

        //keep the collision shapes, for deletion/cleanup
	// btAlignedObjectArray<btCollisionShape*>	m_collisionShapes;
	btBroadphaseInterface* m_broadphase;
	btCollisionDispatcher* m_dispatcher;
	btConstraintSolver* m_solver;
	btDefaultCollisionConfiguration* m_collisionConfiguration;
};
