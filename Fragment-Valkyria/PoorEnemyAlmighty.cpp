
/*****************************************************************//**
 * \file   PoorEnemyAlmighty.cpp
 * \brief  全ての攻撃を行う雑魚敵
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "PoorEnemyAlmighty.h"
#include "CollisionComponent.h"
#include "ModelAnimeComponent.h"
#include "ObjectFactory.h"
#include "Game.h"
#include "ObjectServer.h"
#include "ParamPoorEnemy.h"

using namespace FragmentValkyria::Enemy;

PoorEnemyAlmighty::PoorEnemyAlmighty() {
   _param = std::make_unique<Param::ParamPoorEnemy>("poorenemy");
}

void PoorEnemyAlmighty::Init() {
   PoorEnemyBase::Init();
   _actionList.emplace_back("Rush");
   _actionList.emplace_back("Gatling");
}

void PoorEnemyAlmighty::Rush(const Vector4& moved) {
   auto [x, y, z] = moved.GetVec3();
   auto position = _position;
   position = _collisionComponent->PoorEnemyCheckStage(position, Vector4(x, y, 0.0));
   position = _collisionComponent->PoorEnemyCheckStage(position, Vector4(0.0, y, z));
   _position = position;
}

void PoorEnemyAlmighty::CreateGatling() {
   auto gatlingFramePos = _modelAnimeComponent->GetFrameChildPosion("root", "mob_gun");
   auto& objServer = Game::Game::GetObjServer();
   objServer.RegistVector("GatlingPos", gatlingFramePos);
   objServer.RegistVector("GatlingMoveDirection", _gatlingMoveDirection);
   auto& objFactory = Game::Game::GetObjFactory();
   auto gatling = objFactory.Create("Gatling");
   objServer.Add(std::move(gatling));
}

void PoorEnemyAlmighty::StateRush::Enter() {
   /**
    * \brief double型の値を文字列で指定し、値管理クラスから取得する
    * \param paramName 値を指定する文字列
    * \return 文字列により指定された値
    */
   const auto _DoubleParam = [&](std::string paramName) {
      return _owner._param->GetDoubleParam(paramName);
   };
   _owner._modelAnimeComponent->ChangeAnime("walk", true, _DoubleParam("rush_animespeed"));
   auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
   _stateCnt = modeServer.frameCount();
   auto& objServer = Game::Game::GetObjServer();
   auto moved = objServer.GetVecData("PlayerPos") - _owner._position;
   auto [x, y, z] = moved.GetVec3();
   _moved = Vector4(x, 0.0, z);
   _moved.Normalized();
   _moved = _moved * _DoubleParam("rush_speed");
}

void PoorEnemyAlmighty::StateRush::Update() {
   auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
   auto frame = static_cast<int>(modeServer.frameCount() - _stateCnt);
   if (frame <= _owner._param->GetIntParam("rush_frame")) {
      _owner.Rush(_moved);
   }
   else {
      _owner._stateServer->GoToState("Idle");
   }
   StateBase::Update();
}

void PoorEnemyAlmighty::StateGatling::Enter() {
   _owner._modelAnimeComponent->ChangeAnime("attack", true, 
      _owner._param->GetDoubleParam("gatling_animespeed"));
   auto& objServer = Game::Game::GetObjServer();
   _owner._gatlingMoveDirection = objServer.GetVecData("PlayerPos") - _owner._position;
   auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
   _stateCnt = modeServer.frameCount();
   _remainingGatiling = _owner._param->GetIntParam("max_gatling");
}

void PoorEnemyAlmighty::StateGatling::Update() {
   auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
   auto frame = modeServer.frameCount() - _stateCnt;
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
