
/*****************************************************************//**
 * \file   EffectPlayerShot.cpp
 * \brief  プレイヤーの射撃エフェクト
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "EffectPlayerShot.h"
#include "GameMain.h"

using namespace FragmentValkyria::Effect;

EffectPlayerShot::EffectPlayerShot(Game::GameMain& gameMain) :EffectBase{gameMain} {
   Init("Shot");
}

EffectPlayerShot::~EffectPlayerShot() {
}

void EffectPlayerShot::Init(std::string_view key) {
   EffectBase::Init(key);
  _position=_gameMain.objServer().GetPosition("PlayerPos");
  _rotation=_gameMain.objServer().GetForward("PlayerFor");
  PlayEffect();
}

void EffectPlayerShot::Update() {
   if (!IsPlaying()) {
      SetDead();
   }
}

void EffectPlayerShot::Draw() {
   EffectBase::Draw();
}
