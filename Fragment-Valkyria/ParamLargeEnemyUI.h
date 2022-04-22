#pragma once
/*****************************************************************//**
 * \file   ParamLargeEnemyUI.h
 * \brief  �{�XUI�̒l�Ǘ��N���X
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
       * \brief �{�XUI�̒l�̓ǂݍ��݁A�擾���s��
       * \class �{�XUI�̒l�Ǘ��N���X
       */
      const class ParamLargeEnemyUI :public AppFrame::Param::ParamBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param key json�t�@�C�����ƈ�v���镶����
          */
         ParamLargeEnemyUI(std::string_view key);
      };
   }
}
