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
          * \brief �f�t�H���g�f�X�g���N�^
          */
         ~ModeBoss() = default;
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
          * \brief �C���Q�[����ʂ̎擾
          * \return ���[�h�{�X
          */
         virtual InGameType GetInGameType()const { return InGameType::Boss; }

      private:
         int _gameCnt{0};
      };
   }
}