
/*****************************************************************//**
 * \file   ModeBaseRoot.cpp
 * \brief  モードの基底の基底
 *
 * \author NAOFUMISATO
 * \date   October 2021
 *********************************************************************/
#include "ModeBaseRoot.h"
#include "ModeServer.h"
#include "GameBase.h"
#include "SimpleTextureComponent.h"
#include "SpriteServer.h"
 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   /**
    * \brief モード関係
    */
   namespace Mode {
      ModeBaseRoot::ModeBaseRoot(Game::GameBase& gameBase) :_gameBase{ gameBase } {
      };

      Resource::LoadResourceJson& ModeBaseRoot::GetLoadJson() const{ 
         return _gameBase.loadresJson();
      }

      Mode::ModeServer& ModeBaseRoot::GetModeServer() const {
         return _gameBase.modeServer(); 
      }

      Object::ObjectServer& ModeBaseRoot::GetObjServer() const {
         return _gameBase.objServer(); 
      }

      Resource::ResourceServer& ModeBaseRoot::GetResServer() const {
         return _gameBase.resServer(); 
      }

      Sound::SoundComponent& ModeBaseRoot::GetSoundServer() const {
         return _gameBase.soundComponent(); 
      }

      Effect::EffectServer& ModeBaseRoot::GetEfcServer() const {
         return _gameBase.efcServer();
      }

      Texture::SimpleTextureComponent& ModeBaseRoot::GetSimpTexComponent() const {
         return _gameBase.simpleTexComponent();
      }

      Sprite::SpriteServer& ModeBaseRoot::GetSprServer() const {
         return _gameBase.sprServer();
      }
   }
}