#pragma once
/*****************************************************************//**
 * \file   LargeEnemy.h
 * \brief  ���[�W�G�l�~�[�̏������񂷃N���X
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "ObjectBase.h"
/**
 * \brief �v���W�F�N�g��
 */
namespace FragmentValkyria {
    /**
     * \brief �G�֌W
     */
    namespace Enemy {
        /**
         * \class ���[�W�G�l�~�[�N���X
         * \brief ���[�W�G�l�~�[�N���X���Ǘ�����
         */
        class LargeEnemy : public Object::ObjectBase {
            using Vector4 = AppFrame::Math::Vector4;
            using Matrix44 = AppFrame::Math::Matrix44;
            using InputManager = AppFrame::Input::InputManager;
        public:
            /**
             * \brief �R���X�g���N�^
             * \param gameMain �Q�[���{�̂̎Q��
             */
            LargeEnemy(Game::GameMain& gameMain);
            /**
             * \brief �f�t�H���g�f�X�g���N�^
             */
            virtual ~LargeEnemy() override = default;
            /**
             * \brief �I�u�W�F�N�g�̎�ʂ�Ԃ�
             * \return ���[�W�G�l�~�[
             */
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
            /**
             * \brief �K�g�����O�̐���
             */
            void CreateGatling();
            /**
             * \brief ���[�U�[�̐���
             */
            void CreateLaser();
            /**
             * \brief �{�X�̗̑͂̎擾
             * \return �{�X�̗̑�
             */
            double hp() { return _hp; }
        private:
            /**
             * \brief �����I�u�W�F�N�g�𐶐�����
             */
            void CreateFallObject();
            /**
             * \brief �����I�u�W�F�N�g�ɓ����������m�F
             */
            void HitCheckFromFallObject();
            /**
             * \brief �v���C���[�̉��u��U���̒e�ɓ����������m�F
             */
            void HitCheckFromBullet();
            /**
             * \brief �ړ�����
             * \param moved �ړ��ʂ̃x�N�g��
             */
            void Move(const Vector4& moved);
            /**
             * \brief ��]����
             * \param rotating ��]���邩
             */
            void Rotate(bool& rotating);
            /**
             * \brief ��]�̊p���x�̐ݒ�
             */
            void SetAddRotate();
            int _stateCnt{ 0 };              //!< �e��Ԃɓ����Ă���̐i��
            int _gatlingCnt{ 0 };            //!< �K�g�����O�̒e��ł�
            int _collision{ 0 };             //!< ���f���̃R���W�����t���[���ԍ�
            int _freezeTime{ 0 };            //!< ���S���Ă���Q�[���N���A�܂ł̃t���[����
            bool _fallObjectflag{ false };   //!< �����I�u�W�F�N�g��
            bool _gatlingFlag{ false };
            bool _moving{ false };
            bool _firstRotating{ true };
            bool _endRotating{ true };
            double _rotateDot{ 0.0 };
            double _addRotate{ 1.0 };
            double _hp{ 100.0 };
            int _createNum{ 0 };

            Vector4 _moved{ 0.0, 0.0, 0.0 }; //!< �ړ��ʂ̃x�N�g��

            std::vector<std::pair<double, Vector4>> _objectDistance;

            std::vector<std::string> _actionList;
            std::vector<std::string> _action;
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
               void FootStepSound();
               bool _footRightStep{ false };       //!< ���������̃t���O
               bool _footLeftStep{ false };       //!< ���������̃t���O
                int _footCnt{ 0 };
                bool _endGetplyPos{ true };
            };
            /**
            * \class ���[�U�[�U����ԃN���X
            * \brief ���[�U�[�U����Ԃ̏�������
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
            private:
                bool _createLaser{ false };   //!< ���[�U�[�𐶐�������
            };
        };
    }
}
