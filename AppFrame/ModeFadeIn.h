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
          * \param appBase �A�v���P�[�V�����t���[�����N���X�̎Q��
          */
         ModeFadeIn(Game::GameBase& gameBase);
         /**
          * \brief �f�t�H���g�f�X�g���N�^
          */
         ~ModeFadeIn() = default;
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
