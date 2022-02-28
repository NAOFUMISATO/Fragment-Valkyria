#pragma once
/*****************************************************************//**
 * \file   Bullet.h
 * \brief  �v���C���[�̉��u��U���̒e�̏������񂷃N���X
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
    * \brief  �v���C���[�֌W
    */
   namespace Player {
      /**
       * \class �v���C���[�̉��u��U���̒e�̏������񂷃N���X
       * \brief �v���C���[�̉��u��U���̒e�̏�������
       */
      class Bullet : public Object::ObjectBase {
         using Vector4 = AppFrame::Math::Vector4;
         using InputManager = AppFrame::Input::InputManager;
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̂̎Q��
          */
         Bullet(Game::GameMain& gameMain);
         /**
          * \brief �I�u�W�F�N�g�̎�ނ�Ԃ�
          * \return ��U���e
          */
         ObjectType GetObjType() const override { return ObjectType::Bullet; };
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
         /**
          * \brief �ړ�����
          */
         void Move();
         /**
          * \brief ���[�W�G�l�~�[�Ɠ������Ă��邩�m�F
          */
         void HitCheckFromLargeEnemy();
         /**
          * \brief �K�g�����O�U�������Ă���G���G�Ɠ������Ă��邩�m�F
          */
         void HitCheckFromPoorEnemyGatling();
         /**
          * \brief �X�e�[�W�O�ɂ��邩�m�F
          */
         void OutCheckFromStage();

         Vector4 _moved{ Vector4() };     //!< �ړ��ʂ̃x�N�g��

      public:
         /**
         * \class �v���C���[�̉��u��U���̒e�̏�Ԃ̊��N���X
         * \brief �e�v���C���[�̉��u��U���̒e�̏�Ԃ͂����h�����Ē�`����
         */
         class StateBase : public AppFrame::State::StateBaseRoot {
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
         class StateShoot : public StateBase {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner �v���C���[�̎Q��
             */
            StateShoot(Bullet& owner) : StateBase{ owner } {};
            /**
             * \brief �X�V����
             */
            void Update() override;
         };
         /**
          * \class ���S��ԃN���X
          * \brief ���S��Ԃ̏�������
          */
         class StateDie : public StateBase {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner �v���C���[�̎Q��
             */
            StateDie(Bullet& owner) : StateBase{ owner } {};
            /**
             * \brief �X�V����
             */
            void Update() override;
         };
      };
   }
}
