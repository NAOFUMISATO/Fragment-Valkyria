#pragma once
/*****************************************************************//**
 * \file   Gatling.h
 * \brief  
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/

#include "ObjectBase.h"

namespace FragmentValkyria {

	namespace Enemy {

		class Gatling : public Object::ObjectBase {
            using Vector4 = AppFrame::Math::Vector4;
            using InputManager = AppFrame::Input::InputManager;
		public:
			Gatling(Game::GameMain& gameMain);

			virtual ~Gatling() override = default;

            /**
            * \brief �I�u�W�F�N�g�̎�ނ�Ԃ�
            * \return �v���C���[
            */
            ObjectType GetObjType() const override { return ObjectType::Gatling; };
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

            Vector4 _moved{ Vector4(0.0, 0.0, 0.0) };
            Vector4 _moveDirection{ Vector4(0.0, 0.0, 0.0) };

            const double Speed{ 2.0 };
            double _radian{ 100.0 };

		public:
            /**
            * \class �K�g�����O�̏�Ԃ̊��N���X
            * \brief �e�K�g�����O�̏�Ԃ͂����h�����Ē�`����
            */
            class StateBase : public AppFrame::State::StateBaseRoot
            {
            public:
                /**
                 * \brief �R���X�g���N�^
                 * \param owner �v���C���[�̎Q��
                 */
                StateBase(Gatling& owner) : _owner{ owner } {};
                /**
                 * \brief �`�揈��
                 */
                virtual void Draw() override;

            protected:
                Gatling& _owner;   //!< �v���C���[�̎Q��
            };
            /**
             * \class �ǌ���ԃN���X
             * \brief �ǌ���Ԃ̏�������
             */
            class StateChase : public StateBase
            {
            public:
                /**
                 * \brief �R���X�g���N�^
                 * \param owner �v���C���[�̎Q��
                 */
                StateChase(Gatling& owner) : StateBase{ owner } {};
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
