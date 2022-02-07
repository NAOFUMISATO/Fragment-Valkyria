
/*****************************************************************//**
 * \file   CollisionServer.cpp
 * \brief  当たり判定処理を行うクラス
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "CollisionComponent.h"
#include "ModelAnimeComponent.h"
#include "Gatling.h"
#include "Player.h"
#include "Stage.h"
#include "StageModelComponent.h"
#include "Laser.h"
#include "ObjectBase.h"
#ifdef _DEBUG
#include <stdexcept>
#include <windows.h>
#endif

using namespace FragmentValkyria::Collision;

namespace {
	auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("collision",
		{ "fallobject_range", "ply_radius", "ply_capsule_pos1",
		 "ply_capsule_pos2", "gatling_radius", "fallobject_capsule_pos1",
		"fallobject_capsule_pos2", "fallobject_radius", "laser_radius"});

	const double FallObjectRange = paramMap["fallobject_range"];     //!< フォールオブジェクトの球の半径
	const double PlayerRadius = paramMap["ply_radius"];              //!< プレイヤーのカプセルの半径
	const double PlayerCapsulePos1 = paramMap["ply_capsule_pos1"];    //!< プレイヤーのカプセルを形成する2点中の一点の座標までのプレイヤーの位置からの距離
	const double PlayerCapsulePos2 = paramMap["ply_capsule_pos2"];    //!< プレイヤーのカプセルを形成する2点中の一点の座標までのプレイヤーの位置からの距離
	const double GatlingRadius = paramMap["gatling_radius"];         //!< ガトリングの半径
	const double FallObjectCapsulePos1 = paramMap["fallobject_capsule_pos1"];         //!< フォールオブジェクトのカプセルを形成する2点中の一点の座標までのフォールオブジェクトの位置からの距離
	const double FallObjectCapsulePos2 = paramMap["fallobject_capsule_pos2"];         //!< フォールオブジェクトのカプセルを形成する2点中の一点の座標までのフォールオブジェクトの位置からの距離
	const double FallObjectRadius = paramMap["fallobject_radius"];         //!< フォールオブジェクトのカプセルの半径
	const double LaserRadius = paramMap["laser_radius"];                   //!<レーザーのカプセルの半径
}

CollisionComponent::CollisionComponent(Object::ObjectBase& owner) : _owner{ owner } {
	_report = std::make_unique<Report>();
}

void CollisionComponent::ObjectRangeFromPlayer() {
	auto& player = dynamic_cast<Player::Player&>(_owner);
	auto invincibleCnt = player.invincibleCnt();
	if (invincibleCnt > 0) {
		return;
	}
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
				if (objectBase.collisionComponent().report().id() == ReportId::HitFromLargeEnemy) {
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
		if (objectBase.collisionComponent().report().id() == ReportId::HitFromLargeEnemy) {
			break;
		}
		auto& player = dynamic_cast<Player::Player&>(objectBase);
		auto invincibleCnt = player.invincibleCnt();
		if (invincibleCnt > 0) {
			return;
		}

		auto plyPoint = player.position();
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
		auto& player = dynamic_cast<Player::Player&>(objectBase);
		auto invincibleCnt = player.invincibleCnt();
		if (invincibleCnt > 0) {
			return;
		}
		// プレイヤー側のカプセルを設定
		auto plyPos = objectBase.position();
		auto pos1 = plyPos + AppFrame::Math::Vector4(0.0, PlayerCapsulePos1, 0.0);
		plyPos.Add(0.0, PlayerCapsulePos2/*60.0*/, 0.0);
		auto radian = static_cast<float>(PlayerRadius/*30.0*/);
		
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
		// ガトリング側の球を設定
		auto gatlingPos = objectBase.position();
		auto radian = static_cast<float>(GatlingRadius);

		auto result = MV1CollCheck_Sphere(objectModel, collision, AppFrame::Math::ToDX(gatlingPos), radian);

		if (result.HitNum > 0) {
			objectBase.collisionComponent().hitPos(_owner.position());
			objectBase.collisionComponent().report().id(ReportId::HitFromIdleFallObject);
		}
	}

}

