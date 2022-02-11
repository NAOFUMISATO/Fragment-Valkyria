#pragma once
/*****************************************************************//**
 * \file   ClearTime.h
 * \brief  �N���A���ԃN���X
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "SpriteBase.h"
/**
 * \brief �v���W�F�N�g��
 */
namespace FragmentValkyria{
   /**
    * \brief �N���A�֌W
    */
   namespace Clear {
      class ClearTime :public Sprite::SpriteBase{
         using Uint = unsigned int;
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̃N���X�̎Q��
          */
         ClearTime(Game::GameMain& gameMain);
         /**
          * \brief ����������
          */
         void Init()override;
         /**
          * \brief ���͏���
          * \param input ���͈ꊇ�Ǘ��N���X
          */
         void Input(AppFrame::Input::InputManager& input)override;
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
          * \return ���Sprite
          */
         virtual SpriteType GetSprType() const { return SpriteType::Sprite; }

      private:
         std::tuple<Uint, Uint, Uint, Uint, Uint> _xPositions{ 0,0,0,0,0 }; //!< �N���A����1�����Ƃ�X���W
         std::tuple<Uint, Uint, Uint, Uint, Uint> _animeNos{ 0,0,0,0,0 };   //!< �N���A����1�����Ƃ̃A�j���ԍ�
      };
   }
}
