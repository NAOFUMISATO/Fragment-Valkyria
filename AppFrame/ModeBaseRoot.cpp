
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

      ModeBaseRoot::~ModeBaseRoot() {
      }

      Resource::LoadResourceJson& ModeBaseRoot::GetLoadJson() const{ 
         return _gameBase.loadJson();
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

      Sound::SoundServer& ModeBaseRoot::GetSoundServer() const {
         return _gameBase.soundServer(); 
      }

      Effect::EffectServer& ModeBaseRoot::GetEfcServer() const {
         return _gameBase.efcServer();
      }
   }
}