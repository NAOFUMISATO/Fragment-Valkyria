#pragma once
#include "AppFrame.h"
 /**
  * \brief �v���W�F�N�g��
  */
namespace FragmentValkyria {
   // ��d�C���N���[�h�h�~
   namespace Game {
      class GameMain;
   }
   
   namespace Sprite {
      class SpriteBase :public AppFrame::Sprite::SpriteBaseRoot {
         using Vector4 = AppFrame::Math::Vector4;
         using Matrix44 = AppFrame::Math::Matrix44;
         using StateServer = AppFrame::State::StateServer;
      public:
         
         enum class SpriteType {
            Sprite = 0,   //!< �X�v���C�g
         };
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̂̎Q��
          */
         SpriteBase(Game::GameMain& gameMain);

         virtual SpriteType GetSprType() const = 0;
         /**
          * \brief �Q�[���N���X�̎Q�Ƃ��擾
          * \return �Q�[���N���X�̎Q��
          */
         inline Game::GameMain& gameMain() const { return _gameMain; }

      protected:
         Game::GameMain& _gameMain;  //!< �Q�[���{�̃N���X�̎Q��
      };
   }
}
