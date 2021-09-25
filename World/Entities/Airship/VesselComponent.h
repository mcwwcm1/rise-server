#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "MysticMath/Double3.h"
#include "MysticMath/Quaternion.h"

enum class VesselComponentType : uint16_t {
	SmallHull      = 100,
	SmallWing      = 101,
	SmallRudder    = 102,
	SmallBalloon   = 103,
	SmallEngine    = 104,
	SmallPropeller = 105,
	SmallFlap      = 106,

	MediumHull      = 200,
	MediumWing      = 201,
	MediumRudder    = 202,
	MediumBalloon   = 203,
	MediumEngine    = 204,
	MediumPropeller = 205,
	MediumFlap      = 206,

	LargeHull      = 300,
	LargeWing      = 301,
	LargeRudder    = 302,
	LargeBalloon   = 303,
	LargeEngine    = 304,
	LargePropeller = 305,
	LargeFlap      = 306,

	GridAttachment = 1000
};

enum class ComponentColliderType : uint8_t { Box = 0, Sphere = 1, Capsule = 2, Cylinder = 3 };

struct VesselComponentSlotInfo {
	VesselComponentSlotInfo(const std::string& name, bool required, const std::vector<VesselComponentType>& types)
			: Name(name), Required(required), AllowedTypes(types)
	{
	}

	const std::string Name;
	const bool Required;
	const std::vector<VesselComponentType> AllowedTypes;
};

struct VesselComponentColliderInfo {
	VesselComponentColliderInfo(ComponentColliderType type, Double3 position, Quaternion rotation, Double3 scale, Double3 size)
			: Type(type), Position(position), Scale(scale), Size(size)
	{
	}

	const ComponentColliderType Type;
	Double3 Position;
	Quaternion Rotation;
	Double3 Scale;
	Double3 Size;
};

struct VesselComponentInfo {
	static std::unordered_map<std::string, VesselComponentInfo> Registry;
	static void PopulateRegistry();
	static VesselComponentInfo Parse(const std::string& str);
	static const VesselComponentInfo& Get(const std::string& name);

	VesselComponentInfo(const std::string& name,
	                    VesselComponentType type,
	                    const std::vector<VesselComponentSlotInfo>& slots         = {},
	                    const std::vector<VesselComponentColliderInfo>& colliders = {})
			: Name(name), Type(type), Slots(slots), Colliders(colliders)
	{
	}

	const std::string Name;
	const VesselComponentType Type;
	const std::vector<VesselComponentSlotInfo> Slots;
	const std::vector<VesselComponentColliderInfo> Colliders;
};

class VesselComponent
{
 public:
	VesselComponent(const VesselComponentInfo& info);

	const VesselComponentInfo& Info() const { return _info; }
	void SetSlot(const std::string& slotName, const std::shared_ptr<VesselComponent>& component);
	std::shared_ptr<VesselComponent>& GetSlot(const std::string& slotName);
	const std::unordered_map<std::string, std::shared_ptr<VesselComponent>>& Slots() const { return _slots; }

 private:
	const VesselComponentInfo& _info;
	std::unordered_map<std::string, std::shared_ptr<VesselComponent>> _slots;
};

class VesselTemplates
{
 public:
	static std::shared_ptr<VesselComponent> Starter();
};