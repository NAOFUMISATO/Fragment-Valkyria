
/*****************************************************************//**
 * \file   EffectGatlingBullet.cpp
 * \brief  敵のガトリング弾エフェクト
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "EffectGatlingBullet.h"

using namespace FragmentValkyria::Effect;

EffectGatlingBullet::EffectGatlingBullet(std::string_view key) :EffectBase{key } {
   SetEffectLoadHandle(key);
}

void EffectGatlingBullet::Init() {
   PlayEffect();
}

void EffectGatlingBullet::Update() {
   if (!IsPlaying()) {
      SetDead();
   }
   EffectBase::Update();
}
