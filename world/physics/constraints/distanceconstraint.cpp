// File: distanceconstraint.cpp
// Purpose: Implements distanceconstraint.h

#include "distanceconstraint.h"

#include "../../entities/dynamicentity.h"

DistanceConstraint::DistanceConstraint(string id) : Constraint(id) {}

void DistanceConstraint::ApplyConstraint(DynamicEntity* entity,
                                         Movement* movement)
{
	Double3 globalAttachmentPointFrom =
			AttachmentPoint * movement->FromRotation + movement->FromPosition;
	Double3 globalAttachmentPointTo =
			AttachmentPoint * movement->ToRotation + movement->ToPosition;

	Double3 globalAttachmentPointTarget =
			TargetEntity ? TargetEntity->LocalPointToGlobal(TargetPoint)
									 : TargetPoint;

	Double3 delta = globalAttachmentPointTo - globalAttachmentPointTarget;

	if (delta.MagnitudeSquared() > Distance * Distance) {
		// Extending
		if (IsRigid) {
			Double3 displacement =
					(delta.Normalized() * (Distance - delta.Magnitude()));

			DynamicEntity* dynamicEntity2 =
					dynamic_cast<DynamicEntity*>(TargetEntity);
			if (dynamicEntity2 != nullptr) {
				// Cast succeeded, we have a dynamic entity

				float massRatio = entity->Mass / dynamicEntity2->Mass;

				float displacementRatio = 1 / (massRatio + 1);  // :D

				movement->ToPosition += displacement * displacementRatio;
				dynamicEntity2->Position -= displacement * (1 - displacementRatio);

				Double3 relativeVelocity = entity->Velocity - dynamicEntity2->Velocity;

				Double3 tugDirection = delta.Normalized();

				entity->AddImpulseForceAtPosition(
						relativeVelocity *
								Dot(relativeVelocity.Normalized(), tugDirection * -1) /
								massRatio,
						globalAttachmentPointFrom);
				dynamicEntity2->AddImpulseForceAtPosition(
						relativeVelocity *
								Dot(relativeVelocity.Normalized(), tugDirection) * massRatio,
						globalAttachmentPointTarget);
			} else {
				movement->ToPosition += displacement;
				entity->AddImpulseForceAtPosition(
						entity->Velocity *
								Dot(entity->Velocity.Normalized(), displacement.Normalized()),
						globalAttachmentPointFrom);
			}
		} else {
			// Apply tensile force
			entity->AddForceAtPosition(
					delta * (Distance - delta.Magnitude()) * TensileForce,
					globalAttachmentPointFrom);
		}
	}
}