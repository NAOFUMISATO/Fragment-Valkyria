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

