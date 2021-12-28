#pragma once
/*****************************************************************//**
 * \file   ModeTitle.h
 * \brief  ���[�h�^�C�g���N���X
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
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
      /**
       * \class ���[�h�^�C�g���N���X
       * \brief �Q�[���̃^�C�g����������
       */
      class ModeTitle : public AppFrame::Mode::ModeBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̂̎Q��
          */
         ModeTitle(Game::GameMain& gameMain);
         /**
          * \brief �f�X�g���N�^
          */
         ~ModeTitle() = default;
         /**
          * \brief ����������
          */
         void Init()override;
         /**
          * \brief ��������
          */
         virtual void Enter()override;
         /**
          * \brief ���͏���
          * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
          */
         void Input(AppFrame::Input::InputManager& input)override;
         /**
          * \brief �X�V����
          */
         void Update() override;
         /**
          * \brief �`�揈��
          */
         void Render() override;

      private:
         int _titleGrHandle{ -1 };  //!< �Q�[���^�C�g���摜�n���h��
         int _guideGrHandle{ -1 };  //!< �X�^�[�g�K�C�h�摜�n���h��
         int _bgGrHandle{ -1 };     //!< �^�C�g���w�i�摜�n���h��
         int _alpha{ 0 };           //!< �X�^�[�g�K�C�h�̓����x
      };
   }
}
