#pragma once
/*****************************************************************//**
 * \file   EffectBossCharge.h
 * \brief  �{�X�̃r�[���`���[�W�G�t�F�N�g�N���X
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
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
       * \class �{�X�̃r�[���`���[�W�G�t�F�N�g�N���X
       * \brief �{�X�̃r�[���`���[�W�G�t�F�N�g�̕`����s��
       */
      class EffectBossCharge :public EffectBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̃N���X�̎Q��
          * \param key ResourceServer�ɓo�^�����C�ӂ̕�����̃G�t�F�N�g�L�[
          */
         EffectBossCharge(Game::GameMain& gameMain, std::string_view key);
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
          * \return ���:�{�X�̃r�[���`���[�W
          */
         virtual EffectType GetEfcType() const override { return EffectType::BossCharge; }
      };
   }
}
