#pragma once
/*****************************************************************//**
 * \file   ModelAnimeComponent.h
 * \brief  ���f���̃A�j���[�V�����̈ꊇ�Ǘ��N���X
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "ModelComponent.h"
 /**
  * \brief �v���W�F�N�g��
  */
namespace FragmentValkyria {
   /**
    * \brief ���f���֌W
    */
   namespace Model {
      /**
       * \class ���f���̃A�j���[�V�����̈ꊇ�Ǘ��N���X
       * \brief �A�j���[�V�����̊Ǘ����s��
       */
      class ModelAnimeComponent :public ModelComponent {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param owner �I�u�W�F�N�g�̊��N���X�̎Q��
          */
         ModelAnimeComponent(Object::ObjectBase& owner);
         /**
          * \brief �f�X�g���N�^
          */
         virtual ~ModelAnimeComponent() override;
         /**
          * \brief ����������
          */
         void Init() override;
         /**
          * \brief �X�V����
          */
         void Update() override;
         /**
          * \brief �`�揈��
          */
         void Draw() override;

         /**
          * \brief �A�j���[�V�����̕ύX
          * \param animeName �A�j���[�V������
          * \param repeate �A�j���[�V���������[�v�Đ����邩�̃t���O
          */
         void ChangeAnime(std::string_view animeName, bool repeate,double animeSpeed=1.0);
         /**
          * \brief �A�j���[�V�����̑����Ԃ̎擾
          * \return �A�j���[�V�����̑�����
          */
         inline float GetPlayProgress() const { return _playTime / _totalTime; }
         /**
          * \brief �A�j���[�V�������J��Ԃ����񐔂̎擾
          * \return �A�j���[�V�������J��Ԃ�����
          */
         inline int repeatedCount() const { return _repeatedCount; }
         /**
          * \brief �A�j���[�V�������Ԃ��擾
          * \return �A�j���[�V��������
          */
         inline float playTime() const { return _playTime; }
         /**
          * \brief �Đ����Ԃ̊|���̎擾
          * \return �Đ����Ԃ̊|��
          */
         inline double timeRate() const { return _timeRate; }
         /**
          * \brief �Đ����Ԃ̊|���̐ݒ�
          * \param �Đ����Ԃ̊|��
          */
         inline void timeRate(double timeRate) { _timeRate = timeRate; };

      private:
         int _animIndex{ -1 };      //!< ���݂̃A�j���ԍ�
         int _newAnimIndex{ -1 };   //!< �V�����A�^�b�`����A�j���ԍ�
         int _attachNum{ -1 };      //!< ���݂̃A�^�b�`�ԍ�
         int _nextAttachNum{ -1 };  //!< �A�j���[�V������؂�ւ���ۂ̎��̃A�^�b�`�ԍ�
         bool _blending{ false };   //!< �A�j���[�V�����u�����h�����̔���
         float _blendRate{ 0.f };   //!< �A�j���[�V�����u�����h��
         float _playTime{ 0 };      //!< �A�j���[�V�����̍Đ�����
         float _totalTime{ 0 };     //!< �A�j���[�V�����̑��Đ�����
         double _timeRate{ 1.0 };   //!< �Đ����Ԃ̊|��
         int _repeatedCount{ 0 };   //!< �A�j���[�V�������J��Ԃ�����
         bool _repeate{ false };    //!< �A�j���[�V���������[�v�����邩�̃t���O
      };
   }
}
