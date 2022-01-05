
/*****************************************************************//**
 * \file   ModeBase.cpp
 * \brief  各モードの基底
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
   }
}