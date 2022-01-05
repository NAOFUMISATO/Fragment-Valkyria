
/*****************************************************************//**
 * \file   ModeBase.cpp
 * \brief  各モードの基底
 *
 * \author NAOFUMISATO
 * \date   October 2021
 *********************************************************************/
#include "ModeBase.h"
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
      ModeBase::ModeBase(Game::GameBase& gameBase) :_gameBase{ gameBase } {
      };

      ModeBase::~ModeBase() {
      }
   }
}