void CollisionComponent::GatlingFromPlayer() {
	auto& player = dynamic_cast<Player::Player&>(_owner);
	auto invincibleCnt = player.invincibleCnt();
	if (invincibleCnt > 0) {
		return;
	}
	auto playerPos = _owner.position();
	auto capsulePos1 = playerPos + Vector4(0.0, 30.0, 0.0);
	auto capsulePos2 = playerPos + Vector4(0.0, 60.0, 0.0);
	auto casuleRadian = PlayerRadius/*30.0*/;

	AppFrame::Math::Capsule playerCapsule = std::make_tuple(capsulePos1, capsulePos2, casuleRadian);

	for (auto&& object : _owner.GetObjServer().runObjects()) {

		auto& objectBase = ObjectBaseCast(*object);
		
		if (objectBase.GetObjType() != Object::ObjectBase::ObjectType::Gatling) {
			continue;
		}

		auto gatling = objectBase.position();
		auto gatlingRadian = GatlingRadius/*100.0*/;

		AppFrame::Math::Sphere gatlingSphere = std::make_tuple(gatling, gatlingRadian);
		if (AppFrame::Math::Utility::CollisionCapsuleSphere(playerCapsule, gatlingSphere)) {
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
	auto capsulePos1 = Vector4(0.0, FallObjectCapsulePos1, 0.0) + fallObjectPos;
	auto capsulePos2 = Vector4(0.0, FallObjectCapsulePos2, 0.0) + fallObjectPos;
	auto capsuleRadian = static_cast<float>(FallObjectRadius);

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

void CollisionComponent::FallObjectFromLaser() {
	auto fallObjectPos = _owner.position();
	auto fallObjectCapsulePos1 = Vector4(0.0, FallObjectCapsulePos1, 0.0) + fallObjectPos;
	auto fallObjectCapsulePos2 = Vector4(0.0, FallObjectCapsulePos2, 0.0) + fallObjectPos;
	auto fallObjectRadius = FallObjectRadius;

	auto fallObjectCapsule = std::make_tuple(fallObjectCapsulePos1, fallObjectCapsulePos2, fallObjectRadius);
	for (auto&& object : _owner.GetObjServer().runObjects()) {

		auto& objectBase = ObjectBaseCast(*object);

		if (objectBase.GetObjType() != Object::ObjectBase::ObjectType::Laser) {
			continue;
		}

		auto& laser = dynamic_cast<Enemy::Laser&>(objectBase);
		auto laserCapsulePos1 = laser.position();
		auto laserCapsulePos2 = laser.end();
		auto laserRadius = LaserRadius;

		auto laserCapsule = std::make_tuple(laserCapsulePos1, laserCapsulePos2, laserRadius);

		if (AppFrame::Math::Utility::CollisionCapsuleCapsule(fallObjectCapsule, laserCapsule)) {
			_owner.collisionComponent().report().id(ReportId::HitFromLaser);
		}
		break;
		
	}

}

void CollisionComponent::PlayerFromLaser() {
	auto& player = dynamic_cast<Player::Player&>(_owner);
	auto invincibleCnt = player.invincibleCnt();
	if (invincibleCnt > 0) {
		return;
	}
	auto playerPos = _owner.position();
	auto plyCapsulePos1 = Vector4(0.0, PlayerCapsulePos1, 0.0) + playerPos;
	auto plyCapsulePos2 = Vector4(0.0, PlayerCapsulePos2, 0.0) + playerPos;
	auto playerCapsuleRadius = PlayerRadius/*30.0*/;

	AppFrame::Math::Capsule playerCapsule = std::make_tuple(plyCapsulePos1, plyCapsulePos2, playerCapsuleRadius);

	for (auto&& object : _owner.GetObjServer().runObjects()) {

		auto& objectBase = ObjectBaseCast(*object);

		if (objectBase.GetObjType() != Object::ObjectBase::ObjectType::Laser) {
			continue;
		}

		auto& laser = dynamic_cast<Enemy::Laser&>(objectBase);
		auto laserCapsulePos1 = laser.position();
		auto laserCapsulePos2 = laser.end();
		auto laserRadius = LaserRadius;

		auto laserCapsule = std::make_tuple(laserCapsulePos1, laserCapsulePos2, laserRadius);

		if (AppFrame::Math::Utility::CollisionCapsuleCapsule(playerCapsule, laserCapsule)) {
			_owner.collisionComponent().report().id(ReportId::HitFromLaser);
			_owner.collisionComponent().damage(20.0);
			_owner.collisionComponent().hitPos(laser.position());
		}
		break;

	}

}

void CollisionComponent::LargeEnemyFromPlayer() {

	auto largeEnemyModel = _owner.modelAnimeComponent().modelHandle();
	auto collision = _owner.modelAnimeComponent().FindFrame("S301_typeCO");

	for (auto&& object : _owner.GetObjServer().runObjects()) {

		auto& objectBase = ObjectBaseCast(*object);

		if (objectBase.GetObjType() != Object::ObjectBase::ObjectType::Player) {
			continue;
		}
		auto& player = dynamic_cast<Player::Player&>(objectBase);
		auto invincibleCnt = player.invincibleCnt();
		if (invincibleCnt > 0) {
			return;
		}
		auto playerPos = objectBase.position();
		auto plyCapsulePos1 = Vector4(0.0, PlayerCapsulePos1, 0.0) + playerPos;
		auto plyCapsulePos2 = Vector4(0.0, PlayerCapsulePos2, 0.0) + playerPos;
		auto playerCapsuleRadius = static_cast<float>(PlayerRadius)/*30.0*/;

		auto result = MV1CollCheck_Capsule(largeEnemyModel, collision, AppFrame::Math::ToDX(plyCapsulePos1), AppFrame::Math::ToDX(plyCapsulePos2), playerCapsuleRadius);

		if (result.HitNum > 0) {
			objectBase.collisionComponent().report().id(ReportId::HitFromLargeEnemy);
			objectBase.collisionComponent().hitPos(_owner.position());
			objectBase.collisionComponent().damage(20.0);
		}
	}

}

void CollisionComponent::BulletFromPoorEnemyGatling() {
	auto poorEnemyGatlingModel = _owner.modelAnimeComponent().modelHandle();
	auto collision = _owner.modelAnimeComponent().FindFrame("Spider");

	for (auto&& object : _owner.GetObjServer().runObjects()) {

		auto& objectBase = ObjectBaseCast(*object);

		if (objectBase.GetObjType() != Object::ObjectBase::ObjectType::Bullet) {
			continue;
		}
		
		auto bullet = objectBase.position();
		auto bulletRadius = static_cast<float>(20.0);

		auto result = MV1CollCheck_Sphere(poorEnemyGatlingModel, collision, AppFrame::Math::ToDX(bullet), bulletRadius);

		if (result.HitNum > 0) {
			objectBase.collisionComponent().report().id(ReportId::HitFromPoorEnemyGatling);
			_owner.collisionComponent().hitPos(_owner.position());
			_owner.collisionComponent().damage(10.0);
			_owner.collisionComponent().report().id(ReportId::HitFromBullet);
		}
	}
}

void CollisionComponent::PoorEnemyGatlingFromObjectModel() {
	auto fallObjectPos = _owner.position();
	auto capsulePos1 = Vector4(0.0, FallObjectCapsulePos1, 0.0) + fallObjectPos;
	auto capsulePos2 = Vector4(0.0, FallObjectCapsulePos2, 0.0) + fallObjectPos;
	auto capsuleRadian = static_cast<float>(FallObjectRadius);

	for (auto&& object : _owner.GetObjServer().runObjects()) {

		auto& objectBase = ObjectBaseCast(*object);

		if (objectBase.GetObjType() != Object::ObjectBase::ObjectType::PoorEnemyGatling) {
			continue;
		}

		auto poorEnemyGatlingModel = objectBase.modelAnimeComponent().modelHandle();
		auto collision = objectBase.modelAnimeComponent().FindFrame("Spider");

		auto result = MV1CollCheck_Capsule(poorEnemyGatlingModel, collision, AppFrame::Math::ToDX(capsulePos1), AppFrame::Math::ToDX(capsulePos2), capsuleRadian);

		if (result.HitNum > 0) {
			objectBase.collisionComponent().report().id(ReportId::HitFromFallObject);
			objectBase.collisionComponent().damage(20.0);
			_owner.collisionComponent().report().id(ReportId::HitFromPoorEnemyGatling);
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

void CollisionComponent::CollisionStage() {
	for (auto&& object : _owner.GetObjServer().runObjects()) {

		auto& objectBase = ObjectBaseCast(*object);

		if (objectBase.GetObjType() != Object::ObjectBase::ObjectType::Stage) {
			continue;
		}
		
		auto& stage = dynamic_cast<Stage::Stage&>(objectBase);
		stage.stageComponent().GetHandleAndCollNum("");
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
