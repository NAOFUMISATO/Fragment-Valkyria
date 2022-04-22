#pragma once
/*****************************************************************//**
 * \file   ParamPoorEnemy.h
 * \brief  �G���G�̒l�Ǘ��N���X
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
       * \class �G���G�̒l�Ǘ��N���X
       * \brief �G���G�̒l�̓ǂݍ��݁A�擾���s��
       */
      const class ParamPoorEnemy :public AppFrame::Param::ParamBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param key json�t�@�C�����ƈ�v���镶����
          */
         ParamPoorEnemy(std::string_view key);
      };
   }
}
