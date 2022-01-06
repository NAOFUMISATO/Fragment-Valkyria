#pragma once
/*****************************************************************//**
 * \file   Player.h
 * \brief  �v���C���[�N���X
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "ObjectBase.h"
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
          * \param gameMain �Q�[���{�̂̎Q��
          */
         Player(Game::GameMain& gameMain);
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

      private:
         /**
          * \brief �ړ�����
          */
         virtual void Move();

         const double MoveSpeed{ GetLoadJson().GetParam("player", "movespeed") };      //!< �ړ����x
         double _rotateSpeed{ 0 };       //!< ��]���x

         Vector4 _moved{ Vector4() };
         Vector4 _direction{ Vector4() };

         Matrix44 _rightRotation{ Matrix44() };
         Matrix44 _leftRotation{ Matrix44() };
         Matrix44 _backRotation{ Matrix44() };

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
             * \brief �`�揈��
             */
            void Draw() override;

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
         };
         /**
          * \class �U����ԃN���X
          * \brief �U����Ԃ̏�������
          */
         class StateAttack : public StateBase {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner �v���C���[�̎Q��
             */
            StateAttack(Player& owner) : StateBase{ owner } {};
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
      };
   }
}
