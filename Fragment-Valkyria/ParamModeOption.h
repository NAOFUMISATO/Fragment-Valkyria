#pragma once
/*****************************************************************//**
 * \file   ParamModeOption.h
 * \brief   �I�v�V������ʂ̒l�Ǘ��N���X
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
       * \class �I�v�V������ʂ̒l�Ǘ��N���X
       * \brief �I�v�V������ʂ̒l�̓ǂݍ��݁A�擾���s��
       */
      const class ParamModeOption :public AppFrame::Param::ParamBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̃N���X�̎Q��
          * \param key json�t�@�C�����ƈ�v���镶����
          */
         ParamModeOption(Game::GameMain& gameMain, std::string_view key);
      };
   }
}
