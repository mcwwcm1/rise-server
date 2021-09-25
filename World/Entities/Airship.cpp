// File: airship.cpp
// Purpose: Implements airship.h

#include "Airship.h"
#include <functional>

Airship::Airship() : Airship::Airship(Double3(0, 0, 0), Quaternion::identity) {}

Airship::Airship(Double3 position, Quaternion rotation)
		: DynamicEntity(position, rotation, 10), RootComponent(VesselTemplates::Starter())
{
	std::function<void(const std::shared_ptr<VesselComponent>&)> addColliders =
			[&](const std::shared_ptr<VesselComponent>& component) -> void {
		const auto& info = component->Info();
		for (auto& collider : info.Colliders) {
			btMatrix3x3 rotationScale(collider.Rotation);
			rotationScale = rotationScale.scaled(collider.Scale);
			btTransform transform(rotationScale, collider.Position);

			const Double3 size = collider.Size;

			btCollisionShape* shape = nullptr;

			printf("Adding vessel collider: ");
			switch (collider.Type) {
				case ComponentColliderType::Box:
					printf("Box\n");
					shape = new btBoxShape(size * 0.5);
					break;

				case ComponentColliderType::Sphere:
					printf("Sphere\n");
					shape = new btSphereShape(size.x);
					break;

				case ComponentColliderType::Capsule:
					printf("Capsule\n");
					shape = new btCapsuleShape(size.x, size.y);
					break;

				case ComponentColliderType::Cylinder:
					printf("Cylinder\n");
					shape = new btCylinderShape(btVector3(size.x, size.y * 0.5, size.x));
					break;
			}

			if (shape != nullptr) {
				btVector3 aabbMin, aabbMax;
				shape->getAabb(transform, aabbMin, aabbMax);
				const btVector3 aabbSize = aabbMax - aabbMin;
				printf("- AABB: %f %f %f\n", aabbSize.x(), aabbSize.y(), aabbSize.z());

				Shape->addChildShape(transform, shape);
			}
		}

		for (auto& child : component->Slots()) { addColliders(child.second); }
	};
	addColliders(RootComponent);

	// HARDCODED COLLIDER SHAPE = BAD
	// btSphereShape* sphereShape = new btSphereShape(2);
	// Shape->addChildShape(btTransform(btQuaternion(0, 0, 0), btVector3(0, 0, 2.5)), sphereShape);
	// Shape->addChildShape(btTransform(btQuaternion(0, 0, 0), btVector3(0, 0, 0.5)), sphereShape);
	// Shape->addChildShape(btTransform(btQuaternion(0, 0, 0), btVector3(0, 0, -2)), sphereShape);

	btVector3 inertia;
	Shape->calculateLocalInertia(RigidBody->getMass(), inertia);
	RigidBody->setMassProps(RigidBody->getMass(), inertia);

	Throttle = 0;
	Pitch    = 0;
	Yaw      = 0;
}

std::string Airship::GetCreationCommand() { return "SpawnEntity StarterAirship|" + ID + "|"; }

void Airship::RunTick(float dt)
{
	Double3 forward, right, up;

	forward = GetForward();
	right   = GetRight();
	up      = GetUp();

	float speed = RigidBody->getLinearVelocity().length();

	RigidBody->activate();  // Activate the body because it likes to be innactive

	RigidBody->setGravity(btVector3(0, 0, 0));  // Dumb. Move it later

	// Apply thrust
	RigidBody->applyCentralForce(forward * Throttle * 25);

	// Apply yaw steering
	RigidBody->applyTorque(up * Dot(Yaw, speed) * -5);

	// Apply pitch steering
	RigidBody->applyTorque(right * Dot(Pitch, speed) * 7);

	// Self-righting
	RigidBody->applyTorque(Cross(Double3(0, -1, 0), up) * 200);
}

void Airship::OnRegistered()
{
	std::function<void(const std::shared_ptr<VesselComponent>&, const std::string&)> submit =
			[&](const std::shared_ptr<VesselComponent>& comp, const std::string& location) {
				std::ostringstream oss;
				oss << location << ":";
				oss << comp->Info().Name;
				SubmitChange("VSComponent", oss.str(), true);

				for (const auto& child : comp->Slots()) {
					const std::string path = location + "." + child.first;
					submit(child.second, path);
				}
			};
	submit(RootComponent, "Root");
}

btVector3 Airship::GetForward() { return Double3(0, 0, 1) * Rotation; }

btVector3 Airship::GetUp() { return Double3(0, 1, 0) * Rotation; }

btVector3 Airship::GetRight() { return Double3(1, 0, 0) * Rotation; }
