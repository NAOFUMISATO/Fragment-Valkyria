
/*****************************************************************//**
 * \file   SpriteBase.cpp
 * \brief  スプライトの基底
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "SpriteBase.h"
#include "Game.h"

using namespace FragmentValkyria::Sprite;

SpriteBase::SpriteBase() {
}

void SpriteBase::Draw() {
   auto [x, y] = _position.GetVec2();
   SetDrawBlendMode(DX_BLENDMODE_ALPHA, _alpha);
   auto& gameInstance = Game::Game::GetInstance();
   gameInstance.texComponent().TransDrawTexture(static_cast<int>(x), static_cast<int>(y),
      _cx, _cy,_scale, _angle, _grHandle, _transFlag,_turnFlag);
   SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}