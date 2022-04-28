
/*****************************************************************//**
 * \file   EffectBossFall.cpp
 * \brief  ボスの落下エフェクト
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "EffectBossFall.h"

using namespace FragmentValkyria::Effect;

EffectBossFall::EffectBossFall(std::string_view key) :EffectBase{key } {
   SetEffectLoadHandle(key);
}

void EffectBossFall::Init() {
   PlayEffect();
}

void EffectBossFall::Update() {
   if (!IsPlaying()) {
      SetDead();
   }
   EffectBase::Update();
}

