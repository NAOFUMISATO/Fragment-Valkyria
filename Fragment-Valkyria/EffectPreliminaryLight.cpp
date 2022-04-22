
/*****************************************************************//**
 * \file   EffectPreliminaryLight.cpp
 * \brief  �{�X�̗\������G�t�F�N�g
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "EffectPreliminaryLight.h"
#include "GameMain.h"

using namespace FragmentValkyria::Effect;

EffectPreliminaryLight::EffectPreliminaryLight(std::string_view key) :EffectBase{key } {
   SetEffectLoadHandle(key);
}

void EffectPreliminaryLight::Init() {
   PlayEffect();
}

void EffectPreliminaryLight::Update() {
   if (!IsPlaying()) {
      SetDead();
   }
   EffectBase::Update();
}
