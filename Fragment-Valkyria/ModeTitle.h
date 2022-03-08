#pragma once
/*****************************************************************//**
 * \file   ModeTitle.h
 * \brief  ���[�h�^�C�g���N���X
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "AppFrame.h"
#include "ModeBase.h"
 /**
  * \brief �v���W�F�N�g��
  */
namespace FragmentValkyria {
   // ��d�C���N���[�h�h�~
   namespace Game {
      class GameMain;
   }
   /**
    * \brief ���[�h�֌W
    */
   namespace Mode {
      /**
       * \class ���[�h�^�C�g���N���X
       * \brief �Q�[���̃^�C�g����������
       */
      class ModeTitle : public ModeBase {
         using InputManager = AppFrame::Input::InputManager;
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̂̎Q��
          */
         ModeTitle(Game::GameMain& gameMain);
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
         void LogoAnimation();
         std::unique_ptr<AppFrame::State::StateServer> _stateServer;  //!< ��Ԉꊇ�Ǘ��N���X�̃|�C���^
         int _logoCnt{ 0 };                                           //!< �^�C�g�����S�̃A�j���[�V�����J�E���g
         int _firstInputCnt{ 0 };                                     //!< �v���X�G�j�C�{�^������ڍs������A�A�����͂ɂ��Q�[���J�n��h���J�E���g
         int _logoHandle{ -1 };                                       //!< �^�C�g�����S�̕`��n���h��
         bool _cntInit{ false };                                      //!< �J�E���g�������t���O
         bool _playSound{ false };
         bool _pushAnyButton{ false };                                //!< �^�C�g���̃v���X�G�j�C�{�^���̉����t���O
         std::vector<int> _startDrawHandles{ 0 };                     //!< �Q�[���J�n�I���̕`��p�n���h��
         std::vector<int> _optionDrawHandles{ 0 };                    //!< �I�v�V�����I���̕`��p�n���h��
         std::vector<int> _endDrawHandles{ 0 };                       //!< �Q�[���I���I���̕`��p�n���h��
         std::unordered_map<std::string, std::vector<int>> _handleMap;//!< �摜�n���h����ۑ�����A�z�z��

      public:
         /**
          * \class ���[�h�^�C�g���̏�Ԋ��N���X
          * \brief �e��Ԃ͂��̃N���X��h�����Ē�`����
          */
         class StateBase : public AppFrame::State::StateBaseRoot {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner ���[�h�^�C�g���̎Q��
             */
            StateBase(ModeTitle& owner) : _owner{ owner } {};
            /**
             * \brief �`�揈��
             */
            void Draw() override;
         protected:
            ModeTitle& _owner;   //!< ���[�h�^�C�g���̎Q��
         };

         /**
          * \class �^�C�g��AnyBotton�I����ԃN���X
          * \brief �^�C�g��AnyBotton��\������
          */
         class StateAnyButton : public StateBase{
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner ���[�h�^�C�g���̎Q��
             */
            StateAnyButton(ModeTitle& owner) :StateBase{ owner } {};
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
          * \class �Q�[���X�^�[�g�I����ԃN���X
          * \brief �Q�[�����J�n����
          */
         class StateStartSelect : public StateBase{
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner ���[�h�^�C�g���̎Q��
             */
            StateStartSelect(ModeTitle& owner) : StateBase{ owner } {};
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

         /**
          * \class �Q�[���I�v�V�����I����ԃN���X
          * \brief �Q�[���I�v�V������\������
          */
         class StateOptionSelect : public StateBase {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner ���[�h�^�C�g���̎Q��
             */
            StateOptionSelect(ModeTitle& owner) : StateBase{ owner } {};
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

         /**
          * \class �Q�[���I���I����ԃN���X
          * \brief �Q�[�����I������
          */
         class StateEndSelect : public StateBase{
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner ���[�h�^�C�g���̎Q��
             */
            StateEndSelect(ModeTitle& owner) : StateBase{ owner } {};
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
