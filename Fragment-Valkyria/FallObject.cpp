
/*****************************************************************//**
 * \file   FallObject.cpp
 * \brief  
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "FallObject.h"
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

void FallObject::StateBase::Draw() {
	_owner._modelAnimeComponent->Draw();
}

void FallObject::StateIdle::Enter() {

}

void FallObject::StateIdle::Input(InputManager& input) {

}

void FallObject::StateIdle::Update() {

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

	++_owner._fallTimer;
}
