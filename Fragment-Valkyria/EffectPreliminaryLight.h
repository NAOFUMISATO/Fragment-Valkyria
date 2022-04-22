#pragma once
/*****************************************************************//**
 * \file   EffectPreliminaryLight.h
 * \brief  �{�X�̗\������G�t�F�N�g�N���X
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
      * \class �{�X�̗\������G�t�F�N�g�N���X
      * \brief �{�X�̗\������G�t�F�N�g�̔���
      */
      class EffectPreliminaryLight :public EffectBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param key �G�t�F�N�g�n���h����o�^����������
          */
         EffectPreliminaryLight( std::string_view key);
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
          * \return ���:�\������
          */
         virtual EffectType GetEfcType() const override { return EffectType::PreliminaryLight; }
      };
   }
}
