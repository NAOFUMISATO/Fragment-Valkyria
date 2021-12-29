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
          * \brief ���f���̃A�j���[�V�����̓o�^
          * \param key �A�j���[�V�����Ɋ֘A�t����C�ӂ̕�����
          * \param animeIndex �֘A�t����A�j���ԍ�
          */
         void Register(std::string_view key, int animeIndex);
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
          * \param key �ύX��̃A�j���[�V�����Ɋ֘A�t�����C�ӂ̕�����
          * \param repeate �A�j���[�V���������[�v�Đ����邩�̃t���O
          */
         void ChangeAnime(std::string_view key, bool repeate = false);
         /**
          * \brief �A�j���[�V�����̑����Ԃ̎擾
          * \return �A�j���[�V�����̑�����
          */
         float GetPlayProgress() const { return _playTime / _totalTime; }
         /**
          * \brief �A�j���[�V�������J��Ԃ����񐔂̎擾
          * \return �A�j���[�V�������J��Ԃ�����
          */
         int repeatedCount() const { return _repeatedCount; }
         /**
          * \brief �A�j���[�V�������Ԃ��擾
          * \return �A�j���[�V��������
          */
         float playTime() const { return _playTime; }
         /**
          * \brief �Đ����Ԃ̊|���̐ݒ�
          * \param �Đ����Ԃ̊|��
          */
         void timeRate(float timeRate) { _timeRate = timeRate; };

      private:
         std::unordered_map<std::string, int> _animeRegistry;   //!< ���f���A�j���[�V������o�^����A�z�z��
         int _animIndex{ -1 };      //!< �A�j���ԍ�
         int _attachIndex{ -1 };    //!< �A�^�b�`����A�j���ԍ�
         float _playTime{ 0 };      //!< �A�j���[�V�����̍Đ�����
         float _totalTime{ 0 };     //!< �A�j���[�V�����̑��Đ�����
         float _timeRate{ 1.f };    //!< �Đ����Ԃ̊|��
         int _repeatedCount{ 0 };   //!< �A�j���[�V�������J��Ԃ�����
         bool _repeate{ false };    //!< �A�j���[�V���������[�v�����邩�̃t���O
      };
   }
}