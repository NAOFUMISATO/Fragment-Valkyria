
/*****************************************************************//**
 * \file   Gatling.cpp
 * \brief  
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "Gatling.h"
#include "ModelAnimeComponent.h"

using namespace FragmentValkyria::Enemy;

Gatling::Gatling(Game::GameMain& gameMain) : ObjectBase{ gameMain } {

}

void Gatling::Init() {
	_moveDirection = GetObjServer().GetPosition("PlayerPos") - _position;
	auto [x, y, z] = _moveDirection.GetXYZ();
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
	_modelAnimeComponent->Update();
}

void Gatling::Draw() {
	_stateServer->Draw();
}

void Gatling::Move() {
	_moved = _moveDirection * Speed;
	_position = _position + _moved;
}

void Gatling::StateBase::Draw() {
	_owner._modelAnimeComponent->Draw();
	DrawSphere3D(AppFrame::Math::ToDX(_owner._position), static_cast<float>(_owner._radian), 0, GetColor(255, 0, 0), GetColor(0, 0, 0), TRUE);
}

void Gatling::StateChase::Enter() {

}

void Gatling::StateChase::Input(InputManager& input) {

}

void Gatling::StateChase::Update() {
	_owner.Move();
}
