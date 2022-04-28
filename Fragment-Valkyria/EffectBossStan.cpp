
/*****************************************************************//**
 * \file   EffectBossStan.cpp
 * \brief  ボスのスタンエフェクト
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "EffectBossStan.h"

using namespace FragmentValkyria::Effect;

EffectBossStan::EffectBossStan(std::string_view key) :EffectBase{ key } {
   SetEffectLoadHandle(key);
}

void EffectBossStan::Init() {
   PlayEffect();
}

void EffectBossStan::Update() {
   if (!IsPlaying()) {
      SetDead();
   }
   EffectBase::Update();
}

