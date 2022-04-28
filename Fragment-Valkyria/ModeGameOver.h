#pragma once
/*****************************************************************//**
 * \file   ModeGameOver.h
 * \brief  ���[�h�Q�[���I�[�o�[�N���X
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
       * \class ���[�h�Q�[���I�[�o�[�N���X
       * \brief �Q�[���I�[�o�[��ʂ�\������
       */
      class ModeGameOver:public AppFrame::Mode::ModeBase {
         using InputManager = AppFrame::Input::InputManager;
      public:
         /**
          * \brief �R���X�g���N�^
          */
         ModeGameOver();
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
         void Input(InputManager& input)override;
         /**
          * \brief �X�V����
          */
         void Update() override;
         /**
          * \brief �`�揈��
          */
         void Render() override;

      private:
         std::vector<int> _continueDrawHandles;                        //!< �`��p�摜�n���h��
         std::vector<int> _exitDrawHandles;                            //!< �`��p�摜�n���h��
         std::unordered_map<std::string, std::vector<int>> _handleMap; //!< �摜�n���h�����i�[����A�z�z��
         std::unique_ptr<AppFrame::State::StateServer> _stateServer;   //!< ��Ԉꊇ�Ǘ��N���X�̃|�C���^

      public:
         /**
          * \class ��Ԃ̊��N���X
          * \brief �e��Ԃ͂��̃N���X��h�����Ē�`����
          */
         class StateBase : public AppFrame::State::StateBaseRoot {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner ���[�h�^�C�g���̎Q��
             */
            StateBase(ModeGameOver& owner) : _owner{ owner } {};
            /**
             * \brief �`�揈��
             */
            void Draw() override;

         protected:
            ModeGameOver& _owner;
         };
         /**
          * \class �R���e�j���[�I����ԃN���X
          * \brief �R���e�j���[�I�����̏������s��
          */
         class StateContinue : public StateBase {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner �Q�[���I�[�o�[�N���X�̎Q��
             */
            StateContinue(ModeGameOver& owner) :StateBase{ owner } {};
            /**
             * \brief ��������
             */
            void Enter() override;
            /**
             * \brief ���͏���
             * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
             */
            void Input(InputManager& input) override;
            /**
             * \brief �o������
             */
            void Exit()override;
         };
         /**
          * \class �^�C�g���֖߂�I����ԃN���X
          * \brief �^�C�g���֖߂�I�����̏������s��
          */
         class StateExit : public StateBase {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner �Q�[���I�[�o�[�N���X�̎Q��
             */
            StateExit(ModeGameOver& owner) : StateBase{ owner } {};
            /**
             * \brief ��������
             */
            void Enter() override;
            /**
             * \brief ���͏���
             * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
             */
            void Input(InputManager& input) override;
            /**
             * \brief �o������
             */
            void Exit() override;
         };
      };
   }
}
