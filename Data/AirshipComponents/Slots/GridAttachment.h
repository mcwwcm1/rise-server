// File: GridAttachment.h
// Purpose: For placing arbitrary sized objects on airships :D

#pragma once

#include "ComponentSlot.h"

struct GridSpace {
 public:
	unsigned int PosX, PosY;
};

class GridAttachment : public ComponentSlot
{
 public:
	std::vector<GridSpace> GridSpaces;
};