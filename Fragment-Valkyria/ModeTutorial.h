#pragma once
/*****************************************************************//**
 * \file   ModeTutorial.h
 * \brief  ���[�h�`���[�g���A���N���X
 * 
 * \author NAOFUMISATO
 * \date   March 2022
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
       * \class ���[�h�`���[�g���A���N���X
       * \brief �`���[�g���A�����s��
       */
      class ModeTutorial :public ModeInGameBase {
         using InputManager = AppFrame::Input::InputManager;
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̃N���X�̎Q��
          */
         ModeTutorial(Game::GameMain& gameMain);
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

      private:
         /**
          * \brief ������Ԃ̃`���[�g���A��TIPS�͂��邩
          * \return �����true,�������false
          */
         bool TipsAlive();
         /**
          * \brief �`���[�g���A�����N���A���Ă���Ώ������s��
          * \param key �`���[�g���A���Ɋ֘A�t����������
          */
         void ClearJudge(std::string_view key);
         /**
          * \brief �`���[�g���A��TIPS�̐������s��
          * \param key ��������`���[�g���A���Ɋ֘A�t����������
          */
         void TipsBorn(std::string_view key);
         /**
          * \brief �����I�u�W�F�N�g���Ȃ��Ȃ�ēx�o��������
          */
         void FallObjectRespawn();
         int _tutorialProgress{ 1 }; //!< �`���[�g���A���̐i��
         bool _born{ true };         //!< TIPS�����t���O
         bool _isAppear{ false };    //!< TIPS�o���t���O
      };
   }
}
