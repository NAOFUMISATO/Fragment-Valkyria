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
#include "ModeBase.h"
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
      class ModeInGame : public Mode::ModeBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param game �Q�[���{�̂̎Q��
          */
         ModeInGame(Game::GameMain& gameMain);
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
          * \brief �I�u�W�F�N�g�����ꊇ�Ǘ��N���X�̎Q�Ƃ��Q�[�����C���o�R�Ŏ擾
          * \return �I�u�W�F�N�g�����ꊇ�Ǘ��N���X�̎Q��
          */
         Create::ObjectFactory& objFactory() const { return _gameMain.objFactory(); }

      private:
#ifdef _DEBUG
         short _padLeftX{ 0 };
         short _padLeftY{ 0 };
         short _padRightX{ 0 };
         short _padRightY{ 0 };

         double _largeEnemyHp{ 0 };
         double _playerHp{ 0 };
#endif
      };
   }
}