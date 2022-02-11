#pragma once
/*****************************************************************//**
 * \file   ModeGameOver.h
 * \brief  ���[�h�Q�[���I�[�o�[�N���X
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "ModeBase.h"
/**
 * \brief �v���W�F�N�g��
 */
namespace FragmentValkyria {
   /**
    * \brief ���[�h�֌W
    */
   namespace Mode {
      /**
       * \class ���[�h�Q�[���I�[�o�[�N���X
       * \brief �Q�[���I�[�o�[��ʂ�\������
       */
      class ModeGameOver:public ModeBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̃N���X�̎Q��
          */
         ModeGameOver(Game::GameMain& gameMain);
         /**
          * \brief ����������
          */
         void Init()override;
         /**
          * \brief ��������
          */
         void Enter()override;
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
         int _grHandle{ -1 };   //!< �摜�n���h��
      };
   }
}
