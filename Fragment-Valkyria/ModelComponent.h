#pragma once
/*****************************************************************//**
 * \file   ModelComponent.h
 * \brief  ���f���̊Ǘ��N���X
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "AppFrame.h"
 /**
  * \brief �v���W�F�N�g��
  */
namespace FragmentValkyria {
   // ��d�C���N���[�h�h�~
   namespace Object {
      class ObjectBase;
   }
   /**
    * \brief ���f���֌W
    */
   namespace Model {
      /**
       * \class ���f���̈ꊇ�Ǘ��N���X
       * \brief ���f�����̊Ǘ����s��
       */
      class ModelComponent {
         using Vector4 = AppFrame::Math::Vector4;
         using Matrix44 = AppFrame::Math::Matrix44;
      public:
         /**
          * \brief �R���X�g���N�^
          * \param owner �I�u�W�F�N�g�̊��N���X�̎Q��
          */
         ModelComponent(Object::ObjectBase& owner);
         /**
          * \brief �f�t�H���g�f�X�g���N�^
          */
         virtual ~ModelComponent();
         /**
          * \brief ����������
          */
         virtual void Init();
         /**
          * \brief �X�V����
          */
         virtual void Update();
         /**
          * \brief �`�揈��
          */
         virtual void Draw();
         /**
          * \brief ���f���̐ݒ�
          * \param key ResourceServer�ɓo�^�ς݂̔C�ӂ̕�����
          * \param no ���ꃂ�f���̒ʂ��ԍ�
          * \return �o�^����ʂ��ԍ�
          */
         virtual int SetModel(std::string_view key, int no = 0);
         /**
          * \brief ���W�̐ݒ�
          * \param position ���W
          */
         virtual void SetPosition(Vector4 position);
         /**
          * \brief ��]�p�̐ݒ�
          * \param rotation ��]�p�x
          */
         virtual void SetRotation(Vector4 rotation);
         /**
          * \brief �g�嗦�̐ݒ�
          * \param scale �g�嗦
          */
         virtual void SetScale(Vector4 scale);
         /**
          * \brief ���[���h�s��̐ݒ�
          * \param world ���[���h�s��
          */
         virtual void SetMatrix(Matrix44& world);
         /**
          * \brief ���f���n���h���̐ݒ�
          * \param handle ���f���n���h��
          */
         inline void modelHandle(int handle) { _modelHandle = handle; }
         /**
          * \brief ���f���n���h���̎擾
          * \return ���f���n���h��
          */
         inline int modelHandle() { return _modelHandle; }
         /**
          * \brief ���f���̎��Ȕ����F�̐ݒ�
          * \param index �ݒ肷�铯�ꃂ�f���̒ʂ��ԍ�
          * \param r ��
          * \param g ��
          * \param b ��
          */
         void SetEmiColor(int index, float r, float g, float b);

         void SetStageModels(std::string_view key);

      protected:
         Object::ObjectBase& _owner;   //!< �I�u�W�F�N�g�̊��N���X�̎Q��
         std::string _key;             //!< ���f���y�уA�j���[�V�����Ŏg�p����L�[
         int _modelHandle{ -1 };       //!< ���f���n���h��
         bool _isLighting{ true };     //!< ���C�e�B���O���s�����̃t���O
      };
   }
}