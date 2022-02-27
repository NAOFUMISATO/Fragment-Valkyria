#pragma once
/*****************************************************************//**
 * \file   ModeMovie.h
 * \brief  ���[�h���[�r�[�N���X
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
       * \class ���[�h���[�r�[�N���X
       * \brief ���[�r�[�Đ����s��
       */
      class ModeMovie : public ModeBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̃N���X�̎Q��
          */
         ModeMovie(Game::GameMain& gameMain);
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
         void Update()override;
         /**
          * \brief �`�揈��
          */
         void Render()override;
      
      private:
         bool _play{ true };  //!< ���[�r�[�̃I�[�v���t���O
         int _mvHandle{ -1 }; //!< ���[�r�[�n���h��
      };
   }
}