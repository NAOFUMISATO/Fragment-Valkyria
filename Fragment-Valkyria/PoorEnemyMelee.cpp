
/*****************************************************************//**
 * \file   PoorEnemyMelee.cpp
 * \brief  ‹ßÚUŒ‚‚ÌŽG‹›“G
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "PoorEnemyMelee.h"
#include "CollisionComponent.h"
#include "ModelAnimeComponent.h"
#include "ObjectFactory.h"
#include "GameMain.h"
#include "ObjectServer.h"

namespace {
   // Jsonƒtƒ@ƒCƒ‹‚©‚çŠe’l‚ðŽæ“¾‚·‚é
   auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("poorenemy",
      { "rush_speed", "rush_frame","rush_animespeed" });

   const double RushSpeed = paramMap["rush_speed"];
   const int RushFrame = paramMap["rush_frame"];
   const double RushAnimeSpeed = paramMap["rush_animespeed"];
}

using namespace FragmentValkyria::Enemy;

PoorEnemyMelee::PoorEnemyMelee(Game::GameMain& gameMain) : PoorEnemyBase{ gameMain } {

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
   _owner._modelAnimeComponent->ChangeAnime("walk", true, RushAnimeSpeed);
   _stateCnt = _owner._gameMain.modeServer().frameCount();
   _moved = _owner.GetObjServer().GetVecData("PlayerPos") - _owner._position;
   _moved.Normalized();
   _moved = _moved * RushSpeed;
}

void PoorEnemyMelee::StateRush::Update() {
   auto frame = static_cast<int>(_owner._gameMain.modeServer().frameCount() - _stateCnt);
   if (frame <= RushFrame) {
      _owner.Rush(_moved);
   }
   else {
      _owner._stateServer->GoToState("Idle");
   }
   StateBase::Update();
}
