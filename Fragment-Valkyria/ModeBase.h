#pragma once
#include "AppFrame.h"
 /**
  * \brief �v���W�F�N�g��
  */
namespace FragmentValkyria {
   // ��d�C���N���[�h�h�~
   namespace Game {
      class GameMain;
   }
   /**
    * \brief ���[�h�֌W
    */
   namespace Mode {
      class ModeBase : public AppFrame::Mode::ModeBaseRoot {
      public:
         
         ModeBase(Game::GameMain& gameMain);
         /**
          * \brief �f�X�g���N�^
          */
         ~ModeBase() override;
         /**
          * \brief ����������
          */
         void Init()override {};
         /**
          * \brief ��������
          */
         void Enter()override {};
         /**
          * \brief ���͏���
          * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
          */
         void Input(AppFrame::Input::InputManager& input)override {};
         /**
          * \brief �X�V����
          */
         void Update() override {};
         /**
          * \brief �`�揈��
          */
         void Render() override {};

      protected:
         Game::GameMain& _gameMain; //!< �Q�[���N���X�̎Q��
      };
   }
}
