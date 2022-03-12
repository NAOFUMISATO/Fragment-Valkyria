#pragma once
#include "ModeBase.h"

namespace FragmentValkyria {
   // ��d�C���N���[�h�h�~
   namespace Game {
      class GameMain;
   }
   namespace Mode {
      class ModeTutorialSelect :public ModeBase {
         using InputManager = AppFrame::Input::InputManager;
      public:
         ModeTutorialSelect(Game::GameMain& gameMain);
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
         int _cusorX{ 0 };
         int _cusorY{ 0 };
         std::unique_ptr<AppFrame::State::StateServer> _stateServer;  //!< ��Ԉꊇ�Ǘ��N���X�̃|�C���^
         std::unordered_map<std::string, int> _handleMap;             //!< �摜�n���h����ۑ�����A�z�z��

      public:
         /**
          * \class ���[�h�`���[�g���A���̏�Ԋ��N���X
          * \brief �e��Ԃ͂��̃N���X��h�����Ē�`����
          */
         class StateBase : public AppFrame::State::StateBaseRoot {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner ���[�h�`���[�g���A���̎Q��
             */
            StateBase(ModeTutorialSelect& owner) : _owner{ owner } {};
            /**
             * \brief �`�揈��
             */
            void Draw() override;

         protected:
            ModeTutorialSelect& _owner;   //!< ���[�h�`���[�g���A���̎Q��
         };

         class StateYes : public StateBase {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner ���[�h�`���[�g���A���̎Q��
             */
            StateYes(ModeTutorialSelect& owner) :StateBase{ owner } {};
            /**
             * \brief ��������
             */
            void Enter() override;
            /**
             * \brief ���͏���
             * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
             */
            void Input(InputManager& input) override;
         };

         class StateNo : public StateBase {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner ���[�h�`���[�g���A���̎Q��
             */
            StateNo(ModeTutorialSelect& owner) : StateBase{ owner } {};
            /**
             * \brief ��������
             */
            void Enter() override;
            /**
             * \brief ���͏���
             * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
             */
            void Input(InputManager& input) override;
         };
      };
   }
}
