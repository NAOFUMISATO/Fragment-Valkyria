#pragma once
/*****************************************************************//**
 * \file   ParamPlayerUI.h
 * \brief  �v���C���[UI�̒l�Ǘ��N���X
 * 
 * \author NAOFUMISATO
 * \date   April 2022
 *********************************************************************/
#include "AppFrame.h"
/**
 * \brief �v���W�F�N�g��
 */
namespace FragmentValkyria {
   /**
    * \brief �l�Ǘ��֌W
    */
   namespace Param {
      /**
       * \class �v���C���[UI�̒l�Ǘ��N���X
       * \brief �v���C���[UI�̒l�̓ǂݍ��݁A�擾���s��
       */
      const class ParamPlayerUI :public AppFrame::Param::ParamBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param key json�t�@�C�����ƈ�v���镶����
          */
         ParamPlayerUI(std::string_view key);
      };
   }
}
