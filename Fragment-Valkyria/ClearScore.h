#pragma once
/*****************************************************************//**
 * \file   ClearScore.h
 * \brief  �N���A�X�R�A�\���N���X
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "SpriteBase.h"
 /**
  * \brief �v���W�F�N�g��
  */
namespace FragmentValkyria {
   /**
    * \brief �N���A�֌W
    */
   namespace Clear {
      class ClearScore :public Sprite::SpriteBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̃N���X�̎Q��
          */
         ClearScore(Game::GameMain& gameMain);
         /**
          * \brief ����������
          */
         void Init()override;
         /**
          * \brief �`�揈��
          */
         void Draw()override;
         /**
          * \brief �X�v���C�g��ʂ̎擾
          * \return ���:�N���A�X�R�A
          */
         virtual SpriteType GetSprType() const { return SpriteType::ClearScore; }

      };
   }
}
