#pragma once
/*****************************************************************//**
 * \file   ModeGameOver.h
 * \brief  ���[�h�Q�[���I�[�o�[�N���X
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "ModeBase.h"
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
      class ModeGameOver:public ModeBase {
         using InputManager = AppFrame::Input::InputManager;
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̃N���X�̎Q��
          */
         ModeGameOver(Game::GameMain& gameMain);
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
         std::vector<int> _continueDrawHandles;
         std::vector<int> _exitDrawHandles;
         std::unordered_map<std::string, std::vector<int>> _handleMap;
         std::unique_ptr<AppFrame::State::StateServer> _stateServer;


      public:
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

         class StateContinue : public StateBase {
         public:
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

         class StateExit : public StateBase {
         public:
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
