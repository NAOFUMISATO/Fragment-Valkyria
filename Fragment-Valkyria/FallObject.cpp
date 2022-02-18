
/*****************************************************************//**
 * \file   FallObject.cpp
 * \brief  
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "FallObject.h"
#include "CollisionComponent.h"
#include "GameMain.h"
#include "ModelAnimeComponent.h"
#include "EffectObjFall.h"
#include "ObjectServer.h"

using namespace FragmentValkyria::Enemy;

namespace {
	auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("fallobject",
		{ "range", "gravity", "shoot_speed", "up_speed", "rotate_angle",
		"updown_range", "capsule_pos1", "capsule_pos2", "capsule_radius",
		"fallpoint_pos_y","fallpoint_scale","fallpoint_animespeed" });

	const double Range = paramMap["range"];
	const double Gravity = paramMap["gravity"];
	const double ShootSpeed = paramMap["shoot_speed"];
	const double UpSpeed = paramMap["up_speed"];
	const double RotateAngle = paramMap["rotate_angle"];
	const double UpDownRange = paramMap["updown_range"];
	const double CapsulePos1 = paramMap["capsule_pos1"];
	const double CapsulePos2 = paramMap["capsule_pos2"];
	const double CapsuleRadius = paramMap["capsule_radius"];
	const double FallPointPosY = paramMap["fallpoint_pos_y"];
	const double FallPointScale = paramMap["fallpoint_scale"];
	const int FallPointAnimeSpeed = paramMap["fallpoint_animespeed"];
	constexpr auto DefaultPointScale = 1.0;
	constexpr auto DefaultPointAngle = 0.0;
}

FallObject::FallObject(Game::GameMain& gameMain) : ObjectBase{ gameMain } {

}

void FallObject::Init() {
	auto modelHandle = _modelAnimeComponent->modelHandle();
	_collision = MV1SearchFrame(modelHandle, "drum_green_c");
	//// ナビメッシュを非表示
	//MV1SetFrameVisible(modelHandle, _collision, FALSE);
#ifdef _DEBUG
	MV1SetFrameOpacityRate(modelHandle, _collision, 0.5f);
#endif

	// フレーム1をナビメッシュとして使用
	MV1SetupCollInfo(modelHandle, _collision);

	_fallPointHandles = _gameMain.resServer().GetTextures("FallPoint");
}

void FallObject::Input(InputManager& input) {
	_stateServer->Input(input);
}

void FallObject::Update() {
	//コリジョン情報の更新
	MV1RefreshCollInfo(_modelAnimeComponent->modelHandle(), _collision);
	// 状態の更新
	_stateServer->Update();
	// ワールド行列の更新
	ComputeWorldTransform();
	// モデルの更新
	_modelAnimeComponent->Update();
}

void FallObject::Draw() {
	_stateServer->Draw();
}

void FallObject::HitCheckFromPlayerPoint() {
	auto report = _collisionComponent->report();
	if (report.id() == Collision::CollisionComponent::ReportId::HitFromPlayer) {
		_stateServer->PushBack("Save");
	}
}

void FallObject::HitCheckFromLargeEnemy() {
	auto report = _collisionComponent->report();

	if (report.id() == Collision::CollisionComponent::ReportId::HitFromLargeEnemy) {
		_stateServer->PushBack("Die");
	}
}

void FallObject::HitCheckFromLaser() {
	auto report = _collisionComponent->report();

	if (report.id() == Collision::CollisionComponent::ReportId::HitFromLaser) {
		_stateServer->PushBack("Die");
	}
}

void FallObject::HitCheckFromPoorEnemyGatling() {
	auto report = _collisionComponent->report();

	if (report.id() == Collision::CollisionComponent::ReportId::HitFromPoorEnemyGatling) {
		_stateServer->PushBack("Die");
	}
}

void FallObject::OutStageCheck() {
	auto report = _collisionComponent->report();

	if (report.id() == Collision::CollisionComponent::ReportId::OutStage) {
		_stateServer->PushBack("Die");
	}
}

void FallObject::CheckPlayerKnockBack() {
	auto result = _collisionComponent->knockBack();
	if (result) {
		_stateServer->PushBack("Die");
	}
}

void FallObject::Save() {
	_rotateAngle += 0.01;
	auto radian = AppFrame::Math::Utility::DegreeToRadian(_rotateAngle);
	auto sinValue = std::sin(radian);
	auto xyz = RotateAngle * AppFrame::Math::DEGREES_180 * sinValue;
	_rotation = Vector4(xyz, xyz, xyz);

	_upDownAngle += 2.0;
	auto upDownRadian = AppFrame::Math::Utility::DegreeToRadian(_upDownAngle);
	auto upDownSinValue = std::sin(upDownRadian);
	auto y = UpDownRange * upDownSinValue;
	_position = _vecBeforeSave + Vector4(0.0, 300.0 + y, 0.0);

}

void FallObject::Up() {
	_position.Add(0.0, UpSpeed, 0.0);

	if (_position.GetY() > 300.0) {
		_saveFlag = true;
	}
}

void FallObject::Shoot() {
	auto move = _shootVec * ShootSpeed;
	_position = _position + move;
}

void FallObject::StateBase::Draw() {
	_owner._modelAnimeComponent->Draw();
#ifdef _DEBUG
	auto pos = AppFrame::Math::ToDX(_owner._position);
	auto radian = static_cast<float>(Range);
	DrawSphere3D(pos, radian, 10, GetColor(0, 0, 0), GetColor(0, 0, 0), FALSE);

	auto pos1 = _owner._position + Vector4(0.0, CapsulePos1, 0.0);
	auto pos2 = _owner._position + Vector4(0.0, CapsulePos2, 0.0);
	radian = static_cast<float>(CapsuleRadius);

	DrawCapsule3D(AppFrame::Math::ToDX(pos1), AppFrame::Math::ToDX(pos2), radian, 20, GetColor(0, 255, 0), GetColor(0, 0, 0), FALSE);
#endif
}

void FallObject::StateIdle::Enter() {
	_owner._isFall = false;
}

void FallObject::StateIdle::Input(InputManager& input) {
	if (input.GetXJoypad().LeftTrigger() >= 20) {
		_owner.HitCheckFromPlayerPoint();
	}
}

void FallObject::StateIdle::Update() {
	_owner._collisionComponent->PlayerFromFallObjectModel(_owner._isFall);
	_owner._collisionComponent->PlayerFromObjectRange();
	_owner._collisionComponent->GatlingFromObjectModel();
	_owner._collisionComponent->FallObjectFromLaser();
	_owner.HitCheckFromLaser();
}

void FallObject::StateFall::Enter() {
	_owner._fallTimer = 0.0;
}

void FallObject::StateFall::Input(InputManager& input) {
	
}

void FallObject::StateFall::Update() {
	auto posY = (0.5 * Gravity * _owner._fallTimer * _owner._fallTimer);

	_owner._position.Add(0.0, -posY, 0.0);

	if (_owner._position.GetY() <= 0.0) {
		auto [oldPosX, oldPosY, oldPosZ] = _owner._position.GetVec3();
		_owner._position = AppFrame::Math::Vector4(oldPosX, 0.0, oldPosZ);
		auto efcFall = std::make_unique<Effect::EffectObjFall>(_owner._gameMain, "ObjFall");
		efcFall->SetPosition(_owner._position);
		efcFall->SetSpeed(2.0);
		_owner.GetEfcServer().Add(std::move(efcFall));
		_owner._stateServer->PushBack("Idle");
	}

	_owner._collisionComponent->PlayerFromFallObjectModel(_owner._isFall);

	++_owner._fallTimer;
}

void FallObject::StateFall::Draw() {
	FallObject::StateBase::Draw();
	auto pointPosition = _owner._position;
	pointPosition.SetY(FallPointPosY);
	_owner.GetTexComponent().DrawBillBoard(pointPosition, FallPointScale,DefaultPointAngle, _owner._fallPointHandles, FallPointAnimeSpeed);
}

void FallObject::StateSave::Enter() {
	_owner._vecBeforeSave = _owner._position;

	_owner.residual(false);
}

void FallObject::StateSave::Input(InputManager& input) {
	if (input.GetXJoypad().RBClick()) {
		_owner._stateServer->PushBack("Shoot");
	}
}

void FallObject::StateSave::Update() {
    
	if (!_owner._saveFlag) {
		_owner.Up();
	}
	else {
		_owner.Save();
	}
	_owner._collisionComponent->PlayerKnockBack();
	_owner.CheckPlayerKnockBack();
}

void FallObject::StateShoot::Enter() {
	_owner._shootVec = _owner.GetObjServer().GetVecData("CamTarget") - _owner._position;
	_owner._shootVec.Normalized();
}

void FallObject::StateShoot::Input(InputManager& input) {

}

void FallObject::StateShoot::Update() {
	_owner.Shoot();

	_owner._collisionComponent->ObjectModelFromLargeEnemy();
	_owner._collisionComponent->PoorEnemyGatlingFromObjectModel();
	_owner._collisionComponent->FallObjectFromLaser();
	_owner._collisionComponent->OutStage();
	_owner.HitCheckFromLargeEnemy();
	_owner.HitCheckFromLaser();
	_owner.HitCheckFromPoorEnemyGatling();
	_owner.OutStageCheck();
}

void FallObject::StateDie::Enter() {

}

void FallObject::StateDie::Input(InputManager& input) {

}

void FallObject::StateDie::Update() {
	_owner.SetDead();
}
