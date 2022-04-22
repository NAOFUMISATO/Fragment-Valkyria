
/*****************************************************************//**
 * \file   PoorEnemyGatling.cpp
 * \brief  ƒKƒgƒŠƒ“ƒOUŒ‚‚ğ‚µ‚Ä‚­‚éG‹›“G‚Ìˆ—‚ğ‰ñ‚·ƒNƒ‰ƒX
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
#include "ParamPoorEnemy.h"

using namespace FragmentValkyria::Enemy;

PoorEnemyGatling::PoorEnemyGatling() {
   _param = std::make_unique<Param::ParamPoorEnemy>("poorenemy");
}

void PoorEnemyGatling::Init() {
   PoorEnemyBase::Init();
   _actionList.emplace_back("Gatling");
}

void PoorEnemyGatling::CreateGatling() {
   auto gatlingFramePos = modelAnimeComponent().GetFrameChildPosion("root","mob_gun");
   auto& objServer = GetObjServer();
   objServer.RegistVector("GatlingPos", gatlingFramePos);
   objServer.RegistVector("GatlingMoveDirection", _gatlingMoveDirection);
   auto gameInstance = Game::GameMain::GetInstance();
   auto gatling = gameInstance->objFactory().Create("Gatling");
   objServer.Add(std::move(gatling));
}

void PoorEnemyGatling::StateGatling::Enter() {
   _owner._modelAnimeComponent->ChangeAnime("attack", true,
      _owner._param->GetDoubleParam("gatling_animespeed"));
   _owner._gatlingMoveDirection = _owner.GetObjServer().GetVecData("PlayerPos") - _owner._position;
   auto gameInstance = Game::GameMain::GetInstance();
   _stateCnt = gameInstance->modeServer().frameCount();
   _remainingGatiling = _owner._param->GetIntParam("max_gatling");
}

void PoorEnemyGatling::StateGatling::Update() {
   auto gameInstance = Game::GameMain::GetInstance();
   auto frame = gameInstance->modeServer().frameCount() - _stateCnt;
   if (frame % _owner._param->GetIntParam("gatling_rate") == 0) {
      _owner.CreateGatling();
      --_remainingGatiling;
      if (_remainingGatiling <= 0) {
         _owner._stateServer->GoToState("Idle");
      }
   }
   _owner._collisionComponent->BulletFromPoorEnemy();
   _owner._collisionComponent->PoorEnemyFromPlayer();
   StateBase::Update();
}
