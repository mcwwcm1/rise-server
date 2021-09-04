// File: HullComponent.h
// Purpose: defines the hull component of an airship

#include "RootComponent.h"
#include "Interfaces/Damageable.h"
#include <vector>

class HullComponent : public RootComponent, public Damageable
{
 public:
	float Health = 0;

	void DealDamage(float damage) override;
};