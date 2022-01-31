#pragma once
/*****************************************************************//**
 * \file   PoorEnemyGatling.h
 * \brief  �K�g�����O�U�������Ă���G���G�̏������񂷃N���X
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "LargeEnemy.h"
namespace FragmentValkyria {
	namespace Enemy {

		class PoorEnemyGatling : public Enemy::LargeEnemy {
            using InputManager = AppFrame::Input::InputManager;
        public:
			PoorEnemyGatling(Game::GameMain& gameMain);
			virtual ~PoorEnemyGatling() override = default;

            virtual ObjectType GetObjType() const override { return ObjectType::LargeEnemy; };
            /**
            * \brief ����������
            */
            virtual void Init() override;
            /**
             * \brief ���͏���
             * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
             */
            virtual void Input(InputManager& input) override;
            /**
             * \brief �X�V����
             */
            virtual void Update() override;
            /**
             * \brief �`�揈��
             */
            void Draw() override;

        private:

        public:
            /**
            * \class �K�g�����O�U�������Ă���G���G�̏�Ԃ̊��N���X
            * \brief �e�K�g�����O�U�������Ă���G���G�̏�Ԃ͂����h�����Ē�`����
            */
            class StateBase : public AppFrame::State::StateBaseRoot
            {
            public:
                /**
                 * \brief �R���X�g���N�^
                 * \param owner �K�g�����O�U�������Ă���G���G�̎Q��
                 */
                StateBase(PoorEnemyGatling& owner) : _owner{ owner } {};
                /**
                 * \brief �`�揈��
                 */
                virtual void Draw() override;

            protected:
                PoorEnemyGatling& _owner;   //!< �K�g�����O�U�������Ă���G���G�̎Q��
            };
            /**
            * \class �ҋ@��ԃN���X
            * \brief �ҋ@��Ԃ̏�������
            */
            class StateIdle : public StateBase
            {
            public:
                /**
                 * \brief �R���X�g���N�^
                 * \param owner �K�g�����O�U�������Ă���G���G�̎Q��
                 */
                StateIdle(PoorEnemyGatling& owner) : StateBase{ owner } {};
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
                 * \brief �X�V����
                 */
                void Update() override;
            };
            /**
            * \class �ړ���ԃN���X
            * \brief �ړ���Ԃ̏�������
            */
            class StateMove : public StateBase
            {
            public:
                /**
                 * \brief �R���X�g���N�^
                 * \param owner �K�g�����O�U�������Ă���G���G�̎Q��
                 */
                StateMove(PoorEnemyGatling& owner) : StateBase{ owner } {};
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
                 * \brief �X�V����
                 */
                void Update() override;
            };
		};
	}

}
