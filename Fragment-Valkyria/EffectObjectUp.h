#pragma once
/*****************************************************************//**
 * \file   EffectObjectUp.h
 * \brief  �I�u�W�F�N�g�㏸���̃G�t�F�N�g�N���X
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
       * \class �I�u�W�F�N�g�㏸���̃G�t�F�N�g�N���X
       * \brief �I�u�W�F�N�g�㏸���̃G�t�F�N�g�̔���
       */
      class EffectObjectUp :public EffectBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̃N���X�̎Q��
          * \param key ResourceServer�ɓo�^�����C�ӂ̕�����̃G�t�F�N�g�L�[
          */
         EffectObjectUp(Game::GameMain& gameMain, std::string_view key);
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
          * \return ���:�I�u�W�F�N�g�㏸
          */
         virtual EffectType GetEfcType() const override { return EffectType::ObjectUp; }
      };
   }
}
