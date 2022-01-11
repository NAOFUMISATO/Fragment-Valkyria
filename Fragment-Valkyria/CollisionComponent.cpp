
/*****************************************************************//**
 * \file   CollisionServer.cpp
 * \brief  
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "CollisionComponent.h"
#include "ObjectBase.h"

using namespace FragmentValkyria::Collision;

CollisionComponent::CollisionComponent(Object::ObjectBase& owner) : _owner{ owner } {
	_report = std::make_unique<Report>();
}

void CollisionComponent::ObjectRangeFromPlayer() {
	auto plyPoint = _owner.position();

	for (auto&& object : _owner.GetObjServer().runObjects()) {
		auto& objectBase = dynamic_cast<Object::ObjectBase&>(*object);

		if (objectBase.GetObjType() != Object::ObjectBase::ObjectType::FallObject) {
			continue;
		}
		if (objectBase.collisionComponent().report().id() == ReportId::HitFromPlayer) {
			continue;
		}

		auto objectPos = objectBase.position();
		auto objectRadian = /*_owner.GetLoadJson()*/100.0;
		AppFrame::Math::Sphere objectRange = std::make_tuple(objectPos, objectRadian);

		if (AppFrame::Math::Utility::CollisionSpherePoint(plyPoint, objectRange)) {
			objectBase.collisionComponent().report().id(ReportId::HitFromPlayer);
			break;
		}
	}
}
