#pragma once
/*****************************************************************//**
 * \file   ParamCameraComponent.h
 * \brief  �J�����̒l�Ǘ��N���X
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
       * \class �J�����̒l�Ǘ��N���X
       * \brief �J�����̒l�̓ǂݍ��݁A�擾���s��
       */
      const class ParamCameraComponent :public AppFrame::Param::ParamBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param key json�t�@�C�����ƈ�v���镶����
          */
         ParamCameraComponent( std::string_view key);
      };
   }
}
