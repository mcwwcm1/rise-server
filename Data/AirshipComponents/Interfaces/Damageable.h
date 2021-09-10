// File: Damageable.h
// Purpose: An interface for adding damage functionality to airship components

#pragma once

class Damageable
{
 public:
	virtual void DealDamage(float damage) = 0;
};