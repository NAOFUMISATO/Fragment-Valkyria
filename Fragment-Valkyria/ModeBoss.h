#pragma once
/*****************************************************************//**
 * \file   ModeBoss.h
 * \brief  ���[�h�{�X�N���X
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "ModeInGameBase.h"
 /**
  * \brief �v���W�F�N�g��
  */
namespace FragmentValkyria {
   /**
    * \brief ���[�h�֌W
    */
   namespace Mode {
      /**
       * \class ���[�h�{�X�N���X
       * \brief �C���Q�[���̏�������
       */
      class ModeBoss : public ModeInGameBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param game �Q�[���{�̂̎Q��
          */
         ModeBoss(Game::GameMain& gameMain);
         /**
          * \brief ����������
          */
         void Init()override {};
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

      private:
         /**
          * \brief ���C�e�B���O�ɂ�鉉�o���s��
          */
         void LightStaging();
         bool _cntInit{ false }; //!< �t���[���J�E���g�������t���O
         bool _plus{ true };     //!< ���C�e�B���O�̐ԗv�f���㏸�����邩�̃t���O
         int _lightOnCount{0};   //!< ���x�_�ł����邩
         int _lightCnt{ 0 };     //!< �t���[���J�E���g�ۑ��p
         float _red{0.1f};       //!< ��
         float green{ 0 };       //!< ��
         float blue{ 0 };        //!< ��
      };
   }
}