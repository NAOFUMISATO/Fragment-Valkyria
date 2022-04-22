#pragma once
/*****************************************************************//**
 * \file   EffectGatlingBullet.h
 * \brief  �G�̃K�g�����O�e�G�t�F�N�g�N���X
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
       * \class �{�X�̃K�g�����O�U���G�t�F�N�g�N���X
       * \brief �K�g�����O�U���G�t�F�N�g�̔���
       */
      class EffectGatlingBullet :public EffectBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param key �G�t�F�N�g�n���h����o�^����������
          */
         EffectGatlingBullet(std::string_view key);
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
          * \return ���:�K�g�����O�e
          */
         virtual EffectType GetEfcType() const override { return EffectType::GatlingBullet; }
      };
   }

}
