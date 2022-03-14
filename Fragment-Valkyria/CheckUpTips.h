#pragma once
#include "SpriteBase.h"

namespace FragmentValkyria {
   namespace Clear {
      class CheckUpTips :public Sprite::SpriteBase {
         using InputManager = AppFrame::Input::InputManager;
      public:
         CheckUpTips(Game::GameMain& gameMain);
         /**
          * \brief ����������
          */
         void Init()override;
         /**
          * \brief ���͏���
          * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
          */
         void Input(InputManager& input)override;
         /**
          * \brief �X�V����
          */
         void Update()override;
         /**
          * \brief �`�揈��
          */
         void Draw()override;
         /**
          * \brief �X�v���C�g��ʂ̎擾
          * \return ���:�N���A�ē��\���e�L�X�g
          */
         virtual SpriteType GetSprType() const { return SpriteType::ClearTips; }

      private:
         int _width{ 0 };
         int _grHandle{ -1 };
         bool _isAppear{ false };
         std::unique_ptr<AppFrame::State::StateServer> _stateServer;  //!< ��Ԉꊇ�Ǘ��N���X�̃|�C���^ 

      public:
         class StateBase : public AppFrame::State::StateBaseRoot {
         public:

            StateBase(CheckUpTips& owner) : _owner{ owner } {};
            /**
             * \brief �`�揈��
             */
            void Draw() override;

         protected:
            CheckUpTips& _owner;   //!< ���[�h�^�C�g���̎Q��
         };

         class StateFadeIn : public StateBase {
         public:
            StateFadeIn(CheckUpTips& owner) :StateBase{ owner } {};
            /**
             * \brief ��������
             */
            void Enter() override;
            /**
             * \brief �X�V����
             */
            void Update()override;
            /**
             * \brief �o������
             */
            void Exit()override;
         };

         class StateJudge : public StateBase {
         public:
            StateJudge(CheckUpTips& owner) : StateBase{ owner } {};
            /**
            * \brief �X�V����
            */
            void Update()override;

         };

         class StateFadeOut : public StateBase {
         public:
            StateFadeOut(CheckUpTips& owner) : StateBase{ owner } {};
            /**
             * \brief ��������
             */
            void Enter() override;
            /**
             * \brief �X�V����
             */
            void Update()override;
         };
      };
   }
}
