
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

      SpriteServer& SpriteBaseRoot::GetSprServer() const {
         return _gameBase.objServer();
      }

      Effect::EffectServer& SpriteBaseRoot::GetEfcServer() const {
         return _gameBase.efcServer();
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