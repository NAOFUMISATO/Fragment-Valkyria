
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

SpriteBase::SpriteBase(Game::GameMain& gameMain) : _gameMain{ gameMain }{
}

void SpriteBase::Draw() {
   auto [x, y] = _position.GetVec2();
   SetDrawBlendMode(DX_BLENDMODE_ALPHA, _alpha);
   GetTexComponent().TransDrawTexture(static_cast<int>(x), static_cast<int>(y), _cx, _cy, 
      _scale, _angle, _grHandle, _transFlag,_turnFlag);
   SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

SpriteServer& SpriteBase::GetSprServer() const {
   return _gameMain.sprServer();
}

AppFrame::Texture::TextureComponent& SpriteBase::GetTexComponent() const {
   return _gameMain.texComponent();
}

AppFrame::Resource::LoadResourceJson& SpriteBase::GetLoadJson() const {
   return _gameMain.loadresJson();
}

AppFrame::Resource::ResourceServer& SpriteBase::GetResServer() const {
   return _gameMain.resServer();
}

void SpriteBase::stateServer(std::unique_ptr<StateServer> state) {
   _stateServer = std::move(state);
}