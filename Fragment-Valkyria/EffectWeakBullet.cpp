
/*****************************************************************//**
 * \file   EffectWeakBullet.cpp
 * \brief  �v���C���[��U���G�t�F�N�g
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "EffectWeakBullet.h"
#include "GameMain.h"

using namespace FragmentValkyria::Effect;

EffectWeakBullet::EffectWeakBullet(Game::GameMain& gameMain, std::string_view key) :EffectBase{ gameMain,key } {
   SetEffectLoadHandle(key);
}

void EffectWeakBullet::Init() {
   PlayEffect();
}

void EffectWeakBullet::Update() {
   if (!IsPlaying()) {
      SetDead();
   }
   EffectBase::Update();
}


