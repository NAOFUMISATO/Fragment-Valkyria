
/*****************************************************************//**
 * \file   EffectBossFall.cpp
 * \brief  �{�X�̗����G�t�F�N�g
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

