#pragma once

#include <vector>
#include <bullet/btBulletDynamicsCommon.h>
#include "GLDebugDrawer.h"

class World
{
    public:
	World();
        virtual ~World();
	
        // Step the simulation by a given time
        virtual void stepSimulation(float deltaTime);

        // Clear (empty the world)
        void clear();
        void freeze();

        // Creating shapes
        btCompoundShape *createCompound();
        btCollisionShape *createBox(btScalar a, btScalar b, btScalar c);
        btCollisionShape *createSphere(btScalar r);
        btCollisionShape *createCylinder(btScalar r, btScalar h);

        // Creating hinges
        btHingeConstraint *createHinge(btRigidBody *A, btRigidBody *B, btTransform AFrame, btTransform BFrame);
	
        // Creating a rigid body
        btRigidBody* createRigidBody(float mass, btTransform startTransform, btCollisionShape* shape, btVector3 inertia = btVector3(0, 0, 0));

        void debugDraw();
        float zOffset;
        GLDebugDrawer drawer;

    protected:
        // Elements
        std::vector<btCollisionShape *> shapes;
        std::vector<btRigidBody *> bodies;
        std::vector<btHingeConstraint *> hinges;

        //keep the collision shapes, for deletion/cleanup
	// btAlignedObjectArray<btCollisionShape*>	m_collisionShapes;
	btBroadphaseInterface* m_broadphase;
	btCollisionDispatcher* m_dispatcher;
	btConstraintSolver* m_solver;
	btDefaultCollisionConfiguration* m_collisionConfiguration;
	btDiscreteDynamicsWorld* m_dynamicsWorld;
};
