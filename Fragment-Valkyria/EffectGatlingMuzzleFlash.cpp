
/*****************************************************************//**
 * \file   EffectGatlingMuzzleFlash.cpp
 * \brief  ガトリングの発火炎エフェクト
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "EffectGatlingMuzzleFlash.h"
#include "GameMain.h"

using namespace FragmentValkyria::Effect;

EffectGatlingMuzzleFlash::EffectGatlingMuzzleFlash(Game::GameMain& gameMain, std::string_view key) :EffectBase{ gameMain,key } {
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
