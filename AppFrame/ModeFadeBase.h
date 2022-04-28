#pragma once
/*****************************************************************//**
 * \file   ModeFadeBase.h
 * \brief  ���[�h�̃t�F�[�h���N���X
 *
 * \author NAOFUMISATO
 * \date   November 2021
 *********************************************************************/
#include "ModeBase.h"
/**
 * \brief �A�v���P�[�V�����t���[��
 */
namespace AppFrame {
   /**
    * \brief ���[�h�֌W
    */
   namespace Mode {
      /**
       * \class ���[�h�̃t�F�[�h���N���X
       * \brief �t�F�[�h�p���[�h�̊��N���X
       */
      class ModeFadeBase :public ModeBase {
      public:
         /**
          * \brief �R���X�g���N�^
          */
         ModeFadeBase();
         /**
          * \brief ����������
          */
         void Init()override {};
         /**
          * \brief �`�揈��
          */
         void Render() override;

      protected:
         float _alpha{ 0 };            //!< ��ʃt�F�[�h�p�����x
         float _deltaAlpha{ 0 };       //!< ���t���[���̈Ó]���x
      };
   }
}