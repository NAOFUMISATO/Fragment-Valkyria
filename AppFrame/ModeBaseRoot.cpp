
/*****************************************************************//**
 * \file   ModeBaseRoot.cpp
 * \brief  ���[�h�̊��̊��
 *
 * \author NAOFUMISATO
 * \date   October 2021
 *********************************************************************/
#include "ModeBaseRoot.h"
#include "ModeServer.h"
#include "GameBase.h"
#include "TextureComponent.h"
#include "LoadResourceJson.h"
 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   /**
    * \brief ���[�h�֌W
    */
   namespace Mode {
      ModeBaseRoot::ModeBaseRoot() {
      };

      Resource::LoadResourceJson& ModeBaseRoot::GetLoadJson() const{ 
         auto& gameInstance = AppFrame::Game::GameBase::GetInstance();
         return gameInstance.loadresJson();
      }

      Mode::ModeServer& ModeBaseRoot::GetModeServer() const {
         auto& gameInstance = AppFrame::Game::GameBase::GetInstance();
         return gameInstance.modeServer();
      }

      Resource::ResourceServer& ModeBaseRoot::GetResServer() const {
         auto& gameInstance = AppFrame::Game::GameBase::GetInstance();
         return gameInstance.resServer();
      }

      Sound::SoundComponent& ModeBaseRoot::GetSoundComponent() const {
         auto& gameInstance = AppFrame::Game::GameBase::GetInstance();
         return gameInstance.soundComponent();
      }

      Texture::TextureComponent& ModeBaseRoot::GetTexComponent() const {
         auto& gameInstance = AppFrame::Game::GameBase::GetInstance();
         return gameInstance.texComponent();
      }
   }
}