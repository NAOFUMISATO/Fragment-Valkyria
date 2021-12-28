#pragma once
/*****************************************************************//**
 * \file   GameMain.h
 * \brief  �Q�[���{�̃N���X
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "AppFrame.h"
 /**
  * \brief �v���W�F�N�g��
  */
namespace FragmentValkyria {
   namespace {
      constexpr auto ScreenWidth = 1920;   //!< ����ʃT�C�Y
      constexpr auto ScreenHeight = 1080;  //!< �c��ʃT�C�Y
      constexpr auto ScreenDepth = 32;     //!< �r�b�g��
   }
   // ��d�C���N���[�h�h�~
   namespace Object {
      class ObjectServer;
   }
   namespace Create {
      class ObjectFactory;
   }
   /**
    * \brief �Q�[���{��
    */
   namespace Game {
      /**
       * \class �Q�[���{�̃N���X
       * \brief �Q�[���̃��C����������
       */
      class GameMain : public AppFrame::Game::GameBase {
         using GameBase = AppFrame::Game::GameBase;
      public:
         /**
          * \brief ����������
          * \param hInstance WinMain�̑�����
          * \return �������ɐ���������true�A���s������false��Ԃ�
          */
         bool Initialize(HINSTANCE hInstance)override;
         /**
          * \brief ���s
          */
         void Run()override;
         /**
          * \brief ��~
          */
         void ShutDown()override;
         /**
          * \brief ���͏���
          */
         void Input()override;
         /**
          * \brief �X�V����
          */
         void Update()override;
         /**
          * \brief �`�揈��
          */
         void Render()override;

         /**
          * \brief �I�u�W�F�N�g�̈ꊇ�Ǘ��N���X�̎擾
          * \return �I�u�W�F�N�g�̈ꊇ�Ǘ��N���X�̎Q��
          */
         Object::ObjectServer& objServer() const { return *_objServer; }
         /**
          * \brief �I�u�W�F�N�g�̐����ꊇ�Ǘ��N���X�̎擾
          * \return �I�u�W�F�N�g�̐����ꊇ�Ǘ��N���X�̎Q��
          */
         Create::ObjectFactory& objFactory() const { return *_objFactory; }

      private:
         std::unique_ptr<Object::ObjectServer> _objServer;    //!< �I�u�W�F�N�g�ꊇ�Ǘ��N���X�̂̃��j�[�N�|�C���^
         std::unique_ptr<Create::ObjectFactory> _objFactory;  //!< �I�u�W�F�N�g�����ꊇ�Ǘ��N���X�̃��j�[�N�|�C���^
      };
   }
}
