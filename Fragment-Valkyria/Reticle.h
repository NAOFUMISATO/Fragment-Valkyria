#pragma once
/*****************************************************************//**
 * \file   Reticle.h
 * \brief  ���e�B�N���`��N���X
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "SpriteBase.h"
/**
 * \brief �v���W�F�N�g��
 */
namespace FragmentValkyria {
   /**
    * \brief �v���C���[�֌W
    */
   namespace Player {
      /**
       * \class ���e�B�N���`��N���X
       * \brief �v���C���[�̏�Ԃɍ��킹�A���e�B�N���̕`����s��
       */
      class Reticle :public Sprite::SpriteBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̃N���X�̎Q��
          */
         Reticle(Game::GameMain& gameMain);
         /**
          * \brief ����������
          */
         void Init()override;
         /**
          * \brief �X�V����
          */
         void Update()override;
         /**
          * \brief �`�揈��
          */
         void Draw()override;
         /**
          * \brief �X�v���C�g��ʂ̎擾
          * \return ���:Sprite
          */
         virtual SpriteType GetSprType()const { return SpriteType::Sprite; }

      private:
         bool _isAim{ false };   //!< �v���C���[���G�C�����Ă��邩�̃t���O
      };
   }
}
