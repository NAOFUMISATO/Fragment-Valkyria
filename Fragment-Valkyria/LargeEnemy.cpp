
/*****************************************************************//**
 * \file   LargeEnemy.cpp
 * \brief  ラージエネミーの処理を回すクラス
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "LargeEnemy.h"
#include "GameMain.h"
#include "ObjectFactory.h"
#include "CollisionComponent.h"
#include "ModelAnimeComponent.h"

using namespace FragmentValkyria::Enemy;

LargeEnemy::LargeEnemy(Game::GameMain& gameMain) : ObjectBase{ gameMain } {
	
}

void LargeEnemy::Init() {
	auto modelHandle = _modelAnimeComponent->modelHandle();
	_collision = _modelAnimeComponent->FindFrame("Spider");
	// フレーム1をナビメッシュとして使用
	MV1SetupCollInfo(modelHandle, _collision);
}

void LargeEnemy::Input(InputManager& input) {
	_stateServer->Input(input);
}

void LargeEnemy::Update() {
	//コリジョン情報の更新
	MV1RefreshCollInfo(_modelAnimeComponent->modelHandle(), _collision);
	// 状態の更新
	_stateServer->Update();
	// ワールド行列の更新
	ComputeWorldTransform();
	// モデルの更新
	_modelAnimeComponent->Update();

	GetObjServer().Register("EnemyPos", _position);
}

void LargeEnemy::Draw() {
	_stateServer->Draw();
}

void LargeEnemy::CreateFallObject() {
	auto fallObject = gameMain().objFactory().Create("FallObject");
	gameMain().objServer().Add(std::move(fallObject));
}

void LargeEnemy::CreateGatling() {
	auto gatling = gameMain().objFactory().Create("Gatling");
	gameMain().objServer().Add(std::move(gatling));
}

void LargeEnemy::HitCheckFromFallObject() {
	auto report = _collisionComponent->report();
	if (report.id() == Collision::CollisionComponent::ReportId::HitFromFallObject) {
		_hp -= _collisionComponent->damage();

		if (_hp <= 0) {
			_stateServer->GoToState("Die");
		}

		_collisionComponent->report().id(Collision::CollisionComponent::ReportId::None);
	}

}

void LargeEnemy::StateBase::Draw() {
	_owner._modelAnimeComponent->Draw();
}

void LargeEnemy::StateIdle::Enter() {
	_owner._stateCnt = 0;
	_owner._modelAnimeComponent->ChangeAnime("Spider_Armature|warte_pose", true);
}

void LargeEnemy::StateIdle::Input(InputManager& input) {

}

void LargeEnemy::StateIdle::Update() {

	if (_owner._stateCnt >= 1 && _owner._stateCnt % 600 == 0) {
		if (!_owner._fallObjectflag) {
			_owner._stateServer->PushBack("FallObject");
		}
		else {
			_owner._stateServer->PushBack("Gatling");
		}
	}

	_owner.HitCheckFromFallObject();

	++_owner._stateCnt;
}

void LargeEnemy::StateFallObject::Enter() {
	_owner._stateCnt = 0;
	_owner._fallObjectflag = true;
	_owner._modelAnimeComponent->ChangeAnime("Spider_Armature|Attack", true);
}

void LargeEnemy::StateFallObject::Input(InputManager& input) {

}

void LargeEnemy::StateFallObject::Update() {
	auto cnt = _owner._modelAnimeComponent->repeatedCount();
	if (cnt > 0) {
		_owner._stateServer->PopBack();
	}

	if (_owner._stateCnt == 0) {
		_owner.CreateFallObject();
	}

	/*_owner._stateServer->PopBack();*/
	_owner.HitCheckFromFallObject();

	++_owner._stateCnt;
}

void LargeEnemy::StateGatling::Enter() {
	_owner._stateCnt = 0;
	_owner._gatlingCnt = 10;
	_owner._modelAnimeComponent->ChangeAnime("Spider_Armature|Jump", true);
}

void LargeEnemy::StateGatling::Update() {
	if (_owner._stateCnt % _owner.GatlingFrame == 0) {
		_owner.CreateGatling();
		--_owner._gatlingCnt;
	}

	if (_owner._gatlingCnt <= 0) {
		_owner._stateServer->PopBack();
		_owner._fallObjectflag = false;
	}

	_owner.HitCheckFromFallObject();

	++_owner._stateCnt;
}

void LargeEnemy::StateDie::Enter() {
	_owner.modelAnimeComponent().ChangeAnime("Spider_Armature|die", false);
	_owner._freezeTime = 60 * 2;
}

void LargeEnemy::StateDie::Update() {
	auto playTime = _owner.modelAnimeComponent().playTime();
	if (playTime >= 70.0f) {
		//モーションを止める
		_owner.modelAnimeComponent().timeRate(0.0);
		if (_owner._freezeTime > 0) {
			--_owner._freezeTime;
		}
		else {
			_owner.gameMain().modeServer().GoToMode("Title");
		}
	}
}
