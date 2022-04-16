#pragma once
/*****************************************************************//**
 * \file   FallObject.h
 * \brief  �����I�u�W�F�N�g�̏������񂷃N���X
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "ObjectBase.h"
#include "EffectObjectUp.h"
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
       * \class �����I�u�W�F�N�g�̏������񂷃N���X
       * \brief �����I�u�W�F�N�g�̏�������
       */
      class FallObject : public Object::ObjectBase {
         using InputManager = AppFrame::Input::InputManager;
         using Vector4 = AppFrame::Math::Vector4;
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̂̎Q��
          */
         FallObject(Game::GameMain& gameMain);
         /**
          * \brief ����������
          */
         virtual void Init() override;
         /**
          * \brief ���͏���
          * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
          */
         void Input(InputManager& input) override;
         /**
          * \brief �X�V����
          */
         void Update() override;
         /**
          * \brief �`�揈��
          */
         void Draw() override;
         /**
          * \brief �I�u�W�F�N�g�̎�ނ�Ԃ�
          * \return ���:�����I�u�W�F�N�g
          */
         ObjectType GetObjType() const override { return ObjectType::FallObject; };
         /**
          * \brief �c���I�u�W�F�N�g���m�F
          * \return �c���I�u�W�F�N�g���ǂ���
          */
         bool residual() { return _residual; }
         /**
          * \brief �c���I�u�W�F�N�g���ǂ����ݒ�
          * \param residual �c���I�u�W�F�N�g���ǂ���
          */
         void residual(bool residual) { _residual = residual; }
         /**
          * \brief �J�v�Z�����`�������ڂ̍��W�̎擾
          * \return �J�v�Z�����`�������ڂ̍��W
          */
         Vector4 capsulePos1() { return _capsulePos1; }
         /**
          * \brief �J�v�Z�����`�������ڂ̍��W�̎擾
          * \return �J�v�Z�����`�������ڂ̍��W
          */
         Vector4 capsulePos2() { return _capsulePos2; }
         /**
          * \brief ���f���̃R���W�����t���[���̖��O�̐ݒ�
          * \param collision ���f���̃R���W�����t���[���̖��O
          */
         void collisionName(std::string_view collision) { _collisionName = collision; }
         /**
          * \brief ���f���̃R���W�����t���[���̖��O�̎擾
          * \return ���f���̃R���W�����t���[���̖��O
          */
         std::string_view collisionName() { return _collisionName; }

      private:
         /**
          * \brief �v���C���[�������͈͓��ɂ��邩�m�F
          */
         void HitCheckFromPlayerPoint();
         /**
          * \brief ���[�W�G�l�~�[�̃��f���Ɠ������Ă��邩�m�F
          */
         void HitCheckFromLargeEnemy();
         /**
          * \brief ���[�U�[�Ɠ������Ă��邩�m�F
          */
         void HitCheckFromLaser();
         /**
          * \brief �K�g�����O�U�������Ă���G���G�Ɠ������Ă��邩�m�F
          */
         void HitCheckFromPoorEnemy();
         /**
          * \brief �X�e�[�W�O�ɂ��邩�m�F
          */
         void OutStageCheck();
         /**
          * \brief �v���C���[���m�b�N�o�b�N���Ă��邩�m�F
          */
         void CheckPlayerKnockBack();
         /**
          * \brief �������Ȃ����]�����鏈��
          */
         void Save();
         /**
          * \brief ��ɏグ�Ă�������
          */
         void Up();
         /**
          * \brief �ł������̒����_�Ɍ������Đi�ޏ���
          */
         void Shoot();
         /**
          * \brief �J�v�Z���̈ʒu�̍X�V
          */
         void SetCapsulePos();

         std::unique_ptr<Param::ParamCollision> _collParam;        //!< �����蔻��N���X�̒l�Ǘ��N���X�̃|�C���^
         std::unique_ptr<Effect::EffectObjectUp> _efcUp;           //!< �I�u�W�F�N�g�㏸�N���X�̃|�C���^
         double _fallTimer{ 0.0 };                                 //!< ������Ԃ̐i��
         double _upDownAngle{ 0.0 };                               //!< �ӂ�ӂ킳���鎞�̃T�C���̒l�����Ƃ��̊p�x
         double _rotateAngle{ 0.0 };                               //!< �s�K���ȉ�]�������鎞�̃T�C���̒l�����Ƃ��̊p�x
         bool _saveFlag{ false };                                  //!< �ӂ�ӂ핂�����邩
         bool _isFall{ true };                                     //!< ������Ԃ��ǂ���
         bool _residual{ true };                                   //!< �c���I�u�W�F�N�g��
         bool _efcBorn{ false };                                   //!< �G�t�F�N�g�͏o��������
         int _collision{ 0 };                                      //!< ���f���̃R���W�����t���[���ԍ�
         Vector4 _shootVec{ Vector4(0.0, 0.0, 0.0) };              //!< �ł������̒����_�֌������x�N�g��
         Vector4 _plyRightHandVec{ Vector4(0.0, 0.0, 0.0) };       //!< �ł������̒����_�֌������x�N�g��
         Vector4 _capsulePos1{ Vector4(0.0, 0.0, 0.0) };           //!< �J�v�Z�����`�������ڂ̈ʒu
         Vector4 _capsulePos2{ Vector4(0.0, 0.0, 0.0) };           //!< �J�v�Z�����`�������ڂ̈ʒu
         std::string_view _collisionName{ "" };                    //!< ���f���̃R���W�����t���[���̖��O
         std::vector<int> _fallPointHandles{ -1 };                 //!< �����n�_�r���{�[�h�摜�n���h��

      public:
         /**
         * \class �����I�u�W�F�N�g�̏�Ԃ̊��N���X
         * \brief �e�����I�u�W�F�N�g�̏�Ԃ͂����h�����Ē�`����
         */
         class StateBase : public AppFrame::State::StateBaseRoot {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner �����I�u�W�F�N�g�̎Q��
             */
            StateBase(FallObject& owner) : _owner{ owner } {};
            /**
             * \brief �`�揈��
             */
            virtual void Draw() override;

         protected:
            FallObject& _owner;   //!< �����I�u�W�F�N�g�̎Q��
         };
         /**
          * \class �ҋ@��ԃN���X
          * \brief �ҋ@��Ԃ̏�������
          */
         class StateIdle : public StateBase {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner �����I�u�W�F�N�g�̎Q��
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
         class StateFall : public StateBase {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner �����I�u�W�F�N�g�̎Q��
             */
            StateFall(FallObject& owner) : StateBase{ owner } {};
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
            void Draw() override;
         };
         /**
          * \class �㏸��Ԃ̃N���X
          * \brief �㏸��Ԃ̏�������
          */
         class StateUp : public StateBase {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner �����I�u�W�F�N�g�̎Q��
             */
            StateUp(FallObject& owner) : StateBase{ owner } {};
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
          * \class ������Ԃ̃N���X
          * \brief ������Ԃ̏�������
          */
         class StateSave : public StateBase {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner �����I�u�W�F�N�g�̎Q��
             */
            StateSave(FallObject& owner) : StateBase{ owner } {};
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
          * \class ���ˏ�Ԃ̃N���X
          * \brief ���ˏ�Ԃ̏�������
          */
         class StateShoot : public StateBase {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner �����I�u�W�F�N�g�̎Q��
             */
            StateShoot(FallObject& owner) : StateBase{ owner } {};
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
         class StateDie : public StateBase {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner �����I�u�W�F�N�g�̎Q��
             */
            StateDie(FallObject& owner) : StateBase{ owner } {};
            /**
             * \brief �X�V����
             */
            void Update() override;
         };
      };
   }
}
