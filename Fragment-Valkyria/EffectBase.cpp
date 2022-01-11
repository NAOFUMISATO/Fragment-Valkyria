
/*****************************************************************//**
 * \file   EffectBase.cpp
 * \brief  エフェクトの基底
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "EffectBase.h"
#include "GameMain.h"

using namespace FragmentValkyria::Effect;

EffectBase::EffectBase(Game::GameMain& gameMain) : EffectBaseRoot{ gameMain }, _gameMain{gameMain} {
}

EffectBase::~EffectBase() {
}

void EffectBase::Init(std::string_view key) {
   SetEffectLoadHandle(key);
}

void EffectBase::Draw() {
   SetPosition(_position);
   SetRotation(_rotation);
   SetScale(_scale);
   SetSpeed(_speed);
   SetEfcColor(_color);
}
