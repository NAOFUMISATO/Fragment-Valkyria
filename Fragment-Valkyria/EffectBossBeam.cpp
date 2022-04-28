
/*****************************************************************//**
 * \file   EffectBossBeam.cpp
 * \brief  �{�X�̃r�[���G�t�F�N�g
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
