#pragma once
/*****************************************************************//**
 * \file   Player.h
 * \brief  �v���C���[�̏������񂷃N���X
 *
 * \author NAOFUMISATO, AHMD2000
 * \date   December 2021
 *********************************************************************/
#include "ObjectBase.h"
#include "ParamCollision.h"
 /**
  * \brief �v���W�F�N�g��
  */
namespace FragmentValkyria {
   // ��d�C���N���[�h�h�~
   namespace Camera {
      class CameraComponent;
   }
   namespace Model {
      class ModelAnimeComponent;
   }
   /**
    * \brief �v���C���[�֌W
    */
   namespace Player {
      /**
       * \class �v���C���[�N���X
       * \brief �v���C���[���Ǘ�����
       */
      class Player : public Object::ObjectBase {
         using Vector4 = AppFrame::Math::Vector4;
         using Matrix44 = AppFrame::Math::Matrix44;
         using InputManager = AppFrame::Input::InputManager;
      public:
         /**
          * \brief �R���X�g���N�^
          */
         Player();
         /**
          * \brief �f�t�H���g�f�X�g���N�^
          */
         virtual ~Player() override = default;
         /**
          * \brief �I�u�W�F�N�g�̎�ނ�Ԃ�
          * \return �v���C���[
          */
         ObjectType GetObjType() const override { return ObjectType::Player; };
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
          * \brief ���[���h�s��̎擾
          */
         void ComputeWorldTransform() override;
         /**
          * \brief ���G���Ԃ̎擾
          * \return ���G����
          */
         inline int invincibleCnt() { return _invincibleCnt; }
         /**
          * \brief �G�C�������̎擾
          * \return �G�C�����Ȃ��true�A�łȂ����false
          */
         inline bool isAim() { return _isAim; }
         /**
          * \brief ���S���[�V���������̎擾
          * \return ���S���[�V�������Ȃ��true,�łȂ����false
          */
         inline bool isDeadMotion() { return _isDeadMotion; }
         /**
          * \brief �I�u�W�F�N�g�������グ���邩�̎擾
          * \return �I�u�W�F�N�g�������グ����Ȃ�true,�����グ���Ȃ����false
          */
         inline bool isLift() { return _isLift; }
         /**
          * \brief �I�u�W�F�N�g��łĂ邩���擾
          * \return �I�u�W�F�N�g��łĂ�Ȃ�true,�łĂȂ����false
          */
         inline bool objectShoot() { return _objectShoot; }
         /**
          * \brief �I�u�W�F�N�g��łĂ邩�ݒ�
          * \param shoot �I�u�W�F�N�g��łĂ�Ȃ�true,�łĂȂ����false
          */
         inline void objectShoot(bool shoot) { _objectShoot = shoot; }
      private:
         /**
          * \brief �ړ�����
          * \param moved �ړ��ʂ̃x�N�g��
          */
         void Move(const Vector4& moved);
         /**
          * \brief �ˌ�������Ԃ̉�]����
          */
         void ShootRotate();
         /**
          * \brief �v���C���[�̉�]�̏���
          */
         void Rotate();
         /**
          * \brief �����I�u�W�F�N�g�������グ����͈͂ɂ��邩�m�F
          */
         void HitCheckFromFallObjectRange();
         /**
          * \brief �ҋ@��Ԃ̗����I�u�W�F�N�g�Ɠ������Ă��邩�m�F
          */
         void HitCheckFromIdleFallObject(std::string_view state);
         /**
          * \brief �K�g�����O�Ɠ������Ă��邩�m�F
          */
         void HitCheckFromGatling();
         /**
          * \brief �������̗����I�u�W�F�N�g�Ɠ������Ă��邩�m�F
          */
         void HitCheckFromFallObject();
         /**
          * \brief ���[�U�[�Ɠ������Ă��邩�m�F
          */
         void HitCheckFromLaser();
         /**
          * \brief ���[�W�G�l�~�[�Ɠ������Ă��邩�m�F
          */
         void HitCheckFromLargeEnemy();
         /**
          * \brief �G���G�Ɠ������Ă��邩�m�F
          */
         void HitCheckFromPoorEnemy();
         /**
          * \brief ���u��U��������
          */
         void WeakAttack();

         std::unique_ptr<Param::ParamCollision> _collParam; //!< �����蔻��N���X�̒l�Ǘ��N���X�̃|�C���^
         int _freezeTime{ 0 };                              //!< �m�b�N�o�b�N���鎞��
         int _bulletStock{ 5 };                             //!< ���u��U���̎c��e��
         int _invincibleCnt{ 0 };                           //!< ���G����
         int _portionStock{ 3 };                            //!< �|�[�V�����̐�
         int _stateCnt{ 0 };                                //!< �e��Ԃɓ��������̃��[�h�T�[�o�[�̃t���[���J�E���g��
         int _invincibleModeCnt{ 0 };                       //!< ���G��Ԃɓ��������̃��[�h�T�[�o�[�̃t���[���J�E���g��
         int _coolTime{ 0 };                                //!< ���u��U���̃N�[���^�C��
         int _blendMode{ DX_BLENDMODE_ALPHA };              //!< �u�����h���[�h
         bool _isAim{ false };                              //!< �G�C������
         bool _isDeadMotion{ false };                       //!< ���S���[�V��������
         bool _isLift{ true };                              //!< �I�u�W�F�N�g�������グ���邩
         bool _objectShoot{ false };                        //!< �I�u�W�F�N�g��łĂ邩
         double _hp{ 100.0 };                               //!< �q�b�g�|�C���g
         Vector4 _moved{ Vector4() };                       //!< �ړ��ʂ̃x�N�g��
         Vector4 _knockBack{ Vector4() };                   //!< �m�b�N�o�b�N�ʂ̃x�N�g��
         Vector4 _direction{ Vector4() };                   //!< �O�i�����̃x�N�g��
         Vector4 _rotateDir{ Vector4() };                   //!< �v���C���[�̌�������������
         Matrix44 _rightRotation{ Matrix44() };             //!< �x�N�g����90�x��]������}�g���N�X
         Matrix44 _leftRotation{ Matrix44() };              //!< �x�N�g����-90�x��]������}�g���N�X
         Matrix44 _backRotation{ Matrix44() };              //!< �x�N�g����180�x��]������}�g���N�X

      public:
         /**
          * \class �v���C���[��Ԃ̊��N���X
          * \brief �e�v���C���[�̏�Ԃ͂����h�����Ē�`����
          */
         class StateBase : public AppFrame::State::StateBaseRoot {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner �v���C���[�̎Q��
             */
            StateBase(Player& owner) : _owner{ owner } {};
            /**
             * \brief �X�V����
             */
            virtual void Update() override;
            /**
             * \brief �`�揈��
             */
            virtual void Draw() override;

         protected:
            Player& _owner;   //!< �v���C���[�̎Q��
         };
         /**
          * \class �ҋ@��ԃN���X
          * \brief �ҋ@��Ԃ̏�������
          */
         class StateIdle : public StateBase {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner �v���C���[�̎Q��
             */
            StateIdle(Player& owner) : StateBase{ owner } {};
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
         class StateWalk : public StateBase {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner �v���C���[�̎Q��
             */
            StateWalk(Player& owner) : StateBase{ owner } {};
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

         private:
         };
         /**
          * \class �����ԃN���X
          * \brief �����Ԃ̏�������
          */
         class StateRun : public StateBase {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner �v���C���[�̎Q��
             */
            StateRun(Player& owner) : StateBase{ owner } {};
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

         private:
            /**
             * \brief ��������
             */
            void FootStepSound();
            int _footCnt{ 0 };                  //!< ��������o���t���[���̕ۑ��p
            bool _footRightStep{ false };       //!< �E���������̃t���O
            bool _footLeftStep{ false };        //!< �����������̃t���O
         };
         /**
          * \class �I�u�W�F�N�g�ˌ�������ԃN���X
          * \brief �I�u�W�F�N�g�ˌ�������Ԃ̏�������
          */
         class StateShootReady : public StateBase {
         public:
             /**
              * \brief �R���X�g���N�^
              * \param owner �v���C���[�̎Q��
              */
             StateShootReady(Player& owner) : StateBase{ owner } {};
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
             /**
              * \brief �o������
              */
             void Exit()override;

         private:
            bool _changeAnim{ false };              //!< �A�j���[�V������ς�����
         };
         /**
          * \class �m�b�N�o�b�N��ԃN���X
          * \brief �m�b�N�o�b�N��Ԃ̏�������
          */
         class StateKnockBack : public StateBase
         {
         public:
             /**
              * \brief �R���X�g���N�^
              * \param owner �v���C���[�̎Q��
              */
             StateKnockBack(Player& owner) : StateBase{ owner } {};
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
         class StateDie : public StateBase
         {
         public:
             /**
              * \brief �R���X�g���N�^
              * \param owner �v���C���[�̎Q��
              */
             StateDie(Player& owner) : StateBase{ owner } {};
             /**
              * \brief ��������
              */
             void Enter() override;
             /**
              * \brief �X�V����
              */
             void Update() override;
         private:
             int _timeOver{ 0 };    //!< ���S��ԂɂȂ��Ă���Q�[���I�[�o�[�܂ł̃t���[����
         };
         /**
          * \class ���u��U���ˌ�������ԃN���X
          * \brief ���u��U���ˌ�������Ԃ̏�������
          */
         class StateWeakShootReady : public StateBase {
         public:
             /**
              * \brief �R���X�g���N�^
              * \param owner �v���C���[�̎Q��
              */
             StateWeakShootReady(Player& owner) : StateBase{ owner } {};
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
             /**
              * \brief �o������
              */
             void Exit() override;
         };
         /**
          * \class ���U��ԃN���X
          * \brief ���U��Ԃ̏�������
          */
         class StateReload : public StateBase {
         public:
             /**
              * \brief �R���X�g���N�^
              * \param owner �v���C���[�̎Q��
              */
             StateReload(Player& owner) : StateBase{ owner } {};
             /**
              * \brief ��������
              */
             void Enter() override;
             /**
              * \brief �X�V����
              */
             void Update() override;

         private:
             int _reloadCnt{ 0 };      //!< �����[�h��Ԃ̃J�E���g
         };
         /**
          * \class �񕜏�ԃN���X
          * \brief �񕜏�Ԃ̏�������
          */
         class StateRecovery : public StateBase {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner �v���C���[�̎Q��
             */
            StateRecovery(Player& owner) : StateBase{ owner } {};
            /**
             * \brief ��������
             */
            void Enter() override;
            /**
             * \brief �X�V����
             */
            void Update() override;

         private:
            int _recoveryCnt{ 0 };    //!< �񕜏�Ԃ̃J�E���g
         };
      };
   }
}
