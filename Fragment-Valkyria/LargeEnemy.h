#pragma once
/*****************************************************************//**
 * \file   LargeEnemy.h
 * \brief  
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "ObjectBase.h"
namespace FragmentValkyria {

    namespace LargeEnemy {

        class LargeEnemy : public Object::ObjectBase {
            using Vector4 = AppFrame::Math::Vector4;
            using Matrix44 = AppFrame::Math::Matrix44;
            using InputManager = AppFrame::Input::InputManager;
        public:
            LargeEnemy(Game::GameMain& gameMain);
            virtual ~LargeEnemy() override = default;

            ObjectType GetObjType() const override { return ObjectType::LargeEnemy; };

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

            void CreateFallObject();

            int _stateCnt{ 0 };

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
            * \class �ҋ@��ԃN���X
            * \brief �ҋ@��Ԃ̏�������
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
        };
    }
}