
/*****************************************************************//**
 * \file   EffectObjectUp.cpp
 * \brief  �I�u�W�F�N�g�Փ˃G�t�F�N�g
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "EffectObjectUp.h"

using namespace FragmentValkyria::Effect;

EffectObjectUp::EffectObjectUp(std::string_view key) :EffectBase{key } {
   SetEffectLoadHandle(key);
}

void EffectObjectUp::Init() {
   PlayEffect();
}

void EffectObjectUp::Update() {
   if (!IsPlaying()) {
      SetDead();
   }
   EffectBase::Update();
}
