#pragma once
/*****************************************************************//**
 * \file   EffectPlayerShot.h
 * \brief  �v���C���[�ˌ��G�t�F�N�g�N���X
 * 
 * \author NAOFUMISATO
 * \date   January 2022
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
       * \class �v���C���[�ˌ��G�t�F�N�g�N���X
       * \brief �ˌ��G�t�F�N�g�̔���
       */
      class EffectPlayerShot :public EffectBase{
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̃N���X�̎Q��
          */
         EffectPlayerShot(Game::GameMain& gameMain,std::string_view key);

         /**
          * \brief ����������
          */
         void Init()override;
         /**
          * \brief �X�V����
          */
         void Update()override;
         /**
          * \brief �I�u�W�F�N�g�̎�ʂ̎擾
          * \return �v���C���[�ˌ��G�t�F�N�g
          */
         EffectType GetEfcType() const override { return EffectType::PlayerShot; }

      private:
         bool _play{ true };
      };
   }

}
