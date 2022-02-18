
/*****************************************************************//**
 * \file   ObjectBaseRoot.cpp
 * \brief  �I�u�W�F�N�g�̊��̊��
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "ObjectBaseRoot.h"
#include "GameBase.h"
#include "ObjectServer.h"
#include "StateServer.h"
#include "DxUtility.h"
#include "ModeServer.h"
#include "TextureComponent.h"

namespace AppFrame {
   namespace Object {

      ObjectBaseRoot::ObjectBaseRoot(Game::GameBase& gameBase) :_gameBase{ gameBase } {
      }

      ObjectBaseRoot::~ObjectBaseRoot() {
      }

      
   }
}