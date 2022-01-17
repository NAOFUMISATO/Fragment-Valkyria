
/*****************************************************************//**
 * \file   CollisionServer.cpp
 * \brief  
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "CollisionComponent.h"
#include "ModelAnimeComponent.h"
#include "ObjectBase.h"
#ifdef _DEBUG
#include <stdexcept>
#include <windows.h>
#endif

using namespace FragmentValkyria::Collision;

//namespace {
//	auto paramMap = AppFrame::Resource::LoadJson::GetParamMap("collision",
//		{ "fallObjectRange", "plyRadian", "plyCapsulePos1",
//		"plyCapsulePos2" });
//
//	const double FallObjectRange = paramMap["fallObjectRange"];     //!< �t�H�[���I�u�W�F�N�g�̋��̔��a
//	const double PlayerRadian = paramMap["plyRadian"];              //!< �v���C���[�̃J�v�Z���̔��a
//	const double PlayerCapsulePos1 = paramMap["plyCapsulePos1"];    //!< �v���C���[�̃J�v�Z�����`������2�_���̈�_�̍��W�܂ł̃v���C���[�̈ʒu����̋���
//	const double PlayerCapsulePos2 = paramMap["plyCapsulePos2"];    //!< �v���C���[�̃J�v�Z�����`������2�_���̈�_�̍��W�܂ł̃v���C���[�̈ʒu����̋���
//}

CollisionComponent::CollisionComponent(Object::ObjectBase& owner) : _owner{ owner } {
	_report = std::make_unique<Report>();
}

void CollisionComponent::ObjectRangeFromPlayer() {
	auto plyPoint = _owner.position();

	for (auto&& object : _owner.GetObjServer().runObjects()) {

		auto& objectBase = ObjectBaseCast(*object);

		if (objectBase.GetObjType() != Object::ObjectBase::ObjectType::FallObject) {
			continue;
		}
		/*if (objectBase.collisionComponent().report().id() == ReportId::HitFromPlayer) {
			continue;
		}*/

		auto objectPos = objectBase.position();
		auto objectRadian = /*_owner.GetLoadJson()*/300.0;
		AppFrame::Math::Sphere objectRange = std::make_tuple(objectPos, objectRadian);

		if (AppFrame::Math::Utility::CollisionSpherePoint(plyPoint, objectRange)) {
			objectBase.collisionComponent().report().id(ReportId::HitFromPlayer);
			break;
		}
		else {
			objectBase.collisionComponent().report().id(ReportId::None);
		}
	}
}

void CollisionComponent::PlayerFromObjectRange() {

	auto objectPos = _owner.position();
	auto objectRadian = 300.0;
	AppFrame::Math::Sphere objectRange = std::make_tuple(objectPos, objectRadian);

	for (auto&& object : _owner.GetObjServer().runObjects()) {

		auto& objectBase = ObjectBaseCast(*object);
		if (objectBase.collisionComponent().report().id() == ReportId::HitFromPlayer) {
			for (auto&& plyObject : _owner.GetObjServer().runObjects()) {
				auto& ply = ObjectBaseCast(*plyObject);
				if (ply.GetObjType() != Object::ObjectBase::ObjectType::Player) {
					continue;
				}
				if (objectBase.collisionComponent().report().id() == ReportId::HitFromIdleFallObject) {
					break;
				}
				ply.collisionComponent().report().id(ReportId::HitFromObjectRange);
			}
			break;
		}
		if (objectBase.GetObjType() != Object::ObjectBase::ObjectType::Player) {
			continue;
		}
		if (objectBase.collisionComponent().report().id() == ReportId::HitFromIdleFallObject) {
			break;
		}

		auto plyPoint = objectBase.position();
		if (AppFrame::Math::Utility::CollisionSpherePoint(plyPoint, objectRange)) {
			objectBase.collisionComponent().report().id(ReportId::HitFromObjectRange);
		}
		else {
			objectBase.collisionComponent().report().id(ReportId::None);
		}
	}
}

void FragmentValkyria::Collision::CollisionComponent::PlayerFromObjectModel() {
	auto objectModel = _owner.modelAnimeComponent().modelHandle();
	auto collision = MV1SearchFrame(objectModel, "drum_c");

	for (auto&& object : _owner.GetObjServer().runObjects()) {

		auto& objectBase = ObjectBaseCast(*object);
		if (objectBase.GetObjType() != Object::ObjectBase::ObjectType::Player) {
			continue;
		}
		if (objectBase.collisionComponent().report().id() == ReportId::HitFromIdleFallObject) {
			continue;
		}
		// �v���C���[���̃J�v�Z����ݒ聦�������̓t���[���֌W�Ȃ��K����
		auto plyPos = objectBase.position();
		auto pos1 = plyPos + AppFrame::Math::Vector4(0.0, 30.0, 0.0);
		plyPos.Add(0.0, 60.0, 0.0);
		auto radian = static_cast<float>(30.0);
		
		auto result = MV1CollCheck_Capsule(objectModel, collision, AppFrame::Math::ToDX(plyPos), AppFrame::Math::ToDX(plyPos), radian);

		if (result.HitNum > 0) {
			objectBase.collisionComponent().hitPos(_owner.position());
			objectBase.collisionComponent().report().id(ReportId::HitFromIdleFallObject);
		}
	}

}

FragmentValkyria::Object::ObjectBase& CollisionComponent::ObjectBaseCast(AppFrame::Object::ObjectBaseRoot& obj) {
#ifndef _DEBUG
	auto& objectBase = dynamic_cast<Object::ObjectBase&>(obj);

	return objectBase;
	
#else
	try {
		auto& objectBase = dynamic_cast<Object::ObjectBase&>(obj);

		return objectBase;
	}
	catch (std::bad_cast&) {
		OutputDebugString("----------�_�E���L���X�g���s----------");
	}
#endif

	return dynamic_cast<Object::ObjectBase&>(obj);
}
