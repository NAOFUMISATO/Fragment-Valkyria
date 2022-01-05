#pragma once
/*****************************************************************//**
 * \file   ModeFadeOut.h
 * \brief  ���[�h�̃t�F�[�h�A�E�g�N���X
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
       * \class ���[�h�̃t�F�[�h�A�E�g�N���X
       * \brief ���[�h�̑J�ڎ��ɌĂяo���A�^���̃V�[�������O����
       */
      class ModeFadeOut : public ModeFadeBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param appBase �A�v���P�[�V�����t���[���̊��N���X�̎Q��
          */
         ModeFadeOut(Game::GameBase& gameBase);
         /**
          * \brief �f�t�H���g�f�X�g���N�^
          */
         ~ModeFadeOut() = default;
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