
/*****************************************************************//**
 * \file   EffectObjectHit.cpp
 * \brief  �I�u�W�F�N�g�Փ˃G�t�F�N�g
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "EffectObjectHit.h"

using namespace FragmentValkyria::Effect;

EffectObjectHit::EffectObjectHit(std::string_view key) :EffectBase{key } {
   SetEffectLoadHandle(key);
}

void EffectObjectHit::Init() {
   PlayEffect();
}

void EffectObjectHit::Update() {
   if (!IsPlaying()) {
      SetDead();
   }
   EffectBase::Update();
}
