
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
   auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("poorenemy",
      { "gravity", "rotate_speed"});

   const double Gravity = paramMap["gravity"];
   const double RotateSpeed = paramMap["rotate_speed"];
}

PoorEnemyGatling::PoorEnemyGatling(Game::GameMain& gameMain) : PoorEnemyBase{ gameMain } {

}

void PoorEnemyGatling::Init() {
   PoorEnemyBase::Init();
   _actionList.emplace_back("Gatling");
}

void PoorEnemyGatling::CreateGatling() {
   auto gatlingFramePos = modelAnimeComponent().GetFrameChildPosion("root","mob_gun");
   _gameMain.objServer().RegistVector("GatlingPos", gatlingFramePos);
   _gameMain.objServer().RegistVector("GatlingMoveDirection", _gatlingMoveDirection);
   auto gatling = _gameMain.objFactory().Create("Gatling");
   gameMain().objServer().Add(std::move(gatling));
}

void PoorEnemyGatling::StateGatling::Enter() {
   _owner._modelAnimeComponent->ChangeAnime("attack", true);
   _owner._gatlingMoveDirection = _owner.GetObjServer().GetVecData("PlayerPos") - _owner._position;
   _stateCnt = _owner._gameMain.modeServer().frameCount();
   _remainingGatiling = 5;
}

void PoorEnemyGatling::StateGatling::Update() {
   auto frame = _owner._gameMain.modeServer().frameCount() - _stateCnt;
   if (frame % 60 == 0) {
      _owner.CreateGatling();
      --_remainingGatiling;
      if (_remainingGatiling <= 0) {
         _owner._stateServer->GoToState("Idle");
      }
   }
   _owner._collisionComponent->BulletFromPoorEnemy();
   _owner._collisionComponent->PoorEnemyFromPlayer();
   _owner.HitCheckFromBullet();
   _owner.HitCheckFromFallObject();
}
