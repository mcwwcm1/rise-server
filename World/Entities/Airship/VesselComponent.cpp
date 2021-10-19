#include "VesselComponent.h"
#include "Core/Commands.h"
#include "Utilities.h"

#include <stdexcept>

std::unordered_map<std::string, VesselComponentInfo> VesselComponentInfo::Registry;

void VesselComponentInfo::PopulateRegistry()
{
	const std::string componentRegistry[] = {
			"StarterHull|100|9|True|Balloon|[2.082216E-06; 6.952594; 0.2018468]|[-0.7071069; 2.184212E-08; 2.184212E-08; 0.7071068]|[1; 1; 1]|2|103|201|True|Engine|[-0.005120992; -0.01806281; 2.926454]|[-0.7071069; 2.184212E-08; 2.184212E-08; 0.7071068]|[0.4; 0.4; 0.4]|1|104|True|Wing|[1.64334; -0.1503086; 0.8533136]|[-0.7071069; 2.191218E-08; 2.191217E-08; 0.7071067]|[-1; 1; 1]|2|101|201|True|Rudder|[0; -0.4528786; 2.817787]|[-0.7071069; 2.60824E-08; 2.608239E-08; 0.7071068]|[1; 1; 1]|1|102|True|Flap|[1.643339; -0.1533522; 2.879015]|[-0.7071067; -1.15628E-07; 1.00501E-10; 0.7071069]|[-1; 1; 1]|1|106|True|Wing1|[-1.64334; -0.1503086; 0.8533152]|[-0.7071069; 2.191218E-08; 2.191217E-08; 0.7071067]|[1; 1; 1]|2|101|201|True|Flap1|[-1.643339; -0.153356; 2.879017]|[-0.7071069; 2.191218E-08; 2.191217E-08; 0.7071067]|[1; 1; 1]|1|106|True|Propeller|[-1.014045; -0.7297491; 3.053618]|[-0.7071069; 2.191218E-08; 2.191217E-08; 0.7071067]|[1; 1; 1]|1|105|True|Propeller1|[1.014044; -0.7297491; 3.053616]|[-0.7071069; 2.191218E-08; 2.191217E-08; 0.7071067]|[1; 1; 1]|1|105|2|0|[0; 0; 1.086504]|[1.282727E-10; -1.715759E-08; 5.346439E-10; 1]|[0.006503899; 0.0125; 0.03338389]|[511.6618; 45.26403; 149.105]|3|[0; 0; -1.219823]|[-8.821305E-11; -1.839161E-08; 3.507461E-10; 1]|[0.0207738; 0.0125; 0.02906088]|[80.52649; 45.26403; 67.1109]|",
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
		const Double3 slotPosition = parse.Get<Double3>();
		printf("- Position: %s\n", slotPosition.ToString().c_str());
		const Quaternion slotRotation = parse.Get<Quaternion>();
		printf("- Rotation: %s\n", slotRotation.ToString().c_str());
		const Double3 slotScale = parse.Get<Double3>();
		printf("- Scale: %s\n", slotScale.ToString().c_str());
		std::vector<VesselComponentType> types(parse.Get<uint32_t>());
		for (size_t j = 0; j < types.size(); ++j) {
			types[j] = static_cast<VesselComponentType>(parse.Get<uint32_t>());
			printf("- Type: %d\n", static_cast<uint32_t>(types[j]));
		}
		componentSlots.emplace_back(slotName, required, types, slotPosition, slotRotation, slotScale);
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