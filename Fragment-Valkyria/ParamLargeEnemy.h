#pragma once
/*****************************************************************//**
 * \file   ParamLargeEnemy.h
 * \brief  �{�X�̒l�Ǘ��N���X
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
       * \brief �{�X�̒l�̓ǂݍ��݁A�擾���s��
       * \class �{�X�̒l�Ǘ��N���X
       */
      const class ParamLargeEnemy :public AppFrame::Param::ParamBase {
      public:
         /**
          * \brief �{�X�̒l�̓ǂݍ��݁A�擾���s��
          * \class �{�X�̒l�Ǘ��N���X
          */
         ParamLargeEnemy(std::string_view key);
      };
   }
}