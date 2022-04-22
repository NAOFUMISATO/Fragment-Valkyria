#pragma once
/*****************************************************************//**
 * \file   EffectWeakBullet.h
 * \brief  �v���C���[��U���G�t�F�N�g�N���X
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
       * \class �v���C���[��U���G�t�F�N�g�N���X
       * \brief �v���C���[��U���G�t�F�N�g�̔���
       */
      class EffectWeakBullet :public EffectBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param key �G�t�F�N�g�n���h����o�^����������
          */
         EffectWeakBullet( std::string_view key);
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
