#pragma once
/*****************************************************************//**
 * \file   ModeInGame.h
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
       * \class ���[�h�C���Q�[���N���X
       * \brief �C���Q�[���̏�������
       */
      class ModeInGame : public ModeInGameBase {
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
          * \brief �C���Q�[����ʂ̎擾
          * \return ���[�h�{�X
          */
         virtual InGameType GetInGameType()const { return InGameType::Boss; }
      };
   }
}