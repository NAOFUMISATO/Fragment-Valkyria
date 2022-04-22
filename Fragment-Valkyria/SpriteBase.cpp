
/*****************************************************************//**
 * \file   SpriteBase.cpp
 * \brief  スプライトの基底
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "SpriteBase.h"
#include "GameMain.h"

using namespace FragmentValkyria::Sprite;

SpriteBase::SpriteBase() {
}

void SpriteBase::Draw() {
   auto [x, y] = _position.GetVec2();
   SetDrawBlendMode(DX_BLENDMODE_ALPHA, _alpha);
   GetTexComponent().TransDrawTexture(static_cast<int>(x), static_cast<int>(y), _cx, _cy, 
      _scale, _angle, _grHandle, _transFlag,_turnFlag);
   SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

SpriteServer& SpriteBase::GetSprServer() const {
   auto gameInstance = Game::GameMain::GetInstance();
   return gameInstance->sprServer();
}

AppFrame::Texture::TextureComponent& SpriteBase::GetTexComponent() const {
   auto gameInstance = Game::GameMain::GetInstance();
   return gameInstance->texComponent();
}

AppFrame::Resource::LoadResourceJson& SpriteBase::GetLoadJson() const {
   auto gameInstance = Game::GameMain::GetInstance();
   return gameInstance->loadresJson();
}

AppFrame::Resource::ResourceServer& SpriteBase::GetResServer() const {
   auto gameInstance = Game::GameMain::GetInstance();
   return gameInstance->resServer();
}

void SpriteBase::stateServer(std::unique_ptr<StateServer> state) {
   _stateServer = std::move(state);
}