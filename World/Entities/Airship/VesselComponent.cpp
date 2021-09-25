#include "VesselComponent.h"
#include "Core/Commands.h"
#include "Utilities.h"

#include <stdexcept>

std::unordered_map<std::string, VesselComponentInfo> VesselComponentInfo::Registry;

void VesselComponentInfo::PopulateRegistry()
{
	const std::string componentRegistry[] = {
		"StarterHull|100|6|True|Engine|1|104|True|Balloon|2|103|203|False|Grid|1|1000|False|Wing|2|101|201|False|Rudder|1|102|False|Propeller|2|105|205|2|0|[0.1246022; -1.378965; 86.92015]|[0; 0; 0; 1]|[0.5203119; 1.499606; 2.670711]|[511.6618; 31.29639; 149.105]|3|[0.01932025; -0.763169; -97.58627]|[0; 0; 0; 1]|[1.661904; 1; 2.32487]|[80.52649; 45.26403; 67.1109]|",
		"StarterEngine|104|0|0|",
		"StarterBalloon|103|0|0|",
		"StarterWing|101|0|0|",
		"StarterRudder|102|0|0|",
		"StarterPropeller|105|0|0|"
	};
	for (const auto& componentString : componentRegistry) {
		const auto vesselComponent = VesselComponentInfo::Parse(componentString);
		Registry.insert({vesselComponent.Name, vesselComponent});
	}
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
		const std::string slotName = parse.Get<std::string>();
		printf("- Name: %s\n", slotName.c_str());
		std::vector<VesselComponentType> types(parse.Get<uint32_t>());
		for (size_t j = 0; j < types.size(); ++j) {
			types[j] = static_cast<VesselComponentType>(parse.Get<uint32_t>());
			printf("- Type: %d\n", static_cast<uint32_t>(types[j]));
		}
		componentSlots.emplace_back(slotName, required, types);
	}

	const int colliderCount = parse.Get<uint32_t>();
	printf("Colliders: %d\n", colliderCount);
	for (size_t i = 0; i < colliderCount; ++i) {
		const int colliderType = parse.Get<uint32_t>();
		printf("- Type: %d\n", colliderType);
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