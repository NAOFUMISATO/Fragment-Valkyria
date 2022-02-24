
/*****************************************************************//**
 * \file   PoorEnemyGatling.cpp
 * \brief  ƒKƒgƒŠƒ“ƒOUŒ‚‚ð‚µ‚Ä‚­‚éŽG‹›“G‚Ìˆ—‚ð‰ñ‚·ƒNƒ‰ƒX
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "PoorEnemyGatling.h"
#include "CollisionComponent.h"
#include "ModelAnimeComponent.h"
#include "ObjectFactory.h"
#include "GameMain.h"
#include "ObjectServer.h"

using namespace FragmentValkyria::Enemy;

namespace {
	auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("poorenemygatling",
		{ "gravity", "rotate_speed"});

	const double Gravity = paramMap["gravity"];
	const double RotateSpeed = paramMap["rotate_speed"];
}

PoorEnemyGatling::PoorEnemyGatling(Game::GameMain& gameMain) : PoorEnemyBase{ gameMain } {

}

void PoorEnemyGatling::Init() {
	PoorEnemyBase::Init();
	_actionList.emplace_back("FallObject");
	_actionList.emplace_back("Gatling");
	_actionList.emplace_back("Move");
	_actionList.emplace_back("Laser");
}

void PoorEnemyGatling::CreateGatling() {
	auto handle = modelAnimeComponent().modelHandle();
	auto gatlingFrame = modelAnimeComponent().FindFrame("Spider_Armature");
	auto gatlingPos = MV1GetFramePosition(handle, gatlingFrame);
	_gameMain.objServer().RegistVector("GatlingPos", AppFrame::Math::ToMath(gatlingPos));
	_gameMain.objServer().RegistVector("GatlingMoveDirection", _gatlingMoveDirection);
	auto gatling = _gameMain.objFactory().Create("Gatling");
	gameMain().objServer().Add(std::move(gatling));
}

void PoorEnemyGatling::StateIdle::Enter() {
	PoorEnemyBase::StateIdle::Enter();
}

void PoorEnemyGatling::StateIdle::Update() {
	if (_owner._stateCnt >= 60 * 7) {
		_owner._stateServer->GoToState("Gatling");
	}
	PoorEnemyBase::StateIdle::Update();
}

void PoorEnemyGatling::StateGatling::Enter() {
	_owner._modelAnimeComponent->ChangeAnime("Spider_Armature|Jump", true);
	_owner._gatlingMoveDirection = _owner.GetObjServer().GetVecData("PlayerPos") - _owner._position;
	_owner._stateCnt = 0;
	_gatlingCnt = 5;
}

void PoorEnemyGatling::StateGatling::Update() {
	if (_owner._stateCnt % 60 == 0) {
		_owner.CreateGatling();
		--_gatlingCnt;
		if (_gatlingCnt <= 0) {
			_owner._stateServer->GoToState("Idle");
		}
	}
	_owner._collisionComponent->BulletFromPoorEnemyGatling();
	_owner._collisionComponent->PoorEnemyFromPlayer();
	_owner.HitCheckFromBullet();
	_owner.HitCheckFromFallObject();
	++_owner._stateCnt;
}
