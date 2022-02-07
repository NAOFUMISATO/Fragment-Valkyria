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
         virtual void Enter()override;
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
         std::tuple<int, handles, handles, int, int> _grHandles{ -1,0,0,-1,-1 };
         std::unique_ptr<AppFrame::State::StateServer> _stateServer;
         int _alpha{ 255 };           //!< �X�^�[�g�K�C�h�̓����x

      public:

         class StateBase : public AppFrame::State::StateBaseRoot {
         public:

            StateBase(ModeTitle& owner) : _owner{ owner } {};

            void Draw() override;

         protected:
            ModeTitle& _owner;   //!< �J�����̎Q��
         };

         class StateAnyBotton : public StateBase
         {
         public:

            StateAnyBotton(ModeTitle& owner) :StateBase{ owner } {};

            void Enter() override;

            void Input(InputManager& input) override;

            void Update() override;

            void Draw()override;

         private:
            int _grHandle{ -1 };
         };

         class StateStartSelect : public StateBase
         {
         public:

            StateStartSelect(ModeTitle& owner) : StateBase{ owner } {};

            void Enter() override;

            void Input(InputManager& input) override;

            void Update() override;
         };

         class StateEndSelect : public StateBase
         {
         public:

            StateEndSelect(ModeTitle& owner) : StateBase{ owner } {};

            void Enter() override;

            void Input(InputManager& input) override;

            void Update() override;
         };
      };
   }
}
