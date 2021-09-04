// File: dynamicentity.cpp
// Purpose: Implements dynamicentity.h

#include "DynamicEntity.h"

#include "MysticMath/MysticMath.h"
#include "World/Physics/PhysicsSpace.h"

DynamicEntity::DynamicEntity() : DynamicEntity(Double3(0, 0, 0), Quaternion::identity, 1) {}

DynamicEntity::DynamicEntity(Double3 position, Quaternion rotation, float mass) : Entity(position, rotation)
{
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
	Double3 pos;
	pos = worldTrans.getOrigin();
	Quaternion rot;
	rot = worldTrans.getRotation();
	TargetEntity->SetLocalPosition(pos);
	TargetEntity->SetLocalRotation(rot);
}