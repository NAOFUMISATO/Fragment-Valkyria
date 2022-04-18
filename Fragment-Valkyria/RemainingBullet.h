#pragma once
/*****************************************************************//**
 * \file   RemainingBullet.h
 * \brief  �v���C���[�̎c�e�\���N���X
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
    * \brief �v���C���[
    */
   namespace Player { 
      /**
       * class �v���C���[�̎c�e�\���N���X
       * \brief �v���C���[�̎�U���̎c�e��\������
       */
      class RemainingBullet :public Sprite::SpriteBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̃N���X�̎Q��
          */
         RemainingBullet(Game::GameMain& gameMain);
         /**
          * \brief ����������
          */
         void Init() override;
         /**
          * \brief �X�V����
          */
         void Update() override;
         /**
          * \brief �`�揈��
          */
         void Draw()override;
         /**
          * \brief �X�v���C�g��ʂ̎擾
          * \return ���:�c���U���e��
          */
         virtual SpriteType GetSprType() const { return SpriteType::RemainingBullet; }

      private:
         /**
          * \brief 1�������U����Ă��邩���m�F����
          * \param stock ���݂̒e��
          */
         void StockCheck(int stock);
         std::tuple<int, int, int, int, int> _xPositions{ 0,0,0,0,0 };  //!< �\�����Ă���1�����̂w���W
         std::tuple<bool, bool, bool, bool, bool> _stockFlag{           //!< 1�������U����Ă��邩�̃t���O
            true,true ,true ,true ,true 
         };
      };

   }

}
