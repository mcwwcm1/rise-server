#include "VesselComponent.h"
#include "Core/Commands.h"
#include "Utilities.h"

#include <stdexcept>

std::unordered_map<std::string, VesselComponentInfo> VesselComponentInfo::Registry;

void VesselComponentInfo::PopulateRegistry()
{
	const std::string componentRegistry[] = {
			"StarterHull|100|10|True|Balloon|2|103|201|True|Engine|1|104|True|Wing|2|101|201|False|Grid|1|1000|True|Rudder|1|102|True|Flap|1|106|True|Wing1|2|101|201|True|Flap1|1|106|True|Propeller|1|105|True|Propeller1|1|105|2|0|[0; 0; 1.086504]|[1.282727E-10; -1.715759E-08; 5.346439E-10; 1]|[0.006503899; 0.0125; 0.03338389]|[511.6618; 45.26403; 149.105]|3|[0; 0; -1.219823]|[-8.821305E-11; -1.839161E-08; 3.507461E-10; 1]|[0.0207738; 0.0125; 0.02906088]|[80.52649; 45.26403; 67.1109]|",
			"StarterEngine|104|0|0|",
			"StarterBalloon|103|0|0|",
			"StarterWing|101|0|0|",
			"StarterRudder|102|0|0|",
			"StarterPropeller|105|0|0|",
			"StarterFlap|106|0|0"};
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
	std::vector<VesselComponentColliderInfo> colliders;

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
		const ComponentColliderType colliderType = static_cast<ComponentColliderType>(parse.Get<uint32_t>());
		printf("- Type: %d\n", static_cast<int>(colliderType));
		const Double3 colliderPosition = parse.Get<Double3>();
		printf("- Position: %s\n", colliderPosition.ToString().c_str());
		const Quaternion colliderRotation = parse.Get<Quaternion>();
		printf("- Rotation: %s\n", colliderRotation.ToString().c_str());
		const Double3 colliderScale = parse.Get<Double3>();
		printf("- Scale: %s\n", colliderScale.ToString().c_str());
		const Double3 colliderSize = parse.Get<Double3>();
		printf("- Size: %s\n", colliderSize.ToString().c_str());

		colliders.emplace_back(colliderType, colliderPosition, colliderRotation, colliderScale, colliderSize);
	}

	return VesselComponentInfo(componentName, componentType, componentSlots, colliders);
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
	hull->SetSlot("Wing1", std::make_shared<VesselComponent>(VesselComponentInfo::Get("StarterWing")));
	hull->SetSlot("Flap", std::make_shared<VesselComponent>(VesselComponentInfo::Get("StarterFlap")));
	hull->SetSlot("Flap1", std::make_shared<VesselComponent>(VesselComponentInfo::Get("StarterFlap")));
	hull->SetSlot("Rudder", std::make_shared<VesselComponent>(VesselComponentInfo::Get("StarterRudder")));
	hull->SetSlot("Propeller", std::make_shared<VesselComponent>(VesselComponentInfo::Get("StarterPropeller")));
	hull->SetSlot("Propeller1", std::make_shared<VesselComponent>(VesselComponentInfo::Get("StarterPropeller")));

	return hull;
}