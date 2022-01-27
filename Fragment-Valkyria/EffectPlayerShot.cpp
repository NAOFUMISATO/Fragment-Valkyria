
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

EffectPlayerShot::EffectPlayerShot(Game::GameMain& gameMain,std::string_view key) :EffectBase{gameMain,key} {
   SetEffectLoadHandle(key);
}

void EffectPlayerShot::Init() {
  _position=_gameMain.objServer().GetVecData("PlayerPos");
  _rotation = _gameMain.objServer().GetVecData("PlayerFor");
  PlayEffect();
}

void EffectPlayerShot::Update() {
   if (!IsPlaying()) {
      SetDead();
   }
}
