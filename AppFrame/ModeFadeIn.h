#pragma once
/*****************************************************************//**
 * \file   ModeFadeIn.h
 * \brief  ���[�h�̃t�F�[�h�C���N���X
 *
 * \author NAOFUMISATO
 * \date   November 2021
 *********************************************************************/
#include "ModeFadeBase.h"
 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   /**
    * \brief ���[�h�֌W
    */
   namespace Mode {
      /**
       * \class ���[�h�̃t�F�[�h�C���N���X
       * \brief ���[�h�J�ڎ��ɌĂяo��
       */
      class ModeFadeIn : public ModeFadeBase {
      public:
         /**
          * \brief �R���X�g���N�^
          */
         ModeFadeIn();
         /**
          * \brief ����������
          */
         void Init()override {};
         /**
          * \brief ��������
          */
         void Enter() override;
         /**
          * \brief �X�V����
          */
         void Update() override;
      };
   }
}
