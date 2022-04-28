
/*****************************************************************//**
 * \file   PoorEnemyMelee.cpp
 * \brief  �ߐڍU���̎G���G
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "PoorEnemyMelee.h"
#include "CollisionComponent.h"
#include "ModelAnimeComponent.h"
#include "ObjectFactory.h"
#include "Game.h"
#include "ObjectServer.h"
#include "ParamPoorEnemy.h"

using namespace FragmentValkyria::Enemy;

PoorEnemyMelee::PoorEnemyMelee() {
   _param = std::make_unique<Param::ParamPoorEnemy>("poorenemy");
}

void PoorEnemyMelee::Init() {
   PoorEnemyBase::Init();
   _actionList.emplace_back("Rush");
}

void PoorEnemyMelee::Rush(const Vector4& moved) {
   auto [x, y, z] = moved.GetVec3();
   auto position = _position;
   position = _collisionComponent->PoorEnemyCheckStage(position, Vector4(x, y, 0.0));
   position = _collisionComponent->PoorEnemyCheckStage(position, Vector4(0.0, y, z));
   _position = position;
}

void PoorEnemyMelee::StateRush::Enter() {
   /**
    * \brief double�^�̒l�𕶎���Ŏw�肵�A�l�Ǘ��N���X����擾����
    * \param paramName �l���w�肷�镶����
    * \return ������ɂ��w�肳�ꂽ�l
    */
   const auto _DoubleParam = [&](std::string paramName) {
      return _owner._param->GetDoubleParam(paramName);
   };
   _owner._modelAnimeComponent->ChangeAnime("walk", true, _DoubleParam("rush_animespeed"));
   auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
   _stateCnt = modeServer.frameCount();
   auto& objServer = Game::Game::GetInstance().objServer();
   _moved = objServer.GetVecData("PlayerPos") - _owner._position;
   _moved.Normalized();
   _moved = _moved * _DoubleParam("rush_speed");
}

void PoorEnemyMelee::StateRush::Update() {
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
