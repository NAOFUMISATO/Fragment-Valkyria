#pragma once
/*****************************************************************//**
 * \file   ParamModeClear.h
 * \brief  �N���A��ʂ̒l�Ǘ��N���X
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
       * \brief �N���A��ʂ̒l�̓ǂݍ��݁A�擾���s��
       * \class �N���A��ʂ̒l�Ǘ��N���X
       */
      const class ParamModeClear :public AppFrame::Param::ParamBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̃N���X�̎Q��
          * \param key json�t�@�C�����ƈ�v���镶����
          */
         ParamModeClear(Game::GameMain& gameMain, std::string_view key);
      };
   }
}
