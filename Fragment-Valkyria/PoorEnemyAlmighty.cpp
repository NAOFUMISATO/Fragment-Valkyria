
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
#include "GameMain.h"
#include "ObjectServer.h"
#include "ParamPoorEnemy.h"

using namespace FragmentValkyria::Enemy;

PoorEnemyAlmighty::PoorEnemyAlmighty(Game::GameMain& gameMain) : PoorEnemyBase{ gameMain } {
   _param = std::make_unique<Param::ParamPoorEnemy>(_gameMain, "poorenemy");
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
   _gameMain.objServer().RegistVector("GatlingPos", gatlingFramePos);
   _gameMain.objServer().RegistVector("GatlingMoveDirection", _gatlingMoveDirection);
   auto gatling = _gameMain.objFactory().Create("Gatling");
   _gameMain.objServer().Add(std::move(gatling));
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
   _stateCnt = _owner._gameMain.modeServer().frameCount();
   _moved = _owner.GetObjServer().GetVecData("PlayerPos") - _owner._position;
   _moved.Normalized();
   _moved = _moved * _DoubleParam("rush_speed");
}

void PoorEnemyAlmighty::StateRush::Update() {
   auto frame = static_cast<int>(_owner._gameMain.modeServer().frameCount() - _stateCnt);
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
   _owner._gatlingMoveDirection = _owner.GetObjServer().GetVecData("PlayerPos") - _owner._position;
   _stateCnt = _owner._gameMain.modeServer().frameCount();
   _remainingGatiling = _owner._param->GetIntParam("max_gatling");
}

void PoorEnemyAlmighty::StateGatling::Update() {
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
