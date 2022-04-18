#pragma once
/*****************************************************************//**
 * \file   ParamModeGameOver.h
 * \brief   �Q�[���I�[�o�[��ʂ̒l�Ǘ��N���X
 * 
 * \author NAOFUMISATO
 * \date   April 2022
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
    * \brief �l�Ǘ��֌W
    */
   namespace Param {
      /**
       * \brief �Q�[���I�[�o�[��ʂ̒l�̓ǂݍ��݁A�擾���s��
       * \class �Q�[���I�[�o�[��ʂ̒l�Ǘ��N���X
       */
      const class ParamModeGameOver :public AppFrame::Param::ParamBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̃N���X�̎Q��
          * \param key json�t�@�C�����ƈ�v���镶����
          */
         ParamModeGameOver(Game::GameMain& gameMain, std::string_view key);
      };
   }
}
