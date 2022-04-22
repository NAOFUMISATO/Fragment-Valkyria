
/*****************************************************************//**
 * \file   EffectGatlingMuzzleFlash.cpp
 * \brief  �K�g�����O�̔��Ή��G�t�F�N�g
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "EffectGatlingMuzzleFlash.h"
#include "GameMain.h"

using namespace FragmentValkyria::Effect;

EffectGatlingMuzzleFlash::EffectGatlingMuzzleFlash(std::string_view key) :EffectBase{key } {
   SetEffectLoadHandle(key);
}

void EffectGatlingMuzzleFlash::Init() {
   PlayEffect();
}

void EffectGatlingMuzzleFlash::Update() {
   if (!IsPlaying()) {
      SetDead();
   }
   EffectBase::Update();
}
