#pragma once
/*****************************************************************//**
 * \file   StageModelComponent.h
 * \brief  �X�e�[�W�`����ꊇ�Ǘ�����N���X
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "AppFrame.h"
/**
 * \brief �v���W�F�N�g��
 */
namespace FragmentValkyria {
   namespace Object {
      class ObjectBase;
   }
   /**
    * \brief �X�e�[�W��
    */
   namespace Stage {
      /**
       * \class �X�e�[�W�`����ꊇ�Ǘ�����N���X
       * \brief �X�e�[�W���̎擾�y�ѕ`����ꊇ�ōs��
       */
      class StageModelComponent {
         using Vector4 = AppFrame::Math::Vector4;
         using Matrix44 = AppFrame::Math::Matrix44;
      public:
         /**
          * \brief �R���X�g���N�^
          * \param owner �I�u�W�F�N�g���N���X�̎Q��
          */
         StageModelComponent(Object::ObjectBase& owner);
         /**
          * \brief �f�t�H���g�f�X�g���N�^
          */
         ~StageModelComponent()=default;
         /**
          * \brief ����������
          */
         void Init();
         /**
          * \brief �X�V����
          */
         void Update();
         /**
          * \brief �`�揈��
          */
         void Draw();
         /**
          * \brief �X�e�[�W���ƃn���h���𓮓I�z��ɓo�^����
          * \param key �A�z�z��Ɋ֘A�t�����C�ӂ̕�����
          */
         void SetModels(std::string_view key);
         /**
          * \brief ���f���n���h���̓��I�z��ւ̐ݒ�
          * \param handles �ݒ肷��摜�n���h���̓��I�z��
          */
         inline void modelHandles(std::vector<int> handles) { _modelHandles = handles; }
         /**
          * \brief ���f���n���h���̓��I�z��̎擾
          * \return ���f���n���h���̓��I�z��
          */
         inline std::vector<int> modelHandles() { return _modelHandles; }
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
          * \brief �s�N�Z�����C�e�B���O�t���O��true�ɂ���
          */
         inline void PixelLightingON() { _isPixelLighting = true; }
         /**
          * \brief �X�e�[�W�p�[�c�̃��f���n���h���̓��I�z�񂩂�w��̃R���W�����t���[�������郂�f���n���h���ƃR���W�����ԍ���Ԃ�
          * \param collName �R���W�����t���[����
          * \return ���f���n���h���ƃR���W�����ԍ�
          */
         std::pair<int, int> GetHandleAndCollNum(std::string_view collName);

      protected:
         /**
          * \brief �w��̃n���h���̍��W�ݒ�
          * \param handle �n���h��
          * \param position ���W
          */
         void SetPosition(int handle, Vector4 position);
         /**
          * \brief �w��̃n���h���̉�]�p�ݒ�
          * \param handle �n���h��
          * \param rotation ��]�p
          */
         void SetRotation(int handle, Vector4 rotation);
         /**
          * \brief �w��̃n���h���̊g�嗦�ݒ�
          * \param handle �n���h��
          * \param scale �g�嗦
          */
         void SetScale(int handle, Vector4 scale);
         /**
          * \brief �w��̃n���h���̃��[���h�s��̐ݒ�
          * \param handle �n���h��
          * \param world ���[���h�s��
          */
         void SetMatrix(int handle, Matrix44& world);

         Object::ObjectBase& _owner;                 //!< �I�u�W�F�N�g�x�[�X�̎Q��
         std::vector<int> _modelHandles{ -1 };       //!< ���f���n���h��
         bool _isLighting{ true };     //!< ���C�e�B���O���s�����̃t���O
         bool _isPixelLighting{ false };
      };
   }
}
