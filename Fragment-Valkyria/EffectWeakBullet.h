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
       * \class �v���C���[��U���G�t�F�N�g�N���X
       * \brief �v���C���[��U���G�t�F�N�g�̔���
       */
      class EffectWeakBullet :public EffectBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̃N���X�̎Q��
          * \param key �G�t�F�N�g�n���h����o�^����������
          */
         EffectWeakBullet(Game::GameMain& gameMain, std::string_view key);
         /**
          * \brief ����������
          */
         void Init()override;
         /**
          * \brief �X�V����
          */
         void Update()override;
         /**
          * \brief �G�t�F�N�g�̎�ʂ�Ԃ�
          * \return ���:�v���C���[��U��
          */
         virtual EffectType GetEfcType() const override { return EffectType::WeakBullet; }
      };
   }
}
