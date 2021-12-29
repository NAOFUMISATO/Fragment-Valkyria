#pragma once
/*****************************************************************//**
 * \file   ModeFadeBase.h
 * \brief  ���[�h�̃t�F�[�h���N���X
 *
 * \author NAOFUMISATO
 * \date   November 2021
 *********************************************************************/
#include "ModeBase.h"
namespace {
   constexpr auto BOX_WIDTH = 1920;  //!< DxLib::DrawBox���T�C�Y
   constexpr auto BOX_HEIGHT = 1080; //!< DxLib::DrawBox�c�T�C�Y
}
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
          * \param appBase �A�v���P�[�V�����t���[�����N���X�̎Q��
          */
         ModeFadeBase(Game::GameBase& appBase);
         /**
          * \brief �f�X�g���N�^
          */
         virtual ~ModeFadeBase();
         /**
          * \brief ����������
          */
         virtual void Init() override;
         /**
          * \brief ��������
          */
         virtual void Enter() override;
         /**
          * \brief �X�V����
          */
         virtual void Update() override;
         /**
          * \brief �`�揈��
          */
         virtual void Render() override;

      protected:
         int _width{ BOX_WIDTH };      //!< ��ʈÓ]�pDxLib::DrawBox���T�C�Y
         int _height{ BOX_HEIGHT };    //!< ��ʈÓ]�pDxLib::DrawBox�c�T�C�Y
         unsigned int _color{ 0 };     //!< ��ʈÓ]�pDxLib::GetColor��RGB
         float _alpha{ 0 };            //!< ��ʃt�F�[�h�p�����x
         float _deltaAlpha{ 0 };       //!< ���t���[���̈Ó]���x
      };
   }
}