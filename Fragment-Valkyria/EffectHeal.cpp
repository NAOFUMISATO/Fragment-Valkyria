
/*****************************************************************//**
 * \file   EffectHeal.cpp
 * \brief  回復エフェクト
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "EffectHeal.h"
#include "GameMain.h"

using namespace FragmentValkyria::Effect;

EffectHeal::EffectHeal(Game::GameMain& gameMain, std::string_view key) :EffectBase{ gameMain,key } {
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

