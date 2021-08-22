// File: sphereShape.h
// Purpose: Defines a sphere shape for collision

#include "../../../mysticmath/double3.h"
#include "../../../mysticmath/matrix4x4.h"
#include "shape.h"

struct SphereShape : Shape {
 public:
  SphereShape(float radius);
  SphereShape(Double3 size);
  Double3 size;
  virtual Double3 getFurthestPointInDirection(const Double3& direction);
};