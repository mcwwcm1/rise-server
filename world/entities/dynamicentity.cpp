// File: dynamicentity.cpp
// Purpose: Implements dynamicentity.h

#include "dynamicentity.h"
#include "mysticmath/mysticmath.h"
#include "../physics/physicsspace.h"

DynamicEntity::DynamicEntity() : DynamicEntity("null") {}

DynamicEntity::DynamicEntity(std::string id) : DynamicEntity(id, Double3(0, 0, 0), Quaternion::identity, 1) {}

DynamicEntity::DynamicEntity(std::string id, Double3 position, Quaternion rotation, float mass) : Entity(id, position, rotation)
{
	ID          = id;
	Shape       = new btCompoundShape();
	MotionState = new EntityMotionState(this);

	auto info = btRigidBody::btRigidBodyConstructionInfo(mass, MotionState, Shape);

	info.m_localInertia   = btVector3(1, 1, 1);
	info.m_linearDamping  = 0.7;
	info.m_angularDamping = .7;
	info.m_restitution    = 0;

	RigidBody = new btRigidBody(info);  // Builds a dynamic RigidBody
}

DynamicEntity::~DynamicEntity()
{
	delete RigidBody;
	if (Space != nullptr)
		Space->UnregisterEntity(this);
}

void DynamicEntity::RegisterToDynamicsWorld(btDynamicsWorld* world)
{
	printf("Registering to dynamics world\n");
	world->addRigidBody(RigidBody);
}

void DynamicEntity::UnregisterFromDynamicsWorld(btDynamicsWorld* world)
{
	world->removeRigidBody(RigidBody);
}

EntityMotionState::EntityMotionState(Entity* targetEntity)
{
	TargetEntity = targetEntity;
}

void EntityMotionState::getWorldTransform(btTransform& worldTrans) const
{
	worldTrans.setOrigin(TargetEntity->Position);
	worldTrans.setRotation(TargetEntity->Rotation);
}

void EntityMotionState::setWorldTransform(const btTransform& worldTrans)
{
	TargetEntity->Position = worldTrans.getOrigin();
	TargetEntity->Rotation = worldTrans.getRotation();

	TargetEntity->SubmitChange("position", TargetEntity->Position.ToString());
	TargetEntity->SubmitChange("rotation", TargetEntity->Rotation.ToString());
}