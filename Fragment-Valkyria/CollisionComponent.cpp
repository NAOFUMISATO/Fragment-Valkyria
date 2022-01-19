
/*****************************************************************//**
 * \file   CollisionServer.cpp
 * \brief  当たり判定処理を行うクラス
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
//	const double FallObjectRange = paramMap["fallObjectRange"];     //!< フォールオブジェクトの球の半径
//	const double PlayerRadian = paramMap["plyRadian"];              //!< プレイヤーのカプセルの半径
//	const double PlayerCapsulePos1 = paramMap["plyCapsulePos1"];    //!< プレイヤーのカプセルを形成する2点中の一点の座標までのプレイヤーの位置からの距離
//	const double PlayerCapsulePos2 = paramMap["plyCapsulePos2"];    //!< プレイヤーのカプセルを形成する2点中の一点の座標までのプレイヤーの位置からの距離
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
				else if (objectBase.collisionComponent().report().id() == ReportId::HitFromGatling) {
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
		// プレイヤー側のカプセルを設定
		auto plyPos = objectBase.position();
		auto pos1 = plyPos + AppFrame::Math::Vector4(0.0, 30.0, 0.0);
		plyPos.Add(0.0, 60.0, 0.0);
		auto radian = static_cast<float>(30.0);
		
		auto result = MV1CollCheck_Capsule(objectModel, collision, AppFrame::Math::ToDX(plyPos), AppFrame::Math::ToDX(plyPos), radian);

		if (result.HitNum > 0) {
			objectBase.collisionComponent().hitPos(AppFrame::Math::ToMath(result.Dim[0].Normal));
			objectBase.collisionComponent().report().id(ReportId::HitFromIdleFallObject);
			
		}
	}

}

void CollisionComponent::GatlingFromObjectModel() {
	auto objectModel = _owner.modelAnimeComponent().modelHandle();
	auto collision = MV1SearchFrame(objectModel, "drum_c");

	for (auto&& object : _owner.GetObjServer().runObjects()) {

		auto& objectBase = ObjectBaseCast(*object);
		if (objectBase.GetObjType() != Object::ObjectBase::ObjectType::Gatling) {
			continue;
		}
		// ガトリング側の球を設定
		auto gatlingPos = objectBase.position();
		auto radian = static_cast<float>(100.0);

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
	auto casuleRadian = 30.0;

	AppFrame::Math::Capsule plyCapsule = std::make_tuple(capsulePos1, capsulePos2, casuleRadian);

	for (auto&& object : _owner.GetObjServer().runObjects()) {

		auto& objectBase = ObjectBaseCast(*object);
		
		if (objectBase.GetObjType() != Object::ObjectBase::ObjectType::Gatling) {
			continue;
		}

		auto gatling = objectBase.position();
		auto gatlinRadian = 100.0;

		AppFrame::Math::Sphere gatlingSphere = std::make_tuple(gatling, gatlinRadian);
		if (AppFrame::Math::Utility::CollisionCapsuleSphere(plyCapsule, gatlingSphere)) {
			objectBase.collisionComponent().report().id(ReportId::HitFromPlayer);
			_owner.collisionComponent().report().id(ReportId::HitFromGatling);
			_owner.collisionComponent().hitPos(objectBase.position());
		}
		else {
			objectBase.collisionComponent().report().id(ReportId::None);
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
		OutputDebugString("----------ダウンキャスト失敗----------");
	}
#endif

	return dynamic_cast<Object::ObjectBase&>(obj);
}
