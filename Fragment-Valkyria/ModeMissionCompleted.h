#pragma once
/*****************************************************************//**
 * \file   ModeMissionCompleted.h
 * \brief  ���[�h�~�b�V���������N���X
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "AppFrame.h"
/**
 * \brief �v���W�F�N�g��
 */
namespace FragmentValkyria {
   /**
    * \brief ���[�h�֌W
    */
   namespace Mode {
      /**
       * \class ���[�h�~�b�V���������N���X
       * \brief �Q�[���N���A���ɕ\������
       */
      class ModeMissionCompleted :public AppFrame::Mode::ModeBase {
      public:
         /**
          * \brief �R���X�g���N�^
          */
         ModeMissionCompleted();
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
          * \brief �`�揈��
          */
         void Render() override;

      private:
         std::vector<int> _grHandles{ -1 };   //!< �摜�n���h��
      };
   }
}
