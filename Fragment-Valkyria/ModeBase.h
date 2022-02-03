#pragma once
/*****************************************************************//**
 * \file   ModeBase.h
 * \brief  �e���[�h�̊��N���X
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "AppFrame.h"
 /**
  * \brief �v���W�F�N�g��
  */
namespace FragmentValkyria {
   // ��d�C���N���[�h�h�~
   namespace Game {
      class GameMain;
   }
   /**
    * \brief ���[�h�֌W
    */
   namespace Mode {
      class ModeBase : public AppFrame::Mode::ModeBaseRoot {
      public:
         ModeBase(Game::GameMain& gameMain);

      protected:
         Game::GameMain& _gameMain; //!< �Q�[���N���X�̎Q��
      };
   }
}
