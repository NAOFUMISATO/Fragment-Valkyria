#pragma once
/*****************************************************************//**
 * \file   PoorEnemyBase.h
 * \brief  �G���G�̊��N���X
 * 
 * \author NAOFUMISATO
 * \date   February 2022
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
       * \class �G���G�̊��N���X
       * \brief �e�G���G�͂��̃N���X��h�����Ē�`����
       */
      class PoorEnemyBase :public Object::ObjectBase {
         using Vector4 = AppFrame::Math::Vector4;
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̃N���X�̎Q��
          */
         PoorEnemyBase(Game::GameMain& gameMain);
         /**
          * \brief ����������
          */
         void Init()override;
         /**
          * \brief �X�V����
          */
         void Update()override;
         /**
          * \brief �`�揈��
          */
         void Draw()override;
         /**
          * \brief �I�u�W�F�N�g��ʂ̎擾
          * \return ���:PoorEnemy
          */
         virtual ObjectType GetObjType() const override { return ObjectType::PoorEnemy; }
         
      protected:
         /**
          * \brief ��]����
          */
         void Rotate();
         /**
          * \brief �v���C���[�̎�U���ɓ��������ꍇ�̏���
          */
         void HitCheckFromBullet();
         /**
          * \brief �v���C���[�̃I�u�W�F�N�g���ˍU���ɓ��������ꍇ�̏���
          */
         void HitCheckFromFallObject();
         void DamageExpression();

         int _collNum{ 0 };                     //!< �R���W�����ԍ��ۑ��p
         int _damageCnt{ 0 };                   //!< �_���[�W�\���̃t���[���J�E���g�ۑ��p
         double _hp{ 0.0 };                     //!< �̗�
         std::vector<std::string> _actionList;  //!< �e��Ԃ̕������S�ēo�^���铮�I�z��
         std::vector<std::string> _action;      //!< �e��Ԃ̕������o�^���A�����_���ɍs�������邽�߂̓��I�z��

      public:
         /**
          * \class �e��Ԃ̊��N���X
          * \brief �e��Ԃ͂��̃N���X��h�����Ē�`����
          */
         class StateBase : public AppFrame::State::StateBaseRoot {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner �G���G�̊��N���X�̎Q��
             */
            StateBase(PoorEnemyBase& owner) : _owner{ owner } {};
            /**
             * \brief �`�揈��
             */
            virtual void Draw() override;

         protected:
            PoorEnemyBase& _owner;   //!< �G���G���N���X�̎Q��
            int _stateCnt;           //!< ��ԑJ�ڎ��Ɏg�p����t���[���ۑ��p
         };

         /**
          * \class �ҋ@��ԃN���X
          * \brief �ҋ@��Ԃ̏�������
          */
         class StateIdle : public StateBase{
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner �G���G�̊��N���X�̎Q��
             */
            StateIdle(PoorEnemyBase& owner) : StateBase{ owner } {};
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
          * \class �T�C�h�X�e�b�v��ԃN���X
          * \brief �T�C�h�X�e�b�v��Ԃ̏�������
          */
         class StateSideStep : public StateBase {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner �G���G�̊��N���X�̎Q��
             */
            StateSideStep(PoorEnemyBase& owner) : StateBase{ owner } {};
            /**
             * \brief ��������
             */
            void Enter() override;
            /**
             * \brief �X�V����
             */
            void Update() override;

         private:
            /**
             * \brief �T�C�h�X�e�b�v���s����Ԃ����邩�`�F�b�N����
             */
            void SideStepDecide();

            Vector4 _moveOnDir{ 0,0,0 };   //!< �X�e�b�v�����ɐ��K�������x�N�g���̕ۑ��p
            Vector4 _moveOnPos{ 0,0,0 };   //!< �X�e�b�v��̍ŏI�ʒu�x�N�g���ۑ��p
         };

         /**
          * \class ������ԃN���X
          * \brief ������Ԃ̏�������
          */
         class StateFall : public StateBase{
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner �G���G�̊��N���X�̎Q��
             */
            StateFall(PoorEnemyBase& owner) : StateBase{ owner } {};
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
          * \class ���S��ԃN���X
          * \brief ���S��Ԃ̏�������
          */
         class StateDie : public StateBase{
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner �G���G�̊��N���X�̎Q��
             */
            StateDie(PoorEnemyBase& owner) : StateBase{ owner } {};
            /**
             * \brief ��������
             */
            void Enter() override;
            /**
             * \brief �X�V����
             */
            void Update() override;
            /**
             * \brief �`�揈��
             */
            void Draw()override;

         private:
            float _opacityRate{ 1.0f }; //!< �s�����x
            int _timeOver{ 0 };         //!< ���S���[�V�����ォ����ł���܂ł̗P�\�t���[��
         };
      };
   }
}
