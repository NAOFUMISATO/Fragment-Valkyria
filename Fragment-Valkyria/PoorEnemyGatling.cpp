
/*****************************************************************//**
 * \file   PoorEnemyGatling.cpp
 * \brief  ガトリング攻撃をしてくる雑魚敵の処理を回すクラス
 * 
 * \author YUSUKEAHAMEDO
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

PoorEnemyGatling::PoorEnemyGatling(Game::GameMain& gameMain) : PoorEnemyBase{ gameMain } {
   _param = std::make_unique<Param::ParamPoorEnemy>(_gameMain, "poorenemy");
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
   _owner._modelAnimeComponent->ChangeAnime("attack", true,
      _owner._param->GetDoubleParam("gatling_animespeed"));
   _owner._gatlingMoveDirection = _owner.GetObjServer().GetVecData("PlayerPos") - _owner._position;
   _stateCnt = _owner._gameMain.modeServer().frameCount();
   _remainingGatiling = _owner._param->GetIntParam("max_gatling");
}

void PoorEnemyGatling::StateGatling::Update() {
   auto frame = _owner._gameMain.modeServer().frameCount() - _stateCnt;
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
