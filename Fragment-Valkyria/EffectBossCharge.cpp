
/*****************************************************************//**
 * \file   EffectBossCharge.cpp
 * \brief  ボスのビームチャージエフェクト
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "EffectBossCharge.h"
#include "GameMain.h"
#include "ObjectServer.h"

using namespace FragmentValkyria::Effect;

EffectBossCharge::EffectBossCharge(Game::GameMain& gameMain, std::string_view key) :EffectBase{ gameMain,key } {
   SetEffectLoadHandle(key);
}

void EffectBossCharge::Init() {
   PlayEffect();
}

void EffectBossCharge::Update() {
   auto laserPos = _gameMain.objServer().GetVecData("LaserPos");
   _position = laserPos;
   auto bossPos = _gameMain.objServer().GetVecData("LargeEnemyPos");
   auto playerPos= _gameMain.objServer().GetVecData("PlayerPos");
   auto [x, y, z] = (bossPos - playerPos).GetVec3();
   _rotation = AppFrame::Math::Vector4(0,std::atan2(-x,-z),0);
   if (!IsPlaying()) {
      SetDead();
   }
   EffectBase::Update();
}
