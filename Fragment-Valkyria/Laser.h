#pragma once
/*****************************************************************//**
 * \file   Laser.h
 * \brief  ���[�U�[�̏������񂷃N���X
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "ObjectBase.h"

namespace FragmentValkyria {

	namespace Enemy {

		class Laser : public Object::ObjectBase {
            using Vector4 = AppFrame::Math::Vector4;
		public:
			Laser(Game::GameMain& gameMain);

			virtual ~Laser() override = default;
			/**
			* \brief �I�u�W�F�N�g�̎�ނ�Ԃ�
			* \return ���[�U�[
			*/
			ObjectType GetObjType() const override { return ObjectType::Laser; };
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

            void end(Vector4 endPos) { _end = endPos; }
            inline Vector4 end() { return _end; }

		private:

            Vector4 _end{ Vector4(0.0, 0.0, 0.0) };
            int _stateCnt{ 0 };
		public:
            /**
            * \class ���[�U�[�̏�Ԃ̊��N���X
            * \brief �e���[�U�[�̏�Ԃ͂����h�����Ē�`����
            */
            class StateBase : public AppFrame::State::StateBaseRoot
            {
            public:
                /**
                 * \brief �R���X�g���N�^
                 * \param owner �v���C���[�̎Q��
                 */
                StateBase(Laser& owner) : _owner{ owner } {};
                /**
                 * \brief �`�揈��
                 */
                virtual void Draw() override;

            protected:
                Laser& _owner;   //!< ���[�U�[�̎Q��
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
                 * \param owner ���[�U�[�̎Q��
                 */
                StateIdle(Laser& owner) : StateBase{ owner } {};
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
