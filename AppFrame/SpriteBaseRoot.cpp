
/*****************************************************************//**
 * \file   SpriteBaseRoot.cpp
 * \brief  �X�v���C�g�̊��̊��
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
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

      
   }
}