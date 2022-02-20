
/*****************************************************************//**
 * \file   PoorEnemyGatling.cpp
 * \brief  ガトリング攻撃をしてくる雑魚敵の処理を回すクラス
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "PoorEnemyGatling.h"
#include "CollisionComponent.h"
#include "ModelAnimeComponent.h"
#include "ObjectFactory.h"
#include "GameMain.h"

using namespace FragmentValkyria::Enemy;

namespace {
	auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("poorenemygatling",
		{ "gravity", "rotate_speed"});

	const double Gravity = paramMap["gravity"];
	const double RotateSpeed = paramMap["rotate_speed"];
}

PoorEnemyGatling::PoorEnemyGatling(Game::GameMain& gameMain) : Object::ObjectBase{ gameMain } {

}

void PoorEnemyGatling::Init() {
	auto modelHandle = _modelAnimeComponent->modelHandle();
	_collision = _modelAnimeComponent->FindFrame("Spider");
	// フレーム1をナビメッシュとして使用
	MV1SetupCollInfo(modelHandle, _collision, 1, 1, 1);
}

void PoorEnemyGatling::Update() {
	//コリジョン情報の更新
	MV1RefreshCollInfo(_modelAnimeComponent->modelHandle(), _collision);
	// 状態の更新
	_stateServer->Update();
	// ワールド行列の更新
	ComputeWorldTransform();
	// モデルの更新
	_modelAnimeComponent->Update();
}

void PoorEnemyGatling::Draw() {
	_stateServer->Draw();
}

void PoorEnemyGatling::Fall() {
	auto y = 0.5 * Gravity * _stateCnt * _stateCnt;
	_position.Add(Vector4(0.0, y, 0.0));
}

void PoorEnemyGatling::Rotate() {
	auto [x, y, z] = _position.GetVec3();
	auto toPlayer = _gameMain.objServer().GetVecData("PlayerPos") - Vector4(x, 0.0, z);
	toPlayer.Normalized();
	AppFrame::Math::Matrix44 rotate;
	rotate.RotateY(_rotation.GetY(), true);

	auto forward = Vector4(0.0, 0.0, 1.0) * rotate;
	auto addRotate = 0.5 * forward.Cross(toPlayer).GetY();
	_rotation.Add(Vector4(0.0, addRotate * RotateSpeed, 0.0));
}

void PoorEnemyGatling::CreateGatling() {
	auto handle = modelAnimeComponent().modelHandle();
	auto gatlingFrame = modelAnimeComponent().FindFrame("Spider_Armature");
	auto gatlingPos = MV1GetFramePosition(handle, gatlingFrame);
	_gameMain.objServer().RegistVector("GatlingPos", AppFrame::Math::ToMath(gatlingPos));
	_gameMain.objServer().RegistVector("GatlingMoveDirection", _gatlingMoveDirection);
	auto gatling = _gameMain.objFactory().Create("Gatling");
	gameMain().objServer().Add(std::move(gatling));
}

void PoorEnemyGatling::HitCheckFromBullet() {
	auto report = _collisionComponent->report();
	if (report.id() == Collision::CollisionComponent::ReportId::HitFromBullet) {

		_hp -= _collisionComponent->damage();

		_collisionComponent->report().id(Collision::CollisionComponent::ReportId::None);

		if (_hp <= 0) {
			_stateServer->GoToState("Die");
		}
	}
}

void PoorEnemyGatling::HitCheckFromFallObject() {
	auto report = _collisionComponent->report();
	if (report.id() == Collision::CollisionComponent::ReportId::HitFromFallObject) {

		_hp -= _collisionComponent->damage();

		_collisionComponent->report().id(Collision::CollisionComponent::ReportId::None);

		if (_hp <= 0) {
			_stateServer->GoToState("Die");
		}
	}
}

void PoorEnemyGatling::StateBase::Draw() {
	_owner._modelAnimeComponent->Draw();
}

void PoorEnemyGatling::StateIdle::Enter() {
	_owner._modelAnimeComponent->ChangeAnime("Spider_Armature|warte_pose", true);
	_owner._stateCnt = 0;
}

void PoorEnemyGatling::StateIdle::Update() {
	_owner.Rotate();
	if (_owner._stateCnt >= 60 * 7) {
		_owner._stateServer->GoToState("Gatling");
	}
	_owner.collisionComponent().BulletFromPoorEnemyGatling();
	_owner.HitCheckFromBullet();
	_owner.HitCheckFromFallObject();
	++_owner._stateCnt;
}

void PoorEnemyGatling::StateFall::Enter() {
	_owner._modelAnimeComponent->ChangeAnime("Spider_Armature|fall", true);
	_owner._stateCnt = 0;
}

void PoorEnemyGatling::StateFall::Update() {
	_owner.Fall();
	_owner.Rotate();

	if (_owner.position().GetY() <= 0.0) {
		auto [x, y, z] = _owner.position().GetVec3();
		_owner.position(Vector4(x, 0.0, z));
		_owner._stateServer->GoToState("Idle");
	}

	++_owner._stateCnt;
}

void PoorEnemyGatling::StateGatling::Enter() {
	_owner._modelAnimeComponent->ChangeAnime("Spider_Armature|Jump", true);
	_owner._gatlingMoveDirection = _owner.GetObjServer().GetVecData("PlayerPos") - _owner._position;
	_owner._stateCnt = 0;
	_gatlingCnt = 5;
}

void PoorEnemyGatling::StateGatling::Update() {
	/*_owner.Rotate();*/
	if (_owner._stateCnt % 60 == 0) {
		_owner.CreateGatling();
		--_gatlingCnt;
		if (_gatlingCnt <= 0) {
			_owner._stateServer->GoToState("Idle");
		}
	}
	_owner.collisionComponent().BulletFromPoorEnemyGatling();
	_owner.HitCheckFromBullet();
	_owner.HitCheckFromFallObject();
	++_owner._stateCnt;
}

void PoorEnemyGatling::StateDie::Enter() {
	_owner._modelAnimeComponent->ChangeAnime("Spider_Armature|die", true);
	_owner._stateCnt = 0;
	_timeOver = 90;
}

void PoorEnemyGatling::StateDie::Update() {
	if (_timeOver <= 0) {
		_owner.SetDead();
	}

	--_timeOver;
}

