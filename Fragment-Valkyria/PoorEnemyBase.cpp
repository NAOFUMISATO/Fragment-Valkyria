#include "PoorEnemyBase.h"
#include "CollisionComponent.h"
#include "ModelAnimeComponent.h"
#include "ObjectFactory.h"
#include "GameMain.h"
#include "ObjectServer.h"

using namespace FragmentValkyria::Enemy;

namespace {
	auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("poorenemygatling",
		{ "gravity", "rotate_speed" });

	const double Gravity = paramMap["gravity"];
	const double RotateSpeed = paramMap["rotate_speed"];
	constexpr auto OneFrame = 60;
	constexpr auto DegreeQuarter = 90;
	constexpr auto StepDistance = 1000.0;
	constexpr auto StepSpeed = 15.0;
	constexpr auto StepDistanceLimit = 100.0;
}

PoorEnemyBase::PoorEnemyBase(Game::GameMain& gameMain) : Object::ObjectBase{ gameMain } {
}

void PoorEnemyBase::Init() {
	auto modelHandle = _modelAnimeComponent->modelHandle();
	_collNum = _modelAnimeComponent->FindFrame("Spider");
	// フレーム1をナビメッシュとして使用
	MV1SetupCollInfo(modelHandle, _collNum, 1, 1, 1);
}

void PoorEnemyBase::Update() {
	//コリジョン情報の更新
	MV1RefreshCollInfo(_modelAnimeComponent->modelHandle(), _collNum);
	_collisionComponent->BulletFromPoorEnemyGatling();
	_collisionComponent->PoorEnemyFromPlayer();
	HitCheckFromBullet();
	HitCheckFromFallObject();
	// 状態の更新
	_stateServer->Update();
	// ワールド行列の更新
	ComputeWorldTransform();
	// モデルの更新
	_modelAnimeComponent->Update();
}

void PoorEnemyBase::Draw() {
	_stateServer->Draw();
}

void PoorEnemyBase::Fall() {
	auto y = 0.5 * Gravity * _stateCnt * _stateCnt;
	_position.Add(Vector4(0.0, y, 0.0));
}

void PoorEnemyBase::Rotate() {
	auto [x, y, z] = _position.GetVec3();
	auto toPlayer = _gameMain.objServer().GetVecData("PlayerPos") - Vector4(x, 0.0, z);
	toPlayer.Normalized();
	AppFrame::Math::Matrix44 rotate;
	rotate.RotateY(_rotation.GetY(), true);

	auto forward = Vector4(0.0, 0.0, 1.0) * rotate;
	auto addRotate = 0.5 * forward.Cross(toPlayer).GetY();
	_rotation.Add(Vector4(0.0, addRotate * RotateSpeed, 0.0));
}

void PoorEnemyBase::HitCheckFromBullet() {
	auto report = _collisionComponent->report();
	if (report.id() == Collision::CollisionComponent::ReportId::HitFromBullet) {

		_hp -= _collisionComponent->damage();

		_collisionComponent->report().id(Collision::CollisionComponent::ReportId::None);

		if (_hp <= 0) {
			_stateServer->GoToState("Die");
		}
	}
}

void PoorEnemyBase::HitCheckFromFallObject() {
	auto report = _collisionComponent->report();
	if (report.id() == Collision::CollisionComponent::ReportId::HitFromFallObject) {

		_hp -= _collisionComponent->damage();

		_collisionComponent->report().id(Collision::CollisionComponent::ReportId::None);

		if (_hp <= 0) {
			_stateServer->GoToState("Die");
		}
	}
}

void PoorEnemyBase::StateBase::Draw() {
	_owner._modelAnimeComponent->Draw();
}

void PoorEnemyBase::StateIdle::Enter() {
	_owner._modelAnimeComponent->ChangeAnime("Spider_Armature|warte_pose", true);
	_owner._stateCnt = 0;
}

void PoorEnemyBase::StateIdle::Update() {
	_owner.Rotate();
	++_owner._stateCnt;
}

void PoorEnemyBase::StateSideStep::Enter() {
   _owner._modelAnimeComponent->ChangeAnime("Spider_Armature|lump", true);
	SideStepDecide();
}

void PoorEnemyBase::StateSideStep::Update() {
	auto length = (_moveOnPos - _owner._position).Lenght();
	if (length <= StepDistanceLimit){
		_owner._stateServer->GoToState("Idle");
	}
	_owner._position = _owner._position + _moveOnDir * StepSpeed;
}

void PoorEnemyBase::StateFall::Enter() {
	_owner._modelAnimeComponent->ChangeAnime("Spider_Armature|fall", true);
	_owner._stateCnt = 0;
}

void PoorEnemyBase::StateFall::Update() {
	_owner.Fall();
	_owner.Rotate();

	if (_owner.position().GetY() <= 0.0) {
		auto [x, y, z] = _owner.position().GetVec3();
		_owner.position(Vector4(x, 0.0, z));
		_owner._stateServer->GoToState("Idle");
	}

	++_owner._stateCnt;
}

void PoorEnemyBase::StateDie::Enter() {
	_owner._modelAnimeComponent->ChangeAnime("Spider_Armature|die", true);
	_owner._stateCnt = 0;
	_timeOver = 90;
}

void PoorEnemyBase::StateDie::Update() {
	if (_timeOver <= 0) {
		_owner.SetDead();
	}

	--_timeOver;
}

void PoorEnemyBase::StateSideStep::SideStepDecide() {
	auto moved = _owner.GetObjServer().GetVecData("PlayerPos") - _owner._position;
	moved.Normalized();
	auto quarterRight = AppFrame::Math::Matrix44();
	auto quarterLeft = AppFrame::Math::Matrix44();
	quarterRight.RotateY(DegreeQuarter, false);
	quarterLeft.RotateY(-DegreeQuarter, false);
	auto rightDir = moved * quarterRight;
	auto leftDir = moved * quarterLeft;
	auto rightPos = _owner._position + rightDir * StepDistance;
	auto leftPos = _owner._position + leftDir * StepDistance;
	auto isRightSafeZone = _owner._collisionComponent->IsLineFromStage(rightPos);
	auto isLeftSafeZone = _owner._collisionComponent->IsLineFromStage(leftPos);
	auto random = AppFrame::Math::Utility::GetRandom(0, 1);
	if (random == 0) {
		if (isRightSafeZone) {
			_moveOnDir = rightDir;
			_moveOnPos = rightPos;
		}
		else if (isLeftSafeZone) {
			_moveOnDir = leftDir;
			_moveOnPos = leftPos;
		}
		else {
			_owner._stateServer->GoToState("Idle");
		}
	}
	if (random == 1) {
		if (isLeftSafeZone) {
			_moveOnDir = leftDir;
			_moveOnPos = leftPos;
		}
		else if (isRightSafeZone) {
			_moveOnDir = rightDir;
			_moveOnPos = rightPos;
		}
		else {
			_owner._stateServer->GoToState("Idle");
		}
	}
}