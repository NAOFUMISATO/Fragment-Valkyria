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
          * \brief �f�X�g���N�^
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
          * \param index �ݒ肷�郂�f���̃}�e���A���ԍ�
          * \param r ��
          * \param g ��
          * \param b ��
          */
         void SetEmiColor(int index, float r, float g, float b);
         /**
          * \brief ���C�e�B���O�t���O��false�ɂ���
          */
         inline void LightingOFF() { _isLighting = false; }
         /**
          * \brief �s�N�Z�����C�e�B���O��true�ɂ���
          */
         inline void PixelLightingON() { _isPixelLighting = true; }
         /**
          * \brief ���f������w�薼�̃t���[�����擾����
          * \param frameName �t���[����
          * \return �t���[���ԍ�
          */
         int FindFrame(std::string_view frameName);
         /**
          * \brief ���f���̎w�薼�̃t���[������w�薼�̎q�t���[�����擾����
          * \param frameName �t���[����
          * \param childName �q�t���[����
          * \return �t���[���ԍ�
          */
         int FindFrameChild(std::string_view frameName, std::string_view childName);

      protected:
         Object::ObjectBase& _owner;   //!< �I�u�W�F�N�g�̊��N���X�̎Q��
         std::string _key;             //!< ���f���y�уA�j���[�V�����Ŏg�p����L�[
         int _modelHandle{ -1 };       //!< ���f���n���h��
         bool _isLighting{ true };     //!< ���C�e�B���O���s�����̃t���O
         bool _isPixelLighting{false}; //!< �s�N�Z���P�ʂŃ��C�e�B���O���s�����̃t���O
      };
   }
}