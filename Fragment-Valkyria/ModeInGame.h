#pragma once
/*****************************************************************//**
 * \file   ModeInGame.h
 * \brief  ���[�h�C���Q�[���N���X
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "AppFrame.h"
#include "GameMain.h"
 /**
  * \brief �v���W�F�N�g��
  */
namespace FragmentValkyria {
   // ��d�C���N���[�h�h�~
   namespace Object {
      class ObjectServer;
   }
   namespace Create {
      class ObjectFactory;
   }
   /**
    * \brief ���[�h�֌W
    */
   namespace Mode {
      /**
       * \class ���[�h�C���Q�[���N���X
       * \brief �C���Q�[���̏�������
       */
      class ModeInGame : public AppFrame::Mode::ModeBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param game �Q�[���{�̂̎Q��
          */
         ModeInGame(Game::GameMain& game);
         /**
          * \brief �f�t�H���g�f�X�g���N�^
          */
         ~ModeInGame() = default;
         /**
          * \brief ����������
          */
         void Init() override;
         /**
          * \brief ��������
          */
         void Enter() override;
         /**
          * \brief ���͏���
          * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
          */
         void Input(AppFrame::Input::InputManager& input) override;
         /**
          * \brief �X�V����
          */
         void Update() override;
         /**
          * \brief �`�揈��
          */
         void Render() override;
         /**
          * \brief �o������
          */
         void Exit() override;

         /**
          * \brief �I�u�W�F�N�g�ꊇ�Ǘ��N���X�̎Q�Ƃ��Q�[�����C���o�R�Ŏ擾
          * \return �I�u�W�F�N�g�ꊇ�Ǘ��N���X�̎Q��
          */
         Object::ObjectServer& objServer() const { return _gameMain.objServer(); }
         /**
          * \brief �I�u�W�F�N�g�����ꊇ�Ǘ��N���X�̎Q�Ƃ��Q�[�����C���o�R�Ŏ擾
          * \return �I�u�W�F�N�g�����ꊇ�Ǘ��N���X�̎Q��
          */
         Create::ObjectFactory& objFactory() const { return _gameMain.objFactory(); }

      private:
         Game::GameMain& _gameMain;  //!< �Q�[���{�̃N���X�̎Q��

#ifdef _DEBUG
         short _padLeftX{ 0 };
         short _padLeftY{ 0 };
#endif
      };
   }
}