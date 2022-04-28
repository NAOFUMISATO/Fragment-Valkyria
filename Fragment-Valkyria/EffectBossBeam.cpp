
/*****************************************************************//**
 * \file   EffectBossBeam.cpp
 * \brief  ボスのビームエフェクト
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "EffectBossBeam.h"

using namespace FragmentValkyria::Effect;

EffectBossBeam::EffectBossBeam(std::string_view key) :EffectBase{key } {
   SetEffectLoadHandle(key);
}

void EffectBossBeam::Init() {
   PlayEffect();
}

void EffectBossBeam::Update() {
   if (!IsPlaying()) {
      SetDead();
   }
   EffectBase::Update();
}
