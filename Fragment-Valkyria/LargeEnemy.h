#pragma once
/*****************************************************************//**
 * \file   LargeEnemy.h
 * \brief  ���[�W�G�l�~�[�̏������񂷃N���X
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "ObjectBase.h"
namespace FragmentValkyria {

    namespace Enemy {

        class LargeEnemy : public Object::ObjectBase {
            using Vector4 = AppFrame::Math::Vector4;
            using Matrix44 = AppFrame::Math::Matrix44;
            using InputManager = AppFrame::Input::InputManager;
        public:
            LargeEnemy(Game::GameMain& gameMain);
            virtual ~LargeEnemy() override = default;

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


            void CreateGatling();

            void CreateLaser();

            double hp() { return _hp; }

        private:

            void CreateFallObject();

            void HitCheckFromFallObject();

            void HitCheckFromBullet();

            void Move();
            void Rotate(bool& rotating);
            void SetAddRotate();

            int _stateCnt{ 0 };
            int _gatlingCnt{ 0 };
            int _collision{ 0 };
            const int GatlingFrame{ 60 };
            bool _fallObjectflag{ false };
            bool _moving{ false };
            bool _firstRotating{ true };
            bool _endRotating{ true };
            double _rotateDot{ 0.0 };
            double _addRotate{ 1.0 };
            double _hp{ 100.0 };
            int _freezeTime{ 0 };

            Vector4 _moved{ 0.0, 0.0, 0.0 };


        public:
            /**
            * \class ���[�W�G�l�~�[�̏�Ԃ̊��N���X
            * \brief �e���[�W�G�l�~�[�̏�Ԃ͂����h�����Ē�`����
            */
            class StateBase : public AppFrame::State::StateBaseRoot
            {
            public:
                /**
                 * \brief �R���X�g���N�^
                 * \param owner ���[�W�G�l�~�[�̎Q��
                 */
                StateBase(LargeEnemy& owner) : _owner{ owner } {};
                /**
                 * \brief �`�揈��
                 */
                virtual void Draw() override;

            protected:
                LargeEnemy& _owner;   //!< ���[�W�G�l�~�[�̎Q��
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
                 * \param owner ���[�W�G�l�~�[�̎Q��
                 */
                StateIdle(LargeEnemy& owner) : StateBase{ owner } {};
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
            * \class �I�u�W�F�N�g������ԃN���X
            * \brief �I�u�W�F�N�g�����̏�������
            */
            class StateFallObject : public StateBase
            {
            public:
                /**
                 * \brief �R���X�g���N�^
                 * \param owner ���[�W�G�l�~�[�̎Q��
                 */
                StateFallObject(LargeEnemy& owner) : StateBase{ owner } {};
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
            * \class �K�g�����O�U����ԃN���X
            * \brief �K�g�����O�U����Ԃ̏�������
            */
            class StateGatling : public StateBase
            {
            public:
                /**
                 * \brief �R���X�g���N�^
                 * \param owner ���[�W�G�l�~�[�̎Q��
                 */
                StateGatling(LargeEnemy& owner) : StateBase{ owner } {};
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
            * \class �K�g�����O�U����ԃN���X
            * \brief �K�g�����O�U����Ԃ̏�������
            */
            class StateDie : public StateBase
            {
            public:
                /**
                 * \brief �R���X�g���N�^
                 * \param owner ���[�W�G�l�~�[�̎Q��
                 */
                StateDie(LargeEnemy& owner) : StateBase{ owner } {};
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
            class StateMove : public StateBase
            {
            public:
                /**
                 * \brief �R���X�g���N�^
                 * \param owner ���[�W�G�l�~�[�̎Q��
                 */
                StateMove(LargeEnemy& owner) : StateBase{ owner } {};
                /**
                 * \brief ��������
                 */
                void Enter() override;
                /**
                 * \brief �X�V����
                 */
                void Update() override;

            private:
                bool _endGetplyPos{ true };
            };
            /**
            * \class �K�g�����O�U����ԃN���X
            * \brief �K�g�����O�U����Ԃ̏�������
            */
            class StateLaser : public StateBase
            {
            public:
                /**
                 * \brief �R���X�g���N�^
                 * \param owner ���[�W�G�l�~�[�̎Q��
                 */
                StateLaser(LargeEnemy& owner) : StateBase{ owner } {};
                /**
                 * \brief ��������
                 */
                void Enter() override;
                /**
                 * \brief �X�V����
                 */
                void Update() override;
            };
        };
    }
}
