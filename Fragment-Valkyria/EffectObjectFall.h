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
       * \class�I�u�W�F�N�g�����G�t�F�N�g�N���X
       * \brief �����G�t�F�N�g�̔���
       */
      class EffectObjectFall :public EffectBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̃N���X�̎Q��
          * \param key �G�t�F�N�g�n���h����o�^����������
          */
         EffectObjectFall(Game::GameMain& gameMain, std::string_view key);
         /**
          * \brief ����������
          */
         void Init()override;
         /**
          * \brief �X�V����
          */
         void Update()override;

      };
   }

}
