#pragma once
/*****************************************************************//**
 * \file   ModeMissionFailed.h
 * \brief  ���[�h�~�b�V�������s�N���X
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
       * \class ���[�h�~�b�V�������s�N���X
       * \brief �v���C���[���S���ɕ\������
       */
      class ModeMissionFailed :public AppFrame::Mode::ModeBase {
      public:
         /**
          * \brief �R���X�g���N�^
          */
         ModeMissionFailed();
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
         int _animeNo{ 0 };                   //!< �A�j���[�V�����ԍ�
         int _animeCnt{ 0 };                  //!< �t���[���J�E���g�ۑ��p
         bool _cntInit{ true };               //!< �t���[���J�E���g�������t���O
         std::vector<int> _grHandles{ -1 };   //!< �摜�n���h��
      };
   }
}
