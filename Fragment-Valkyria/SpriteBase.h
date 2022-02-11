#pragma once
#include "AppFrame.h"
 /**
  * \brief プロジェクト名
  */
namespace FragmentValkyria {
   // 二重インクルード防止
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
            Sprite = 0,   //!< スプライト
         };
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体の参照
          */
         SpriteBase(Game::GameMain& gameMain);

         virtual SpriteType GetSprType() const = 0;
         /**
          * \brief ゲームクラスの参照を取得
          * \return ゲームクラスの参照
          */
         inline Game::GameMain& gameMain() const { return _gameMain; }

      protected:
         Game::GameMain& _gameMain;  //!< ゲーム本体クラスの参照
      };
   }
}
