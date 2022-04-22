#pragma once
/*****************************************************************//**
 * \file   ParamGatling.h
 * \brief  �K�g�����O�̒l�Ǘ��N���X
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
       * \class �K�g�����O�̒l�Ǘ��N���X
       * \brief �K�g�����O�̒l�̓ǂݍ��݁A�擾���s��
       */
      const class ParamGatling :public AppFrame::Param::ParamBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param key json�t�@�C�����ƈ�v���镶����
          */
         ParamGatling(std::string_view key);
      };
   }
}
