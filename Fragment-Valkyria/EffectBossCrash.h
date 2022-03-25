#pragma once
/*****************************************************************//**
 * \file   EffectBossCrash.h
 * \brief  �{�X�̔j��G�t�F�N�g�N���X
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
       * \class �{�X�̔j��G�t�F�N�g�N���X
       * \brief �{�X�̔j��G�t�F�N�g�̕`����s��
       */
      class EffectBossCrash :public EffectBase {
         using Vector4 = AppFrame::Math::Vector4;
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̃N���X�̎Q��
          * \param key ResourceServer�ɓo�^�����C�ӂ̕�����̃G�t�F�N�g�L�[
          */
         EffectBossCrash(Game::GameMain& gameMain, std::string_view key);
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
          * \return ���:�{�X�̔j��
          */
         virtual EffectType GetEfcType() const override { return EffectType::BossCrash; }

      private:
         int _efcCnt{ 0 };                      //!< �t���[���J�E���g�ۑ��p
         std::vector<Vector4> _bornPositions;   //!< �G�t�F�N�g�����ʒu�̓��I�z��
      };
   }
}