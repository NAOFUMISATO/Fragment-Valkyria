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
       * \class �v���C���[�ˌ��G�t�F�N�g�N���X
       * \brief �ˌ��G�t�F�N�g�̔���
       */
      class EffectObjFall :public EffectBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̃N���X�̎Q��
          * \param key �G�t�F�N�g�n���h����o�^����������
          */
         EffectObjFall(Game::GameMain& gameMain, std::string_view key);
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
          * \return �G�t�F�N�g
          */
         EffectType GetEfcType() const override { return EffectType::Effect; }

      };
   }

}
