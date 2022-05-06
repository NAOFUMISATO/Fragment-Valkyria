
/*****************************************************************//**
 * \file   Laser.cpp
 * \brief  レーザーの処理を回すクラス
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "Laser.h"
#include "Game.h"
#include "EffectBossBeam.h"
#include "EffectServer.h"
#include "ObjectServer.h"
#include "LargeEnemy.h"
#include "ParamCollision.h"

using namespace FragmentValkyria::Enemy;

Laser::Laser() {
   _param = std::make_unique<Param::ParamCollision>("collision");
}

void Laser::Update() {
   _stateServer->Update();
}

void Laser::Draw() {
   _stateServer->Draw();
}

void Laser::StateBase::Draw() {
#ifdef _DEBUG
   auto firstPos = AppFrame::Math::ToDX(_owner._position);
   auto secondPos = AppFrame::Math::ToDX(_owner._end);
   auto radius = static_cast<float>(_owner._param->GetDoubleParam("laser_radius"));
   DrawCapsule3D(firstPos, secondPos, radius, 5, 
      AppFrame::Math::Utility::GetColorCode(255, 255, 0), 
      AppFrame::Math::Utility::GetColorCode(255, 255, 255), FALSE);
#endif
}

void Laser::StateIrradiation::Enter() {
   // この状態になった時のゲームのフレームカウントの保存
   auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
   _stateCnt = modeServer.frameCount();
   auto efcBeam = std::make_unique<Effect::EffectBossBeam>("BossBeam");
   efcBeam->position(_owner._position);
   auto [x, y, z] = (_owner._end - _owner._position).GetVec3();
   auto efcDir = Vector4(0, std::atan2(-x, -z), 0);
   efcBeam->rotation(efcDir);
   auto& efcServer = Game::Game::GetEfcServer();
   efcServer.Add(std::move(efcBeam));
}

void Laser::StateIrradiation::Update() {
   auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
   auto count = modeServer.frameCount();
   auto& runObjects = Game::Game::GetObjServer().runObjects();
   for (auto& object : runObjects) {
      if (object->GetObjType() == Object::ObjectBase::ObjectType::LargeEnemy) {
         auto& largeEnemy = dynamic_cast<Enemy::LargeEnemy&>(*object);
         if (!largeEnemy.isLaser()) {
            _owner.SetDead();
         }
      }
   }
}
