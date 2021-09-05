// File: distanceconstraint.h
// Purpose:

#include <bullet/btBulletDynamicsCommon.h>
#include "MysticMath/Double3.h"
#include "World/Entities/DynamicEntity.h"

class DistanceConstraint : public btPoint2PointConstraint
{
 protected:
	btScalar m_distance = 0.0;

 public:
	DistanceConstraint(DynamicEntity& entity1, DynamicEntity& entity2, const btVector3& pivotInA, const btVector3& pivotInB, btScalar dist)
			: btPoint2PointConstraint(*entity1.RigidBody, *entity2.RigidBody, pivotInA, pivotInB)
	{
		m_distance = dist;
	}
	virtual void getInfo1(btConstraintInfo1* info)
	{
		info->m_numConstraintRows = 1;
		info->nub                 = 5;
	}
	virtual void getInfo2(btConstraintInfo2* info)
	{
		btVector3 relA    = m_rbA.getCenterOfMassTransform().getBasis() * getPivotInA();
		btVector3 relB    = m_rbB.getCenterOfMassTransform().getBasis() * getPivotInB();
		btVector3 posA    = m_rbA.getCenterOfMassTransform().getOrigin() + relA;
		btVector3 posB    = m_rbB.getCenterOfMassTransform().getOrigin() + relB;
		btVector3 del     = posB - posA;
		btScalar currDist = btSqrt(del.dot(del));
		btScalar delta    = currDist - m_distance;
		btVector3 ortho   = del / currDist;
		btVector3 p, q;
		p = relA.cross(ortho);
		q = relB.cross(ortho);
		if (currDist >= m_distance) {
			if (!m_rbA.isStaticObject()) {
				info->m_J1linearAxis[0]  = ortho[0];
				info->m_J1linearAxis[1]  = ortho[1];
				info->m_J1linearAxis[2]  = ortho[2];
				info->m_J1angularAxis[0] = p[0];
				info->m_J1angularAxis[1] = p[1];
				info->m_J1angularAxis[2] = p[2];
			}
			if (!m_rbB.isStaticObject()) {
				info->m_J2linearAxis[0]  = -ortho[0];
				info->m_J2linearAxis[1]  = -ortho[1];
				info->m_J2linearAxis[2]  = -ortho[2];
				info->m_J2angularAxis[0] = -q[0];
				info->m_J2angularAxis[1] = -q[1];
				info->m_J2angularAxis[2] = -q[2];
			}
		}

		btScalar rhs               = delta * info->fps * info->erp;
		info->m_constraintError[0] = rhs;
		info->cfm[0]               = btScalar(0.f);
		info->m_lowerLimit[0]      = -SIMD_INFINITY;
		info->m_upperLimit[0]      = SIMD_INFINITY;
	}
};