
#include "SpriteBaseRoot.h"
#include "GameBase.h"
#include "StateServer.h"
#include "DxUtility.h"
#include "ModeServer.h"
#include "SimpleTextureComponent.h"

namespace AppFrame {
   namespace Sprite {

      SpriteBaseRoot::SpriteBaseRoot(Game::GameBase& gameBase) :_gameBase{ gameBase } {
      }

      SpriteBaseRoot::~SpriteBaseRoot() {
      }

      void SpriteBaseRoot::Draw() {
         auto [x, y] = _position.GetVec2();
         SetDrawBlendMode(DX_BLENDMODE_ALPHA, _alpha);
         GetSimpTexComponent().TransDrawTexture(x, y, _cx, _cy, _scale, _angle, _grHandle, _turnFlag);
         SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
      }

      SpriteServer& SpriteBaseRoot::GetSprServer() const {
         return _gameBase.sprServer();
      }

      Texture::SimpleTextureComponent& SpriteBaseRoot::GetSimpTexComponent() const {
         return _gameBase.simpleTexComponent();
      }

      AppFrame::Resource::LoadResourceJson& SpriteBaseRoot::GetLoadJson() const {
         return _gameBase.loadresJson();
      }

      void SpriteBaseRoot::stateServer(std::unique_ptr<StateServer> state) {
         _stateServer = std::move(state);
      }
   }
}