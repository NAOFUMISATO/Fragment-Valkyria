#pragma once
/*****************************************************************//**
 * \file   Gatling.h
 * \brief  �K�g�����O�U���̒e�̏������񂷃N���X
 * 
 * \author YUSUKEAHAMEDO
 * \date   January 2022
 *********************************************************************/
#include "ObjectBase.h"
#include "EffectGatlingBullet.h"
#include "ParamCollision.h"
/**
 * \brief �v���W�F�N�g��
 */
namespace FragmentValkyria {
   /**
    * \brief �G�֌W
    */
   namespace Enemy {
      /**
       * \class �K�g�����O�U���̒e�̏������񂷃N���X
       * \brief �K�g�����O�U���̒e�̏�������
       */
      class Gatling : public Object::ObjectBase
      {
         using Vector4 = AppFrame::Math::Vector4;
         using InputManager = AppFrame::Input::InputManager;
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̂̎Q��
          */
         Gatling(Game::GameMain& gameMain);
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
         * \brief �X�V����
         */
         virtual void Update() override;
         /**
         * \brief �`�揈��
         */
         void Draw() override;
         /**
          * \brief �K�g�����O�e�G�t�F�N�g�̎Q�Ƃ̎擾
          * \return �K�g�����O�e�G�t�F�N�g�̎Q��
          */
         Effect::EffectGatlingBullet& efcBullet() const { return *_efcGatling; }

      private:
         /**
          * \brief �ړ�����
          */
         void Move();
         /**
          * \brief �����I�u�W�F�N�g�̃��f���Ɠ������Ă��邩�m�F
          */
         void HitCheckFromObjectModel();
         /**
          * \brief �v���C���[�Ɠ������Ă��邩�m�F
          */
         void HitCheckFromPlayer();
         /**
          * \brief �X�e�[�W�O�ɂ��邩�m�F
          */
         void OutStageCheck();

         std::unique_ptr<Param::ParamCollision> _collParam;         //!< �����蔻��N���X�̒l�Ǘ��N���X�̃|�C���^
         std::unique_ptr<Effect::EffectGatlingBullet> _efcGatling;  //!< �G�t�F�N�g�K�g�����O�e�̃��j�[�N�|�C���^
         Vector4 _moved{ Vector4(0.0, 0.0, 0.0) };                  //!< �ړ��ʂ̃x�N�g��
         Vector4 _moveDirection{ Vector4(0.0, 0.0, 0.0) };          //!< �ړ���������̃x�N�g��

      public:
         /**
         * \class �K�g�����O�̏�Ԃ̊��N���X
         * \brief �e�K�g�����O�̏�Ԃ͂����h�����Ē�`����
         */
         class StateBase : public AppFrame::State::StateBaseRoot {
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
         class StateChase : public StateBase {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner �v���C���[�̎Q��
             */
            StateChase(Gatling& owner) : StateBase{ owner } {};
            /**
             * \brief ��������
             */
            void Enter()override;
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
            StateDie(Gatling& owner) : StateBase{ owner } {};
            /**
             * \brief �X�V����
             */
            void Update() override;
            /**
             * \brief �`�揈��
             */
            void Draw() override;
         };

      };
   }
}
