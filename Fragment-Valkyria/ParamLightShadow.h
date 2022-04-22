#pragma once
/*****************************************************************//**
 * \file   ParamLightShadow.h
 * \brief  ���C�e�B���O�y�ї����e�̏������s���N���X
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
       * \brief ���C�e�B���O�y�ї����e�̏������s���N���X�̒l�̓ǂݍ��݁A�擾���s��
       * \class ���C�e�B���O�y�ї����e�̏������s���N���X�̒l�Ǘ��N���X
       */
      const class ParamLightShadow :public AppFrame::Param::ParamBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param key json�t�@�C�����ƈ�v���镶����
          */
         ParamLightShadow(std::string_view key);
      };
   }
}
