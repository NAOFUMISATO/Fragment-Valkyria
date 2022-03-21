
/*****************************************************************//**
 * \file   EffectObjectUp.cpp
 * \brief  �I�u�W�F�N�g�Փ˃G�t�F�N�g
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "EffectObjectUp.h"
#include "GameMain.h"

using namespace FragmentValkyria::Effect;

EffectObjectUp::EffectObjectUp(Game::GameMain& gameMain, std::string_view key) :EffectBase{ gameMain,key } {
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
