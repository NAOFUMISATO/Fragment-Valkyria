#pragma once
/*****************************************************************//**
 * \file   Bullet.h
 * \brief  �v���C���[�̉��u��U���̒e�̏������񂷃N���X
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/

#include "ObjectBase.h"

namespace FragmentValkyria {

   namespace Player {

      class Bullet : public Object::ObjectBase {
            using Vector4 = AppFrame::Math::Vector4;
            using InputManager = AppFrame::Input::InputManager;
      public:
         Bullet(Game::GameMain& gameMain);

         virtual ~Bullet() override = default;

         ObjectType GetObjType() const override { return ObjectType::Bullet; };

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
            void Move();

            void HitCheckFromLargeEnemy();

            void HitCheckFromPoorEnemyGatling();

            void OutCheckFromStage();

            Vector4 _moved{ Vector4() };

        public:
            /**
            * \class �v���C���[�̉��u��U���̒e�̏�Ԃ̊��N���X
            * \brief �e�v���C���[�̉��u��U���̒e�̏�Ԃ͂����h�����Ē�`����
            */
            class StateBase : public AppFrame::State::StateBaseRoot
            {
            public:
                /**
                 * \brief �R���X�g���N�^
                 * \param owner �v���C���[�̎Q��
                 */
                StateBase(Bullet& owner) : _owner{ owner } {};
                /**
                 * \brief �`�揈��
                 */
                virtual void Draw() override;

            protected:
                Bullet& _owner;   //!< �v���C���[�̎Q��
            };
            /**
             * \class ���ˏ�ԃN���X
             * \brief ���ˏ�Ԃ̏�������
             */
            class StateShoot : public StateBase
            {
            public:
                /**
                 * \brief �R���X�g���N�^
                 * \param owner �v���C���[�̎Q��
                 */
                StateShoot(Bullet& owner) : StateBase{ owner } {};
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
             * \class ���ˏ�ԃN���X
             * \brief ���ˏ�Ԃ̏�������
             */
            class StateDie : public StateBase
            {
            public:
                /**
                 * \brief �R���X�g���N�^
                 * \param owner �v���C���[�̎Q��
                 */
                StateDie(Bullet& owner) : StateBase{ owner } {};
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
