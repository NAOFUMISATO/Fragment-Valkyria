#pragma once
/*****************************************************************//**
 * \file   FallObject.h
 * \brief  
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "ObjectBase.h"

namespace FragmentValkyria {

	namespace FallObject {

		class FallObject : public Object::ObjectBase {
            using InputManager = AppFrame::Input::InputManager;
		public:
			FallObject(Game::GameMain& gameMain);
			virtual ~FallObject() override = default;

			/**
		    * \brief �I�u�W�F�N�g�̎�ނ�Ԃ�
		    * \return �����Ă���I�u�W�F�N�g
		    */
			ObjectType GetObjType() const override { return ObjectType::FallObject; };

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

            const double Gravity{ 0.01 };
            double _fallTimer{ 0.0 };

        public:
            /**
            * \class �v���C���[��Ԃ̊��N���X
            * \brief �e�v���C���[�̏�Ԃ͂����h�����Ē�`����
            */
            class StateBase : public AppFrame::State::StateBaseRoot
            {
            public:
                /**
                 * \brief �R���X�g���N�^
                 * \param owner �����Ă���I�u�W�F�N�g�̎Q��
                 */
                StateBase(FallObject& owner) : _owner{ owner } {};
                /**
                 * \brief �`�揈��
                 */
                virtual void Draw() override;

            protected:
                FallObject& _owner;   //!< �v���C���[�̎Q��
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
                 * \param owner �����Ă���I�u�W�F�N�g�̎Q��
                 */
                StateIdle(FallObject& owner) : StateBase{ owner } {};
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
             * \class ������ԃN���X
             * \brief ������Ԃ̏�������
             */
            class StateFall : public StateBase
            {
            public:
                /**
                 * \brief �R���X�g���N�^
                 * \param owner �v���C���[�̎Q��
                 */
                StateFall(FallObject& owner) : StateBase{ owner } {};
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