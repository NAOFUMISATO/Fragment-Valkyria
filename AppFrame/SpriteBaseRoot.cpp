
#include "SpriteBaseRoot.h"
#include "GameBase.h"
#include "StateServer.h"
#include "DxUtility.h"
#include "ModeServer.h"
#include "TextureComponent.h"
#include "ResourceServer.h"

namespace AppFrame {
   namespace Sprite {

      SpriteBaseRoot::SpriteBaseRoot(Game::GameBase& gameBase) :_gameBase{ gameBase } {
      }

      void SpriteBaseRoot::Draw() {
         auto [x, y] = _position.GetVec2();
         SetDrawBlendMode(DX_BLENDMODE_ALPHA, _alpha);
         GetTexComponent().TransDrawTexture(static_cast<int>(x), static_cast<int>(y), _cx, _cy, _scale, _angle, _grHandle, _turnFlag);
         SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
      }

      SpriteServer& SpriteBaseRoot::GetSprServer() const {
         return _gameBase.sprServer();
      }

      Texture::TextureComponent& SpriteBaseRoot::GetTexComponent() const {
         return _gameBase.texComponent();
      }

      AppFrame::Resource::LoadResourceJson& SpriteBaseRoot::GetLoadJson() const {
         return _gameBase.loadresJson();
      }

      AppFrame::Resource::ResourceServer& SpriteBaseRoot::GetResServer() const {
         return _gameBase.resServer();
      }

      void SpriteBaseRoot::stateServer(std::unique_ptr<StateServer> state) {
         _stateServer = std::move(state);
      }
   }
}