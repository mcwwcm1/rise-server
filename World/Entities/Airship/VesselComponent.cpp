#include "VesselComponent.h"
#include "Core/Commands.h"
#include "Utilities.h"

#include <stdexcept>

std::unordered_map<std::string, VesselComponentInfo> VesselComponentInfo::Registry;

void VesselComponentInfo::PopulateRegistry()
{
	const auto starterHull = VesselComponentInfo::Parse(
			"StarterHull|100|6|True|1|104|True|2|103|203|False|1|1000|False|2|101|201|False|1|102|False|2|105|205|2|0|[0.1246022; -1.378965; 86.92015]|[0; 0; 0; 1]|[0.5203119; 1.499606; 2.670711]|[511.6618; 31.29639; 149.105]|3|[0.01932025; -0.763169; -97.58627]|[0; 0; 0; 1]|[1.661904; 1; 2.32487]|[80.52649; 45.26403; 67.1109]|");
	//Registry.insert({"StarterHull", starterHull});

	Registry.insert(
			{"StarterHull",
	     VesselComponentInfo(
					 "StarterHull",
					 VesselComponentType::SmallHull,
					 {VesselComponentSlotInfo("Engine", true, {VesselComponentType::SmallEngine}),
	          VesselComponentSlotInfo("Balloon", true, {VesselComponentType::SmallBalloon, VesselComponentType::MediumBalloon}),
	          VesselComponentSlotInfo("Grid", false, {VesselComponentType::GridAttachment}),
	          VesselComponentSlotInfo("Wing", false, {VesselComponentType::SmallWing, VesselComponentType::MediumWing}),
	          VesselComponentSlotInfo("Rudder", false, {VesselComponentType::SmallRudder}),
	          VesselComponentSlotInfo("Propeller", false, {VesselComponentType::SmallPropeller})})});

	Registry.insert({"StarterEngine", VesselComponentInfo("StarterEngine", VesselComponentType::SmallEngine)});
	Registry.insert({"StarterBalloon", VesselComponentInfo("StarterBalloon", VesselComponentType::SmallBalloon)});
	Registry.insert({"StarterWing", VesselComponentInfo("StarterWing", VesselComponentType::SmallWing)});
	Registry.insert({"StarterRudder", VesselComponentInfo("StarterRudder", VesselComponentType::SmallRudder)});
	Registry.insert({"StarterPropeller", VesselComponentInfo("StarterPropeller", VesselComponentType::SmallPropeller)});
}

VesselComponentInfo VesselComponentInfo::Parse(const std::string& str)
{
	Parser parse(str);

	const std::string componentName         = parse.Get<std::string>();
	const VesselComponentType componentType = static_cast<VesselComponentType>(parse.Get<uint32_t>());
	std::vector<VesselComponentSlotInfo> componentSlots;

	printf("Name: %s\n", componentName.c_str());
	printf("Type: %d\n", static_cast<uint32_t>(componentType));

	const int slotCount = parse.Get<uint32_t>();
	printf("Slots: %d\n", slotCount);
	for (size_t i = 0; i < slotCount; ++i) {
		const bool required = parse.Get<bool>();
		printf("- Required: %s\n", required ? "True" : "False");
		std::vector<VesselComponentType> types(parse.Get<uint32_t>());
		for (size_t j = 0; j < types.size(); ++j) {
			types[j] = static_cast<VesselComponentType>(parse.Get<uint32_t>());
			printf("- Type: %d\n", static_cast<uint32_t>(types[j]));
		}
		componentSlots.emplace_back("DummyName", required, types);
	}

	return VesselComponentInfo(componentName, componentType, componentSlots);
}

const VesselComponentInfo& VesselComponentInfo::Get(const std::string& name)
{
	const auto it = Registry.find(name);
	if (it == Registry.end()) { throw std::runtime_error("Provided VesselComponentType has no info!"); }

	return it->second;
}

VesselComponent::VesselComponent(const VesselComponentInfo& info) : _info(info) {}

void VesselComponent::SetSlot(const std::string& slotName, const std::shared_ptr<VesselComponent>& component)
{
	for (const auto& slotInfo : _info.Slots) {
		if (slotInfo.Name == slotName) {
			_slots[slotName] = component;
			return;
		}
	}

	throw std::invalid_argument("Unknown slot name!");
}

std::shared_ptr<VesselComponent>& VesselComponent::GetSlot(const std::string& slotName)
{
	for (const auto& slotInfo : _info.Slots) {
		if (slotInfo.Name == slotName) { return _slots[slotName]; }
	}

	throw std::invalid_argument("Unknown slot name!");
}

std::shared_ptr<VesselComponent> VesselTemplates::Starter()
{
	std::shared_ptr<VesselComponent> hull = std::make_shared<VesselComponent>(VesselComponentInfo::Get("StarterHull"));
	hull->SetSlot("Balloon", std::make_shared<VesselComponent>(VesselComponentInfo::Get("StarterBalloon")));
	hull->SetSlot("Engine", std::make_shared<VesselComponent>(VesselComponentInfo::Get("StarterEngine")));
	hull->SetSlot("Wing", std::make_shared<VesselComponent>(VesselComponentInfo::Get("StarterWing")));
	hull->SetSlot("Rudder", std::make_shared<VesselComponent>(VesselComponentInfo::Get("StarterRudder")));
	hull->SetSlot("Propeller", std::make_shared<VesselComponent>(VesselComponentInfo::Get("StarterPropeller")));

	return hull;
}