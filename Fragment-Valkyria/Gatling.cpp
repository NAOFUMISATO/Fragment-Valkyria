
/*****************************************************************//**
 * \file   Gatling.cpp
 * \brief  
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "Gatling.h"
#include "CollisionComponent.h"
#include "ModelAnimeComponent.h"
#include "ObjectServer.h"

using namespace FragmentValkyria::Enemy;

namespace {
	auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("gatling",
		{ "speed", "radius"});

	const double Speed = paramMap["speed"];
	const double Radius = paramMap["radius"];
}

Gatling::Gatling(Game::GameMain& gameMain) : ObjectBase{ gameMain } {

}

void Gatling::Init() {
	_moveDirection = GetObjServer().GetVecData("GatlingMoveDirection");
	auto [x, y, z] = _moveDirection.GetVec3();
	_moveDirection = Vector4(x, 0.0, z);
	_moveDirection.Normalized();
}

void Gatling::Input(InputManager& input) {
	_stateServer->Input(input);
}

void Gatling::Update() {
	// 状態の更新
	_stateServer->Update();
	// ワールド行列の更新
	ComputeWorldTransform();
	// モデルの更新
	/*_modelAnimeComponent->Update();*/
}

void Gatling::Draw() {
	_stateServer->Draw();
}

void Gatling::Move() {
	_moved = _moveDirection * Speed;
	_position = _position + _moved;
}

void Gatling::HitCheckFromObjectModel() {
	auto report = _collisionComponent->report();
	if (report.id() == Collision::CollisionComponent::ReportId::HitFromIdleFallObject) {
		_stateServer->PushBack("Die");
	}
}

void Gatling::HitCheckFromPlayer() {
	auto report = _collisionComponent->report();
	if (report.id() == Collision::CollisionComponent::ReportId::HitFromPlayer) {
		_stateServer->PushBack("Die");
	}
}

void Gatling::OutStageCheck() {
	auto report = _collisionComponent->report();
	if (report.id() == Collision::CollisionComponent::ReportId::OutStage) {
		_stateServer->PushBack("Die");
	}
}

void Gatling::StateBase::Draw() {
	/*_owner._modelAnimeComponent->Draw();*/
	auto position = AppFrame::Math::ToDX(_owner._position);
	auto radian = static_cast<float>(Radius);
	
	DrawSphere3D(position, radian, 10, GetColor(255, 0, 0), GetColor(0, 0, 0), TRUE);
	
}

void Gatling::StateChase::Enter() {

}

void Gatling::StateChase::Input(InputManager& input) {

}

void Gatling::StateChase::Update() {
	_owner.Move();
	_owner._collisionComponent->OutStage();
	_owner.HitCheckFromPlayer();
	_owner.HitCheckFromObjectModel();
	_owner.OutStageCheck();
}

void Gatling::StateDie::Enter() {

}

void Gatling::StateDie::Input(InputManager& input) {

}

void Gatling::StateDie::Update() {
	_owner.SetDead();
}

void Gatling::StateDie::Draw() {
	auto position = AppFrame::Math::ToDX(_owner._position);
	auto radian = static_cast<float>(Radius);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawSphere3D(position, radian, 10, GetColor(255, 0, 0), GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}
