
/*****************************************************************//**
 * \file   EffectBossStan.cpp
 * \brief  �{�X�̃X�^���G�t�F�N�g
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "EffectBossStan.h"
#include "GameMain.h"

using namespace FragmentValkyria::Effect;

EffectBossStan::EffectBossStan(Game::GameMain& gameMain, std::string_view key) :EffectBase{ gameMain,key } {
   SetEffectLoadHandle(key);
}

void EffectBossStan::Init() {
   PlayEffect();
}

void EffectBossStan::Update() {
   if (!IsPlaying()) {
      SetDead();
   }
   EffectBase::Update();
}

