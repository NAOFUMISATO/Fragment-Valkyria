
/*****************************************************************//**
 * \file   EffectBossCharge.cpp
 * \brief  ボスのビームチャージエフェクト
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "EffectBossCharge.h"
#include "Game.h"
#include "ObjectServer.h"

using namespace FragmentValkyria::Effect;

EffectBossCharge::EffectBossCharge(std::string_view key) :EffectBase{ key } {
   SetEffectLoadHandle(key);
}

void EffectBossCharge::Init() {
   PlayEffect();
}

void EffectBossCharge::Update() {
   auto& objServer = Game::Game::GetObjServer();
   auto laserPos = objServer.GetVecData("LaserPos");
   _position = laserPos;
   auto bossPos = objServer.GetVecData("LargeEnemyPos");
   auto playerPos= objServer.GetVecData("PlayerPos");
   auto [x, y, z] = (bossPos - playerPos).GetVec3();
   _rotation = AppFrame::Math::Vector4(0,std::atan2(-x,-z),0);
   if (!IsPlaying()) {
      SetDead();
   }
   EffectBase::Update();
}
