#pragma once
/*****************************************************************//**
 * \file   ParamModeTitle.h
 * \brief  �^�C�g����ʂ̒l�Ǘ��N���X
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
       * \class �^�C�g����ʂ̒l�Ǘ��N���X
       * \brief �^�C�g����ʂ̒l�̓ǂݍ��݁A�擾���s��
       */
      const class ParamModeTitle :public AppFrame::Param::ParamBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param key json�t�@�C�����ƈ�v���镶����
          */
         ParamModeTitle(std::string_view key);
      };
   }
}
