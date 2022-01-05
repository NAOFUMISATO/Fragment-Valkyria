
/*****************************************************************//**
 * \file   ModeBase.cpp
 * \brief  �e���[�h�̊��
 *
 * \author NAOFUMISATO
 * \date   October 2021
 *********************************************************************/
#include "ModeBase.h"
#include "ModeServer.h"
#include "GameBase.h"
 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   /**
    * \brief ���[�h�֌W
    */
   namespace Mode {
      ModeBase::ModeBase(Game::GameBase& gameBase) :_gameBase{ gameBase } {
      };

      ModeBase::~ModeBase() {
      }
   }
}