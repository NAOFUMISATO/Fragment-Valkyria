
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

using namespace FragmentValkyria::Enemy;

FallObject::FallObject(Game::GameMain& gameMain) : ObjectBase{ gameMain } {

}

void FallObject::Init() {
	
}

void FallObject::Input(InputManager& input) {
	_stateServer->Input(input);
}

void FallObject::Update() {
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
	_position = _VecBeforeSave + Vector4(0.0, 300.0 + y, 0.0);

}

void FallObject::Up() {
	_position.Add(0.0, UpSpeed, 0.0);

	if (_position.GetY() > 300.0) {
		_saveFlag = true;
	}
}

void FallObject::StateBase::Draw() {
	_owner._modelAnimeComponent->Draw();
#ifdef _DEBUG
	auto pos = AppFrame::Math::ToDX(_owner._position);
	auto radian = static_cast<float>(_owner._range);
	DrawSphere3D(pos, radian, 10, GetColor(0, 0, 0), GetColor(0, 0, 0), FALSE);
#endif
}

void FallObject::StateIdle::Enter() {

}

void FallObject::StateIdle::Input(InputManager& input) {
	if (input.GetXJoypad().LeftTrigger() >= 20) {
		_owner.HitCheckFromPlayerPoint();
	}
}

void FallObject::StateIdle::Update() {
	_owner._collisionComponent->PlayerFromObjectRange();
}

void FallObject::StateFall::Enter() {
	_owner._fallTimer = 0.0;
}

void FallObject::StateFall::Input(InputManager& input) {
	
}

void FallObject::StateFall::Update() {
	auto posY = (0.5 * _owner.Gravity * _owner._fallTimer * _owner._fallTimer);

	_owner._position.Add(0.0, -posY, 0.0);

	if (_owner._position.GetY() <= 0.0) {
		auto [oldPosX, oldPosY, oldPosZ] = _owner._position.GetXYZ();
		_owner._position = AppFrame::Math::Vector4(oldPosX, 0.0, oldPosZ);
		_owner._stateServer->PushBack("Idle");
	}

	_owner._collisionComponent->PlayerFromObjectRange();

	++_owner._fallTimer;
}

void FallObject::StateSave::Enter() {
	_owner._VecBeforeSave = _owner._position;
}

void FallObject::StateSave::Input(InputManager& input) {

}

void FallObject::StateSave::Update() {
    
	if (!_owner._saveFlag) {
		_owner.Up();
	}
	else {
		_owner.Save();
	}
	
}
