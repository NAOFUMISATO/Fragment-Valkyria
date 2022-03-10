
/*****************************************************************//**
 * \file   Laser.cpp
 * \brief  レーザーの処理を回すクラス
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "Laser.h"
#include "GameMain.h"
#include "EffectBossBeam.h"
#include "EffectServer.h"
#include "ObjectServer.h"
#include "LargeEnemy.h"

namespace {
   auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("collision",
      { "laser_radius" });
   const double Radius = paramMap["laser_radius"];
}

using namespace FragmentValkyria::Enemy;

Laser::Laser(Game::GameMain& gameMain) : Object::ObjectBase{ gameMain } {
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
   auto radius = static_cast<float>(Radius);
   DrawCapsule3D(firstPos, secondPos, radius, 5, 
      AppFrame::Math::Utility::GetColorCode(255, 255, 0), 
      AppFrame::Math::Utility::GetColorCode(255, 255, 255), FALSE);
#endif
}

void Laser::StateIrradiation::Enter() {
   // この状態になった時のゲームのフレームカウントの保存
   _stateCnt = _owner.gameMain().modeServer().frameCount();
   auto efcBeam = std::make_unique<Effect::EffectBossBeam>(_owner._gameMain,"BossBeam");
   efcBeam->position(_owner._position);
   auto [x, y, z] = (_owner._end - _owner._position).GetVec3();
   auto efcDir = Vector4(0, std::atan2(-x, -z), 0);
   efcBeam->rotation(efcDir);
   _owner.GetEfcServer().Add(std::move(efcBeam));
}

void Laser::StateIrradiation::Update() {
   auto count = _owner.gameMain().modeServer().frameCount();
   for (auto& object : _owner.GetObjServer().runObjects()) {
      if (object->GetObjType() == Object::ObjectBase::ObjectType::LargeEnemy) {
         auto& largeEnemy = dynamic_cast<Enemy::LargeEnemy&>(*object);
         if (!largeEnemy.isLaser()) {
            _owner.SetDead();
         }
      }
   }
}
