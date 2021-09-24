#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

enum class VesselComponentType : uint16_t {
	SmallHull      = 100,
	SmallWing      = 101,
	SmallRudder    = 102,
	SmallBalloon   = 103,
	SmallEngine    = 104,
	SmallPropeller = 105,

	MediumHull      = 200,
	MediumWing      = 201,
	MediumRudder    = 202,
	MediumBalloon   = 203,
	MediumEngine    = 204,
	MediumPropeller = 205,

	LargeHull      = 300,
	LargeWing      = 301,
	LargeRudder    = 302,
	LargeBalloon   = 303,
	LargeEngine    = 304,
	LargePropeller = 305,

	GridAttachment = 1000
};

struct VesselComponentSlotInfo {
	VesselComponentSlotInfo(const std::string& name, bool required, const std::vector<VesselComponentType>& types)
			: Name(name), Required(required), AllowedTypes(types)
	{
	}

	const std::string Name;
	const bool Required;
	const std::vector<VesselComponentType> AllowedTypes;
};

struct VesselComponentInfo {
	static std::unordered_map<std::string, VesselComponentInfo> Registry;
	static void PopulateRegistry();
	static VesselComponentInfo Parse(const std::string& str);
	static const VesselComponentInfo& Get(const std::string& name);

	VesselComponentInfo(const std::string& name, VesselComponentType type, const std::vector<VesselComponentSlotInfo>& slots = {})
			: Name(name), Type(type), Slots(slots)
	{
	}

	const std::string Name;
	const VesselComponentType Type;
	const std::vector<VesselComponentSlotInfo> Slots;
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