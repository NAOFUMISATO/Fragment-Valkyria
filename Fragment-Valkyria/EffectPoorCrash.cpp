
/*****************************************************************//**
 * \file   EffectPoorCrash.cpp
 * \brief  雑魚破壊エフェクト
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "EffectPoorCrash.h"
#include "GameMain.h"

using namespace FragmentValkyria::Effect;

EffectPoorCrash::EffectPoorCrash(std::string_view key) :EffectBase{ key } {
   SetEffectLoadHandle(key);
}

void EffectPoorCrash::Init() {
   PlayEffect();
}

void EffectPoorCrash::Update() {
   if (!IsPlaying()) {
      SetDead();
   }
   EffectBase::Update();
}


