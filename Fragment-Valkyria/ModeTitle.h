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
         using handles = std::vector<int>;
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
         std::unique_ptr<AppFrame::State::StateServer> _stateServer;       //!< ��Ԉꊇ�Ǘ��N���X�̃|�C���^
         int _alpha{ 0 };                                                  //!< �����x
         bool _pushAnyBotton{ false };                                     //!< �^�C�g����AnyBotton�̉����t���O
         std::pair<int, int> _cusorPos{ 0,0 };                             //!< �I���G�t�F�N�g�̃J�[�\�����W
         std::tuple<int, handles, handles, int, int, int, int> _grHandles{ //!< �摜�n���h���Q
           -1,0,0,-1,-1,-1,-1
         };

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
         class StateAnyBotton : public StateBase{
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner ���[�h�^�C�g���̎Q��
             */
            StateAnyBotton(ModeTitle& owner) :StateBase{ owner } {};
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

         };
      };
   }
}
