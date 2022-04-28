
/*****************************************************************//**
 * \file   EffectWeakBullet.cpp
 * \brief  プレイヤー弱攻撃エフェクト
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "EffectWeakBullet.h"

using namespace FragmentValkyria::Effect;

EffectWeakBullet::EffectWeakBullet(std::string_view key) :EffectBase{key } {
   SetEffectLoadHandle(key);
}

void EffectWeakBullet::Init() {
   PlayEffect();
}

void EffectWeakBullet::Update() {
   if (!IsPlaying()) {
      SetDead();
   }
   EffectBase::Update();
}


