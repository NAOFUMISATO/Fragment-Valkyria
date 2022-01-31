
/*****************************************************************//**
 * \file   CollisionServer.cpp
 * \brief  �����蔻�菈�����s���N���X
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "CollisionComponent.h"
#include "ModelAnimeComponent.h"
#include "Gatling.h"
#include "ObjectBase.h"
#ifdef _DEBUG
#include <stdexcept>
#include <windows.h>
#endif

using namespace FragmentValkyria::Collision;

namespace {
	auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("collision",
		{ "fallObjectRange", "plyRadian", "plyCapsulePos1",
		"plyCapsulePos2", "gatlingRadian", "fallObjectCapsulePos1",
		"fallObjectCapsulePos2", "fallObjectRadian" });

	const double FallObjectRange = paramMap["fallObjectRange"];     //!< �t�H�[���I�u�W�F�N�g�̋��̔��a
	const double PlayerRadian = paramMap["plyRadian"];              //!< �v���C���[�̃J�v�Z���̔��a
	const double PlayerCapsulePos1 = paramMap["plyCapsulePos1"];    //!< �v���C���[�̃J�v�Z�����`������2�_���̈�_�̍��W�܂ł̃v���C���[�̈ʒu����̋���
	const double PlayerCapsulePos2 = paramMap["plyCapsulePos2"];    //!< �v���C���[�̃J�v�Z�����`������2�_���̈�_�̍��W�܂ł̃v���C���[�̈ʒu����̋���
	const double GatlingRadian = paramMap["gatlingRadian"];         //!< �K�g�����O�̔��a
	const double FallObjectCapsulePos1 = paramMap["fallObjectCapsulePos1"];         //!< �t�H�[���I�u�W�F�N�g�̃J�v�Z�����`������2�_���̈�_�̍��W�܂ł̃t�H�[���I�u�W�F�N�g�̈ʒu����̋���
	const double FallObjectCapsulePos2 = paramMap["fallObjectCapsulePos2"];         //!< �t�H�[���I�u�W�F�N�g�̃J�v�Z�����`������2�_���̈�_�̍��W�܂ł̃t�H�[���I�u�W�F�N�g�̈ʒu����̋���
	const double FallObjectRadian = paramMap["fallObjectRadian"];         //!< �t�H�[���I�u�W�F�N�g�̃J�v�Z���̔��a
}

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
		auto objectRadian = FallObjectRange/*300.0*/;
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
				if (objectBase.collisionComponent().report().id() == ReportId::HitFromFallObject) {
					break;
				}
				if (objectBase.collisionComponent().report().id() == ReportId::HitFromGatling) {
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
		if (objectBase.collisionComponent().report().id() == ReportId::HitFromFallObject) {
			break;
		}
		if (objectBase.collisionComponent().report().id() == ReportId::HitFromGatling) {
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

void CollisionComponent::PlayerFromFallObjectModel(bool fall) {
	auto objectModel = _owner.modelAnimeComponent().modelHandle();
	auto collision = MV1SearchFrame(objectModel, "drum_green_c");

	for (auto&& object : _owner.GetObjServer().runObjects()) {

		auto& objectBase = ObjectBaseCast(*object);
		if (objectBase.GetObjType() != Object::ObjectBase::ObjectType::Player) {
			continue;
		}
		if (objectBase.collisionComponent().report().id() == ReportId::HitFromIdleFallObject) {
			continue;
		}
		// �v���C���[���̃J�v�Z����ݒ�
		auto plyPos = objectBase.position();
		auto pos1 = plyPos + AppFrame::Math::Vector4(0.0, PlayerCapsulePos1, 0.0);
		plyPos.Add(0.0, PlayerCapsulePos2/*60.0*/, 0.0);
		auto radian = static_cast<float>(PlayerRadian/*30.0*/);
		
		auto result = MV1CollCheck_Capsule(objectModel, collision, AppFrame::Math::ToDX(plyPos), AppFrame::Math::ToDX(plyPos), radian);

		if (result.HitNum > 0) {
			if (fall) {
				objectBase.collisionComponent().hitPos(_owner.position());
				objectBase.collisionComponent().report().id(ReportId::HitFromFallObject);

				objectBase.collisionComponent().damage(20.0);
			}
			else {
				objectBase.collisionComponent().hitPos(AppFrame::Math::ToMath(result.Dim[0].Normal));
				objectBase.collisionComponent().report().id(ReportId::HitFromIdleFallObject);
			}
			
		}
	}

}

void CollisionComponent::GatlingFromObjectModel() {
	auto objectModel = _owner.modelAnimeComponent().modelHandle();
	auto collision = MV1SearchFrame(objectModel, "drum_green_c");

	for (auto&& object : _owner.GetObjServer().runObjects()) {

		auto& objectBase = ObjectBaseCast(*object);
		if (objectBase.GetObjType() != Object::ObjectBase::ObjectType::Gatling) {
			continue;
		}
		// �K�g�����O���̋���ݒ�
		auto gatlingPos = objectBase.position();
		auto radian = static_cast<float>(GatlingRadian);

		auto result = MV1CollCheck_Sphere(objectModel, collision, AppFrame::Math::ToDX(gatlingPos), radian);

		if (result.HitNum > 0) {
			objectBase.collisionComponent().hitPos(_owner.position());
			objectBase.collisionComponent().report().id(ReportId::HitFromIdleFallObject);
		}
	}

}

void CollisionComponent::GatlingFromPlayer() {
	auto playerPos = _owner.position();
	auto capsulePos1 = playerPos + Vector4(0.0, 30.0, 0.0);
	auto capsulePos2 = playerPos + Vector4(0.0, 60.0, 0.0);
	auto casuleRadian = PlayerRadian/*30.0*/;

	AppFrame::Math::Capsule plyCapsule = std::make_tuple(capsulePos1, capsulePos2, casuleRadian);

	for (auto&& object : _owner.GetObjServer().runObjects()) {

		auto& objectBase = ObjectBaseCast(*object);
		
		if (objectBase.GetObjType() != Object::ObjectBase::ObjectType::Gatling) {
			continue;
		}

		auto gatling = objectBase.position();
		auto gatlingRadian = GatlingRadian/*100.0*/;

		AppFrame::Math::Sphere gatlingSphere = std::make_tuple(gatling, gatlingRadian);
		if (AppFrame::Math::Utility::CollisionCapsuleSphere(plyCapsule, gatlingSphere)) {
			objectBase.collisionComponent().report().id(ReportId::HitFromPlayer);
			_owner.collisionComponent().report().id(ReportId::HitFromGatling);
			_owner.collisionComponent().damage(20.0);
			_owner.collisionComponent().hitPos(objectBase.position());
		}
		else {
			objectBase.collisionComponent().report().id(ReportId::None);
		}
}

}

void CollisionComponent::ObjectModelFromLargeEnemy() {
	auto fallObjectPos = _owner.position();
	auto capsulePos1 = fallObjectPos + Vector4(0.0, FallObjectCapsulePos1, 0.0);
	auto capsulePos2 = fallObjectPos + Vector4(0.0, FallObjectCapsulePos2, 0.0);
	auto capsuleRadian = static_cast<float>(FallObjectRadian);

	for (auto&& object : _owner.GetObjServer().runObjects()) {

		auto& objectBase = ObjectBaseCast(*object);

		if (objectBase.GetObjType() != Object::ObjectBase::ObjectType::LargeEnemy) {
			continue;
		}

		auto largeEnemyModel = objectBase.modelAnimeComponent().modelHandle();
		auto collision = objectBase.modelAnimeComponent().FindFrame("S301_typeCO");

		auto result = MV1CollCheck_Capsule(largeEnemyModel, collision, AppFrame::Math::ToDX(capsulePos1), AppFrame::Math::ToDX(capsulePos2), capsuleRadian);

		if (result.HitNum > 0) {
			objectBase.collisionComponent().report().id(ReportId::HitFromFallObject);
			objectBase.collisionComponent().damage(20.0);
			_owner.collisionComponent().report().id(ReportId::HitFromLargeEnemy);
			_owner.collisionComponent().hitPos(objectBase.position());
		}
    }

}

void CollisionComponent::LargeEnemyFromBullet() {
	auto bulletPos = _owner.position();
	auto bulletRadius = static_cast<float>(20.0);

	for (auto&& object : _owner.GetObjServer().runObjects()) {

		auto& objectBase = ObjectBaseCast(*object);

		if (objectBase.GetObjType() != Object::ObjectBase::ObjectType::LargeEnemy) {
			continue;
		}

		auto largeEnemyModel = objectBase.modelAnimeComponent().modelHandle();
		auto collision = objectBase.modelAnimeComponent().FindFrame("S301_typeCO");

		auto result = MV1CollCheck_Sphere(largeEnemyModel, collision, AppFrame::Math::ToDX(bulletPos), bulletRadius);

		if (result.HitNum > 0) {
			objectBase.collisionComponent().report().id(ReportId::HitFromBullet);
			objectBase.collisionComponent().damage(10.0);
			_owner.collisionComponent().report().id(ReportId::HitFromLargeEnemy);
			_owner.collisionComponent().hitPos(objectBase.position());
		}
	}

}

void CollisionComponent::PlayerKnockBack() {
	for (auto&& object : _owner.GetObjServer().runObjects()) {

		auto& objectBase = ObjectBaseCast(*object);

		if (objectBase.GetObjType() != Object::ObjectBase::ObjectType::Player) {
			continue;
		}
		if (objectBase.collisionComponent().knockBack()) {
			_owner.collisionComponent().knockBack(true);
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
