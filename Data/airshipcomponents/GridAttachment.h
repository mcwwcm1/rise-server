// File: GridAttachment.h
// Purpose: For placing arbitrary sized objects on airships :D

#include "AirshipComponent.h"

struct GridSpace {
 public:
	unsigned int PosX, PosY;
};

class GridAttachment : public AirshipComponent
{
 public:
	std::vector<GridSpace> GridSpaces;
};