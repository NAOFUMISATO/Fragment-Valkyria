#pragma once
/*****************************************************************//**
 * \file   PoorEnemyGatling.h
 * \brief  �K�g�����O�U�������Ă���G���G�̏������񂷃N���X
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "ObjectBase.h"
namespace FragmentValkyria {
	namespace Enemy {

		class PoorEnemyGatling : public Object::ObjectBase {
            using Vector4 = AppFrame::Math::Vector4;
            using InputManager = AppFrame::Input::InputManager;
        public:
			PoorEnemyGatling(Game::GameMain& gameMain);
			virtual ~PoorEnemyGatling() override = default;

            virtual ObjectType GetObjType() const override { return ObjectType::PoorEnemyGatling; };
            /**
            * \brief ����������
            */
            virtual void Init() override;
            /**
             * \brief �X�V����
             */
            virtual void Update() override;
            /**
             * \brief �`�揈��
             */
            void Draw() override;

        private:
            void Fall();
            void Rotate();
            void CreateGatling();

            void HitCheckFromBullet();
            void HitCheckFromFallObject();

            Vector4 _moved{ Vector4(0.0, 0.0, 0.0) };

            int _stateCnt{ 0 };
            double _hp{ 20.0 };

            int _collision{ 0 };

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
                 * \brief �X�V����
                 */
                void Update() override;
            };
            /**
            * \class ������ԃN���X
            * \brief ������Ԃ̏�������
            */
            class StateFall : public StateBase
            {
            public:
                /**
                 * \brief �R���X�g���N�^
                 * \param owner �K�g�����O�U�������Ă���G���G�̎Q��
                 */
                StateFall(PoorEnemyGatling& owner) : StateBase{ owner } {};
                /**
                 * \brief ��������
                 */
                void Enter() override;
                /**
                 * \brief �X�V����
                 */
                void Update() override;
            };
            /**
            * \class �ړ���ԃN���X
            * \brief �ړ���Ԃ̏�������
            */
            class StateGatling : public StateBase
            {
            public:
                /**
                 * \brief �R���X�g���N�^
                 * \param owner �K�g�����O�U�������Ă���G���G�̎Q��
                 */
                StateGatling(PoorEnemyGatling& owner) : StateBase{ owner } {};
                /**
                 * \brief ��������
                 */
                void Enter() override;
                /**
                 * \brief �X�V����
                 */
                void Update() override;
            private:
                int _gatlingCnt{ 5 };
            };
            /**
            * \class ���S��ԃN���X
            * \brief ���S��Ԃ̏�������
            */
            class StateDie : public StateBase
            {
            public:
                /**
                 * \brief �R���X�g���N�^
                 * \param owner �K�g�����O�U�������Ă���G���G�̎Q��
                 */
                StateDie(PoorEnemyGatling& owner) : StateBase{ owner } {};
                /**
                 * \brief ��������
                 */
                void Enter() override;
                /**
                 * \brief �X�V����
                 */
                void Update() override;
            private:
                int _timeOver{ 0 };
            };
		};
	}

}
