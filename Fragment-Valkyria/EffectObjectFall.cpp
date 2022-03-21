
/*****************************************************************//**
 * \file   EffectObjectFall.cpp
 * \brief  オブジェクト落下エフェクト
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "EffectObjectFall.h"
#include "GameMain.h"

using namespace FragmentValkyria::Effect;

EffectObjectFall::EffectObjectFall(Game::GameMain& gameMain, std::string_view key) :EffectBase{ gameMain,key } {
   SetEffectLoadHandle(key);
}

void EffectObjectFall::Init() {
   PlayEffect();
}

void EffectObjectFall::Update() {
   if (!IsPlaying()) {
      SetDead();
   }
   EffectBase::Update();
}
