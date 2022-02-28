#pragma once
#include "EffectBase.h"
/**
 * \brief �v���W�F�N�g��
 */
namespace FragmentValkyria {
   /**
    * \brief �G�t�F�N�g�֌W
    */
   namespace Effect {
      /**
       * \class �{�X�̃K�g�����O�U���G�t�F�N�g�N���X
       * \brief �K�g�����O�U���G�t�F�N�g�̔���
       */
      class EffectGatlingMuzzleFlash :public EffectBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̃N���X�̎Q��
          * \param key �G�t�F�N�g�n���h����o�^����������
          */
         EffectGatlingMuzzleFlash(Game::GameMain& gameMain, std::string_view key);
         /**
          * \brief ����������
          */
         void Init()override;
         /**
          * \brief �X�V����
          */
         void Update()override;
         /**
          * \brief �G�t�F�N�g�̎�ނ�Ԃ�
          * \return ���:�K�g�����O�̔��Ή�(�}�Y���t���b�V��)
          */
         virtual EffectType GetEfcType() const override { return EffectType::GatlingMuzzleFlash; }
      };
   }

}
