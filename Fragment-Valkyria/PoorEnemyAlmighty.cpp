
#include "PoorEnemyAlmighty.h"
#include "CollisionComponent.h"
#include "ModelAnimeComponent.h"
#include "ObjectFactory.h"
#include "GameMain.h"
#include "ObjectServer.h"

namespace {
   auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("poorenemy", {
      "max_gatling","gatling_rate","gatling_animespeed","rush_speed", "rush_frame","rush_animespeed" });
   const int MaxGatling = paramMap["max_gatling"];
   const int GatlingRate = paramMap["gatling_rate"];
   const double GatlingAnimeSpeed = paramMap["gatling_animespeed"];
   const double RushSpeed = paramMap["rush_speed"];
   const int RushFrame = paramMap["rush_frame"];
   const double RushAnimeSpeed = paramMap["rush_animespeed"];
}

using namespace FragmentValkyria::Enemy;

PoorEnemyAlmighty::PoorEnemyAlmighty(Game::GameMain& gameMain) : PoorEnemyBase{ gameMain } {
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
   _owner._modelAnimeComponent->ChangeAnime("walk", true, RushAnimeSpeed);
   _stateCnt = _owner._gameMain.modeServer().frameCount();
   _moved = _owner.GetObjServer().GetVecData("PlayerPos") - _owner._position;
   _moved.Normalized();
   _moved = _moved * RushSpeed;
}

void PoorEnemyAlmighty::StateRush::Update() {
   auto frame = static_cast<int>(_owner._gameMain.modeServer().frameCount() - _stateCnt);
   if (frame <= RushFrame) {
      _owner.Rush(_moved);
   }
   else {
      _owner._stateServer->GoToState("Idle");
   }
}

void PoorEnemyAlmighty::StateGatling::Enter() {
   _owner._modelAnimeComponent->ChangeAnime("attack", true, GatlingAnimeSpeed);
   _owner._gatlingMoveDirection = _owner.GetObjServer().GetVecData("PlayerPos") - _owner._position;
   _stateCnt = _owner._gameMain.modeServer().frameCount();
   _remainingGatiling = MaxGatling;
}

void PoorEnemyAlmighty::StateGatling::Update() {
   auto frame = _owner._gameMain.modeServer().frameCount() - _stateCnt;
   if (frame % GatlingRate == 0) {
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
