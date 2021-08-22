// File: distanceconstraint.cpp
// Purpose: Implements distanceconstraint.h

#include "distanceconstraint.h"

#include "../../entities/dynamicentity.h"

DistanceConstraint::DistanceConstraint(string id) : Constraint(id) {}

void DistanceConstraint::ApplyConstraint(DynamicEntity* entity,
                                         Movement* movement)
{
	Double3 globalAttachmentPointFrom =
			attachmentPoint * movement->fromRotation + movement->fromPosition;
	Double3 globalAttachmentPointTo =
			attachmentPoint * movement->toRotation + movement->toPosition;

	Double3 globalAttachmentPointTarget =
			targetEntity ? targetEntity->LocalPointToGlobal(targetPoint)
									 : targetPoint;

	Double3 delta = globalAttachmentPointTo - globalAttachmentPointTarget;

	if (delta.magnitudeSquared() > distance * distance) {
		// Extending
		if (isRigid) {
			Double3 displacement =
					(delta.normalized() * (distance - delta.magnitude()));

			DynamicEntity* dynamicEntity2 =
					dynamic_cast<DynamicEntity*>(targetEntity);
			if (dynamicEntity2 != nullptr) {
				// Cast succeeded, we have a dynamic entity

				float massRatio = entity->mass / dynamicEntity2->mass;

				float displacementRatio = 1 / (massRatio + 1);  // :D

				movement->toPosition += displacement * displacementRatio;
				dynamicEntity2->position -= displacement * (1 - displacementRatio);

				Double3 relativeVelocity = entity->velocity - dynamicEntity2->velocity;

				Double3 tugDirection = delta.normalized();

				entity->AddImpulseForceAtPosition(
						relativeVelocity *
								dot(relativeVelocity.normalized(), tugDirection * -1) /
								massRatio,
						globalAttachmentPointFrom);
				dynamicEntity2->AddImpulseForceAtPosition(
						relativeVelocity *
								dot(relativeVelocity.normalized(), tugDirection) * massRatio,
						globalAttachmentPointTarget);
			} else {
				movement->toPosition += displacement;
				entity->AddImpulseForceAtPosition(
						entity->velocity *
								dot(entity->velocity.normalized(), displacement.normalized()),
						globalAttachmentPointFrom);
			}
		} else {
			// Apply tensile force
			entity->AddForceAtPosition(
					delta * (distance - delta.magnitude()) * tensileForce,
					globalAttachmentPointFrom);
		}
	}
}