#pragma once
/*****************************************************************//**
 * \file   LargeEnemy.h
 * \brief  ���[�W�G�l�~�[�̏������񂷃N���X
 * 
 * \author AHMD2000, NAOFUMISATO
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
         /**
          * \brief ���[�U�[�U�����s���Ă��邩
          * \return �s���Ă����true,�łȂ����false
          */
         bool isLaser() { return _isLaser; }

      private:
         /**
          * \brief �����I�u�W�F�N�g�𐶐�����
          */
         void CreateFallObject();
         /**
          * \brief ���[�U�[��łʒu��ݒ�
          */
         void SetLaserPosition();
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
          * \brief �e�s���ւ̈ړ�����
          */
         void Action();
         /**
          * \brief �p���x�ɂ���]����
          * \param rotating ��]���邩
          */
         void AugularRotate(bool& rotating);
         /**
          * \brief �ʐςɂ���]
          */
         void AreaRotate(bool& rotating);
         /**
          * \brief ��]�̊p���x�̐ݒ�
          */
         void SetAddRotate();
         /**
          * \brief �X�^���l�̍X�V�Ɗm�F
          */
         void StunCheck();

         int _gatlingCnt{ 0 };                                      //!< �K�g�����O�̒e��ł�
         int _wholeCollision{ 0 };                                  //!< ���f���̑S�̂̃R���W�����t���[���ԍ�
         int _bodyCollision{ 0 };                                   //!< ���f���̓��̂̃R���W�����t���[���ԍ�
         int _weakNessesCollision{ 0 };                             //!< ���f���̎�_�̃R���W�����t���[���ԍ�
         int _faceCollision{ 0 };                                   //!< ���f���̊�̃R���W�����t���[���ԍ�
         int _freezeTime{ 0 };                                      //!< ���S���Ă���Q�[���N���A�܂ł̃t���[����
         int _createNum{ 0 };                                       //!< ��������Ă��闎���I�u�W�F�N�g�̐�
         bool _firstRotating{ true };                               //!< �ړ����ŏ��Ɉړ������ɉ�]���邩
         bool _endRotating{ true };                                 //!< �ړ����Ō�Ƀv���C���[�̕����ɉ�]���邩
         bool _firstAngryAction{ true };                            //!< �q�b�g�|�C���g��50����؂�����̍ŏ��̍s���̏ꍇ
         bool _rotating{ false };                                   //!< ��]���������邩
         bool _attack{ false };                                     //!< �U�������Ă��邩
         bool _isLaser{ false };                                    //!< ���[�U�[�U�����s���Ă��邩
         double _fanAngle{ 0.0 };                                   //!< ���K�g�����O�U��������Ƃ��̌������擾����Ƃ��x�N�g������]������p�x
         double _rotateDot{ 0.0 };                                  //!< ���������������̃x�N�g���ƃt�H���[�h�x�N�g����90�x��]�������x�N�g���̓��ς̌���
         double _addRotate{ 0.0 };                                  //!< �p���x
         double _hp{ 0.0 };                                         //!< �q�b�g�|�C���g
         double _stunValue{ 0.0 };                                  //!< �X�^���l
         double _rotateEnlarge{ 0.0 };                              //!< ���������������̃x�N�g����傫������l
         Vector4 _moved{ 0.0, 0.0, 0.0 };                           //!< �ړ��ʂ̃x�N�g��
         Vector4 _rotateDir{ 0.0, 0.0, 0.0 };                       //!< ��]�̌����̃x�N�g��
         std::vector<std::pair<double, Vector4>> _objectDistance;   //!< �e�����I�u�W�F�N�g����v���C���[�ւ̋�����2��Ɗe�����I�u�W�F�N�g�̈ʒu�̃y�A�̓��I�z��
         std::vector<std::string> _actionList;                      //!< �e�s����Ԃւ̕�����̓��I�z��
         std::vector<std::string> _action;                          //!< �s��������͈͂𐧌������s����Ԃւ̕�����̓��I�z��
      public:
         /**
         * \class ���[�W�G�l�~�[�̏�Ԃ̊��N���X
         * \brief �e���[�W�G�l�~�[�̏�Ԃ͂����h�����Ē�`����
         */
         class StateBase : public AppFrame::State::StateBaseRoot {
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
            int _stateCnt{ 0 };   //!< �e��Ԃ֓��������̃t���[���J�E���g�ۑ��p
         };
         /**
         * \class �ҋ@��ԃN���X
         * \brief �ҋ@��Ԃ̏�������
         */
         class StateIdle : public StateBase {
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
             * \brief �X�V����
             */
            void Update() override;
         };
         /**
         * \class �I�u�W�F�N�g������ԃN���X
         * \brief �I�u�W�F�N�g������Ԃ̏�������
         */
         class StateFallObject : public StateBase {
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
             * \brief �X�V����
             */
            void Update() override;
         };
         /**
         * \class �I�u�W�F�N�g�A��������ԃN���X
         * \brief �I�u�W�F�N�g�A��������Ԃ̏�������
         */
         class StateConsecutiveFallObject : public StateBase
         {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner ���[�W�G�l�~�[�̎Q��
             */
            StateConsecutiveFallObject(LargeEnemy& owner) : StateBase{ owner } {};
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
             * \brief �����I�u�W�F�N�g�̐���
             */
            void CreateFallObject();

            int _fallObjectNum{ 0 };            //!< �����I�u�W�F�N�g�𐶐����鐔
         };
         /**
         * \class �K�g�����O�U����ԃN���X
         * \brief �K�g�����O�U����Ԃ̏�������
         */
         class StateGatling : public StateBase {
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
         private:
            int _gatlingFrameCnt{ 0 };      //!< �K�g�����O�U�������邽�߂̉�]���Ă��Ȃ����Ɍo�߂�����t���[���J�E���g
         };
         /**
         * \class ���K�g�����O�U����ԃN���X
         * \brief ���K�g�����O�U����Ԃ̏�������
         */
         class StateFanGatling : public StateBase {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner ���[�W�G�l�~�[�̎Q��
             */
            StateFanGatling(LargeEnemy& owner) : StateBase{ owner } {};
            /**
             * \brief ��������
             */
            void Enter() override;
            /**
             * \brief �X�V����
             */
            void Update() override;
         private:
            int _fanGatlingFrameCnt{ 0 };   //!< ���K�g�����O�U�������邽�߂̉�]�����Ă��Ȃ����Ɍo�߂�����t���[���J�E���g
         };
         /**
         * \class ����ԃN���X
         * \brief ���S��Ԃ̏�������
         */
         class StateDie : public StateBase {
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
         class StateMove : public StateBase {
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
            /**
             * \brief �����̏���
             */
            void FootStepSound();
            bool _footRightStep{ false };       //!< ���������̃t���O
            bool _footLeftStep{ false };        //!< ���������̃t���O
            int _footCnt{ 0 };                  //!< 
            bool _endGetplyPos{ true };         //!< �ړ���Ƀv���C���[�ւ̌����̃x�N�g�����擾���邩
         };
         /**
         * \class ���[�U�[�U����ԃN���X
         * \brief ���[�U�[�U����Ԃ̏�������
         */
         class StateLaser : public StateBase {
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
            /**
             * \brief �o������
             */
            void Exit()override;
         
         private:
            /**
             * \brief ���[�U�[�[�U�y�є��ˈʒu�̎擾
             * \return ���[�U�[�[�U�y�є��ˈʒu
             */
            Vector4 GetLaserPos();
            bool _createLaser{ false };       //!< ���[�U�[�𐶐����邩
         };
         /**
         * \class �C���ԃN���X
         * \brief �C���Ԃ̏�������
         */
         class StateStun : public StateBase {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner ���[�W�G�l�~�[�̎Q��
             */
            StateStun(LargeEnemy& owner) : StateBase{ owner } {};
            /**
             * \brief ��������
             */
            void Enter() override;
            /**
             * \brief �X�V����
             */
            void Update() override;
            /**
             * \brief �o������
             */
            void Exit()override;

         private:
            
         };
      };
   }
}
