#pragma once
/*****************************************************************//**
 * \file   EffectObjectHit.h
 * \brief  �I�u�W�F�N�g�Փˎ��̃G�t�F�N�g�N���X
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
       * \class �I�u�W�F�N�g�Փˎ��̃G�t�F�N�g�N���X
       * \brief �I�u�W�F�N�g�Փˎ��̃G�t�F�N�g�̔���
       */
      class EffectObjectHit:public EffectBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̃N���X�̎Q��
          * \param key �G�t�F�N�g�n���h����o�^����������
          */
         EffectObjectHit(Game::GameMain& gameMain,std::string_view key);
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
          * \return ���:�I�u�W�F�N�g�Փ�
          */
         virtual EffectType GetEfcType() const override { return EffectType::ObjectHit; }
      };
   }
}
