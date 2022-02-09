
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

namespace {
	auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("largeenemy", {
	   "gatling_frame"});
	const int GatlingFrame = paramMap["gatling_frame"];
}

LargeEnemy::LargeEnemy(Game::GameMain& gameMain) : ObjectBase{ gameMain } {
	
}

void LargeEnemy::Init() {
	auto modelHandle = _modelAnimeComponent->modelHandle();
	_collision = _modelAnimeComponent->FindFrame("S301_typeCO");
	// フレーム1をナビメッシュとして使用
	MV1SetupCollInfo(modelHandle, _collision, 3, 6, 3);
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
	GetObjServer().RegistDouble("BossHP", _hp);
}

void LargeEnemy::Draw() {
	_stateServer->Draw();
}

void LargeEnemy::CreateGatling() {
	//ガトリングを生成する座標を設定
	GetObjServer().RegistVector("GatlingPos", _position);
	auto gatling = gameMain().objFactory().Create("Gatling");
	gameMain().objServer().Add(std::move(gatling));
}

void LargeEnemy::CreateLaser() {
	for (auto&& objects : _gameMain.objServer().runObjects()) {
		auto& objectBase = dynamic_cast<Object::ObjectBase&>(*objects);

		if (objectBase.GetObjType() == Object::ObjectBase::ObjectType::FallObject) {

			auto fallObjectToPly = _gameMain.objServer().GetVecData("PlayerPos") - objectBase.position();
			auto [x, y, z] = fallObjectToPly.GetVec3();
			auto checkSize = x * x + y * y + z * z;
			auto checkPair = std::make_pair(checkSize, objectBase.position());
			_objectDistance.emplace_back(checkPair);

			continue;
		}
	}
	std::sort(_objectDistance.begin(), _objectDistance.end());
	if (_objectDistance.empty()) {
		_gameMain.objServer().RegistVector("LaserDirectionPos", _gameMain.objServer().GetVecData("PlayerPos"));
	}
	else {
		_gameMain.objServer().RegistVector("LaserDirectionPos", _objectDistance[0].second);
	}

	_objectDistance.clear();

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

void LargeEnemy::Move(const Vector4& moved) {
	auto [x, y, z] = moved.GetVec3();
	auto position = _position;
	position = _collisionComponent->BossCheckStage(position, Vector4(x, y, 0.0));
	position = _collisionComponent->BossCheckStage(position, Vector4(0.0, y, z));

	_position = position;
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

	if (_owner._stateCnt >= 1 && _owner._stateCnt % (60 * 5) == 0) {
		if (!_owner._fallObjectflag) {
			/*_owner._stateServer->GoToState("Move");*/
			_owner._stateServer->GoToState("FallObject");
			/*_owner._stateServer->GoToState("Laser");*/
		}
		else if (!_owner._moving){
			_owner._stateServer->GoToState("Move");
		}
		else if (!_owner._gatlingFlag){
			_owner._stateServer->GoToState("Gatling");
		}
		else {
			_owner._stateServer->GoToState("Laser");
		}

	}
	_owner._collisionComponent->LargeEnemyFromPlayer();
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
	_owner._gatlingFlag = true;
	_owner._modelAnimeComponent->ChangeAnime("beem", true);
}

void LargeEnemy::StateGatling::Update() {
	if (_owner._stateCnt % GatlingFrame == 0) {
		_owner.CreateGatling();
		--_owner._gatlingCnt;
	}

	if (_owner._gatlingCnt <= 0) {
		_owner._stateServer->GoToState("Idle");
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
			_owner.gameMain().modeServer().PushBack("MissionCompleted");
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

	auto result = /*AppFrame::Math::Utility::GetRandom(0, 1)*/true;
	if (result) {
		_owner._moved = _owner.GetObjServer().GetVecData("PlayerPos") - _owner._position;
		_owner._moved.Normalized();
		_owner._moved = _owner._moved * 30.0;
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
			_owner.Move(_owner._moved);
		}
		
		++_owner._stateCnt;
	}
	_owner._collisionComponent->LargeEnemyFromPlayer();

	_owner.HitCheckFromFallObject();
	_owner.HitCheckFromBullet();
}

void LargeEnemy::StateLaser::Enter() {
	_owner._stateCnt = 0;
	_owner._modelAnimeComponent->ChangeAnime("beem", true);
	_createLaser = false;
}

void LargeEnemy::StateLaser::Update() {
	if (_owner._stateCnt >= 60 * 3 && !_createLaser) {
		_owner.CreateLaser();
		_createLaser = true;
	}
	else if (_owner._stateCnt >= 60 * 6) {
		_owner._fallObjectflag = false;
		_owner._moving = false;
		_owner._gatlingFlag = false;
		_owner._stateServer->GoToState("Idle");
	}

	++_owner._stateCnt;
}
