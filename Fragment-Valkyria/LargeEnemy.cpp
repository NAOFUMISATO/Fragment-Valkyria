
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
#include "PoorEnemyGatling.h"

using namespace FragmentValkyria::Enemy;

LargeEnemy::LargeEnemy(Game::GameMain& gameMain) : ObjectBase{ gameMain } {
	
}

void LargeEnemy::Init() {
	auto modelHandle = _modelAnimeComponent->modelHandle();
	_collision = _modelAnimeComponent->FindFrame("S301_typeCO");
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

	GetObjServer().RegistVector("EnemyPos", _position);
}

void LargeEnemy::Draw() {
	_stateServer->Draw();
}

void LargeEnemy::CreateGatling() {
	auto gatling = gameMain().objFactory().Create("Gatling");
	gameMain().objServer().Add(std::move(gatling));
}

void LargeEnemy::CreateLaser() {
	auto laser = gameMain().objFactory().Create("Laser");
	gameMain().objServer().Add(std::move(laser));
}

void LargeEnemy::CreateFallObject() {
	auto fallObject = gameMain().objFactory().Create("FallObject");
	gameMain().objServer().Add(std::move(fallObject));
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

void LargeEnemy::HitCheckFromBullet() {
	auto report = _collisionComponent->report();
	if (report.id() == Collision::CollisionComponent::ReportId::HitFromBullet) {
		_hp -= _collisionComponent->damage();

		if (_hp <= 0) {
			_stateServer->GoToState("Die");
		}

		_collisionComponent->report().id(Collision::CollisionComponent::ReportId::None);
	}
}

void LargeEnemy::Move() {
	_position = _position + _moved * 30.0;
}

void LargeEnemy::Rotate(bool& rotating) {
	_rotation.Add(0.0, _addRotate, 0.0);

	Matrix44 rotateY = Matrix44();
	rotateY.RotateY(_rotation.GetY(), true);
	Vector4 forward = Vector4(-1.0, 0.0, 0.0);
	forward = forward * rotateY;
	forward.Normalized();
	auto dot = _moved.Dot(forward);
	
	if (_addRotate > 0.0) {
		if (dot < 0) {
			rotating = false;
		}
	}
	else {
		if (dot >= 0) {
			rotating = false;
		}
	}
	
}

void LargeEnemy::SetAddRotate() {

	Matrix44 rotateY = Matrix44();
	rotateY.RotateY(_rotation.GetY(), true);
	Vector4 forward = Vector4(-1.0, 0.0, 0.0) * rotateY;
	forward.Normalized();
	_rotateDot = _moved.Dot(forward);

	if (_rotateDot >= 0) {
		_addRotate = 1.0;
	}
	else {
		_addRotate = -1.0;
	}
}

void LargeEnemy::StateBase::Draw() {
	_owner._modelAnimeComponent->Draw();
}

void LargeEnemy::StateIdle::Enter() {
	_owner._stateCnt = 0;
	_owner._modelAnimeComponent->ChangeAnime("idle", true);
}

void LargeEnemy::StateIdle::Input(InputManager& input) {

}

void LargeEnemy::StateIdle::Update() {

	if (_owner._stateCnt >= 1 && _owner._stateCnt % 300 == 0) {
		if (!_owner._fallObjectflag) {
			/*_owner._stateServer->GoToState("Move");*/
			_owner._stateServer->GoToState("FallObject");
			/*_owner._stateServer->GoToState("Laser");*/
		}
		else if (!_owner._moving){
			/*_owner._stateServer->GoToState("Move");*/
			_owner._stateServer->GoToState("Laser");
		}
		else {
			_owner._stateServer->GoToState("Gatling");
		}
	}

	_owner.HitCheckFromFallObject();
	_owner.HitCheckFromBullet();

	++_owner._stateCnt;
}

void LargeEnemy::StateFallObject::Enter() {
	_owner._stateCnt = 0;
	_owner._fallObjectflag = true;
	_owner._modelAnimeComponent->ChangeAnime("object_attack", false);
}

void LargeEnemy::StateFallObject::Input(InputManager& input) {

}

void LargeEnemy::StateFallObject::Update() {
	auto cnt = _owner._modelAnimeComponent->repeatedCount();
	if (cnt > 0) {
		_owner._stateServer->GoToState("Idle");
	}

	if (_owner._stateCnt == 0) {
		_owner.CreateFallObject();
	}

	/*_owner._stateServer->PopBack();*/
	_owner.HitCheckFromFallObject();
	_owner.HitCheckFromBullet();

	++_owner._stateCnt;
}

void LargeEnemy::StateGatling::Enter() {
	_owner._stateCnt = 0;
	_owner._gatlingCnt = 10;
	_owner._modelAnimeComponent->ChangeAnime("beem", true);
}

void LargeEnemy::StateGatling::Update() {
	if (_owner._stateCnt % _owner.GatlingFrame == 0) {
		_owner.CreateGatling();
		--_owner._gatlingCnt;
	}

	if (_owner._gatlingCnt <= 0) {
		_owner._stateServer->GoToState("Idle");
		_owner._fallObjectflag = false;
		_owner._moving = false;
	}

	_owner.HitCheckFromFallObject();
	_owner.HitCheckFromBullet();

	++_owner._stateCnt;
}

void LargeEnemy::StateDie::Enter() {
	_owner.modelAnimeComponent().ChangeAnime("beem", false);
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

void LargeEnemy::StateMove::Enter() {
	_endGetplyPos = true;
	_owner._stateCnt = 0;
	_owner._moving = true;
	_owner._firstRotating = true;
	_owner._endRotating = true;
	_owner._modelAnimeComponent->ChangeAnime("walk", true);

	auto result = AppFrame::Math::Utility::GetRandom(0, 1)/*1*/;
	if (result) {
		_owner._moved = _owner.GetObjServer().GetVecData("PlayerPos") - _owner._position;
		_owner._moved.Normalized();
	}
	else {
		auto degree = AppFrame::Math::Utility::GetRandom(0.0, 360.0);
		Matrix44 rotate = Matrix44();
		rotate.RotateY(degree, true);

		_owner._moved = Vector4(0.0, 0.0, 1.0) * rotate;
		_owner._moved.Normalized();
	}

	_owner.SetAddRotate();
}

void LargeEnemy::StateMove::Update() {
	if (_owner._firstRotating) {
		_owner.Rotate(_owner._firstRotating);
	}
	else {

		if (_owner._stateCnt >= 60 * 3) {
			if (_endGetplyPos) {
				_owner._moved = _owner.GetObjServer().GetVecData("PlayerPos") - _owner._position;
				_owner._moved.Normalized();
				_owner.SetAddRotate();
				_endGetplyPos = false;
			}

			if (_owner._endRotating) {
				_owner.Rotate(_owner._endRotating);
			}
			else {
				_owner._stateServer->GoToState("Idle");
			}
		}
		else if (_owner._stateCnt >= 60 * 1 ) {

			_owner.Move();
		}
		
		++_owner._stateCnt;
	}

	_owner.HitCheckFromFallObject();
	_owner.HitCheckFromBullet();
}

void LargeEnemy::StateLaser::Enter() {
	_owner._stateCnt = 0;
	_owner._moving = true;
	_owner._modelAnimeComponent->ChangeAnime("beem", true);
}

void LargeEnemy::StateLaser::Update() {
	if (_owner._stateCnt >= 60 * 3) {
		_owner.CreateLaser();
		_owner._stateServer->GoToState("Idle");
	}

	++_owner._stateCnt;
}
