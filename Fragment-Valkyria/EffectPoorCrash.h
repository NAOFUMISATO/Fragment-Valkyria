#pragma once
/*****************************************************************//**
 * \file   EffectPoorCrash.h
 * \brief  �G���G�̔j��G�t�F�N�g�N���X
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
       * \class �G���G�̔j��G�t�F�N�g�N���X
       * \brief �G���G�̔j��G�t�F�N�g�̔���
       */
      class EffectPoorCrash :public EffectBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param key ResourceServer�ɓo�^�����C�ӂ̕�����̃G�t�F�N�g�L�[
          */
         EffectPoorCrash(std::string_view key);
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
          * \return ���:�G���G�̔j��
          */
         virtual EffectType GetEfcType() const override { return EffectType::PoorCrash; }
      };
   }
}
