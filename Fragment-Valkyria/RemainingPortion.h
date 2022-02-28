#pragma once
/*****************************************************************//**
 * \file   RemainingPortion.h
 * \brief  �c��񕜃|�[�V�����`��N���X
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
       * \class �c��񕜃|�[�V�����`��N���X
       * \brief �c��񕜃|�[�V��������`�悷��
       */
      class RemainingPortion :public Sprite::SpriteBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̃N���X�̎Q��
          */
         RemainingPortion(Game::GameMain& gameMain);
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
          * \brief �X�v���C�g�̎�ʂ��擾
          * \return ���:�c��񕜃A�C�e����
          */
         virtual SpriteType GetSprType() const { return SpriteType::RemainingPortion; }

      private:
         /**
          * \brief ���݂̎c�񕜃|�[�V���������`�F�b�N����
          * \param stock ���݂̎c�񕜃|�[�V������
          */
         void StockCheck(int stock);
         std::tuple<int, int, int> _xPositions{ 0,0,0 };             //!< �e�|�[�V����X���W
         std::tuple<bool, bool, bool> _stockFlag{ true,true ,true }; //!< �c�|�[�V�������̃t���O
      };
   }
}
