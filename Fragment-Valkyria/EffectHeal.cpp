
/*****************************************************************//**
 * \file   EffectHeal.cpp
 * \brief  回復エフェクト
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "EffectHeal.h"

using namespace FragmentValkyria::Effect;

EffectHeal::EffectHeal(std::string_view key) :EffectBase{key } {
   SetEffectLoadHandle(key);
}

void EffectHeal::Init() {
   PlayEffect();
}

void EffectHeal::Update() {
   if (!IsPlaying()) {
      SetDead();
   }
   EffectBase::Update();
}

