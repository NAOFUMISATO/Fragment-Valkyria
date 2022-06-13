#pragma once
/*****************************************************************//**
 * \file   Laser.h
 * \brief  ���[�U�[�̏������񂷃N���X
 * 
 * \author YUSUKEAHAMEDO
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
       * \class ���[�U�[�̏������񂷃N���X
       * \brief �{�X�������[�U�[�̏�������
       */
      class Laser : public Object::ObjectBase {
         using Vector4 = AppFrame::Math::Vector4;
      public:
         /**
          * \brief �R���X�g���N�^
          */
         Laser();
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
          * \return ���[�U�[
          */
         ObjectType GetObjType() const override { return ObjectType::Laser; }
         /**
          * \brief ���[�U�[�̏I�_��ݒ�
          * \param endPos �ݒ肷��I�_���W
          */
         void end(Vector4 endPos) { _end = endPos; }
         /**
          * \brief ���[�U�[�̏I�_���W���擾
          * \return ���[�U�[�̏I�_���W
          */
         inline Vector4 end() { return _end; }

      private:
         Vector4 _end{ 0.0, 0.0, 0.0 };  //!< ���[�U�[�I�_���W

      public:
         /**
         * \class ���[�U�[�̏�Ԃ̊��N���X
         * \brief �e���[�U�[�̏�Ԃ͂����h�����Ē�`����
         */
         class StateBase : public AppFrame::State::StateBaseRoot {
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
            Laser& _owner;        //!< ���[�U�[�̎Q��
            int _stateCnt{ 0 };   //!< �e��Ԃ֓��������̃t���[���J�E���g�ۑ��p
         };
         /**
          * \class �Ǝˏ�ԃN���X
          * \brief �Ǝˏ�Ԃ̏�������
          */
         class StateIrradiation : public StateBase {
         public:
            /**
             * \brief �R���X�g���N�^
             * \param owner ���[�U�[�̎Q��
             */
            StateIrradiation(Laser& owner) : StateBase{ owner } {};
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
