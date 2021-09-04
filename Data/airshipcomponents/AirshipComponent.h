// File: AirshipComponent.h
// Purpose: The base type for airship components (Hulls, wings, engines, etc...)

#include "mysticmath/double3.h"
#include "mysticmath/quaternion.h"
#include "btBulletDynamicsCommon.h"
#include <vector>

enum ComponentType {
	SmallHull,
	SmallWing,
	SmallRudder,
	SmallBalloon,
	SmallEngine,
	SmallPropeller,
	GridAttachment
};

struct ComponentSlot {
 public:
	ComponentSlot(Double3 position, Quaternion rotation, std::vector<ComponentType> allowedTypes) : Position(position), Rotation(rotation), AllowedTypes(allowedTypes){};

	Double3 Position;
	Quaternion Rotation;
	std::vector<ComponentType> AllowedTypes;
	std::vector<AirshipComponent> Components;
};

class AirshipComponent
{
 public:
	std::vector<ComponentSlot> Slots;
	btCompoundShape* CollissionShape;
};