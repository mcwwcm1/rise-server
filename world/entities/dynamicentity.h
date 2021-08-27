// File: dynamicentity.h
// Purpose: A PhysicsEntity that has a rigidbody

#pragma once

#include "../physics/physicsspace.h"
#include "entity.h"

#include <vector>

class PhysicsSpace;

class EntityMotionState : public btMotionState
{
 public:
	Entity* TargetEntity;

	EntityMotionState(Entity* targetEntity);

	virtual void getWorldTransform(btTransform& worldTrans) const;

	//Bullet only calls the update of worldtransform for active objects
	virtual void setWorldTransform(const btTransform& worldTrans);
};

class DynamicEntity : public Entity
{
 public:
	DynamicEntity();
	DynamicEntity(Double3 position, Quaternion rotation, float mass);

	virtual ~DynamicEntity();

	PhysicsSpace* Space;
	btRigidBody* RigidBody;
	btCompoundShape* Shape;
	EntityMotionState* MotionState;

	virtual void RegisterToDynamicsWorld(btDynamicsWorld* world);
	virtual void UnregisterFromDynamicsWorld(btDynamicsWorld* world);
};

class CustomRigidbody : public btRigidBody
{
};