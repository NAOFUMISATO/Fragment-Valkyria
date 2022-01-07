
/*****************************************************************//**
 * \file   LargeEnemy.cpp
 * \brief  
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "LargeEnemy.h"
#include "GameMain.h"
#include "ObjectFactory.h"
#include "ModelAnimeComponent.h"

using namespace FragmentValkyria::LargeEnemy;

LargeEnemy::LargeEnemy(Game::GameMain& gameMain) : ObjectBase{ gameMain } {
	
}

void LargeEnemy::Init() {

}

void LargeEnemy::Input(InputManager& input) {
	_stateServer->Input(input);
}

void LargeEnemy::Update() {
	// 状態の更新
	_stateServer->Update();
	// ワールド行列の更新
	ComputeWorldTransform();
	// モデルの更新
	_modelAnimeComponent->Update();
}

void LargeEnemy::Draw() {
	_stateServer->Draw();
}

void LargeEnemy::CreateFallObject() {
	auto fallObject = gameMain().objFactory().Create("FallObject");
	gameMain().objServer().Add(std::move(fallObject));
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
		_owner._stateServer->PushBack("FallObject");
	}

	++_owner._stateCnt;
}

void LargeEnemy::StateFallObject::Enter() {
	_owner._stateCnt = 0;
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

	++_owner._stateCnt;
}
