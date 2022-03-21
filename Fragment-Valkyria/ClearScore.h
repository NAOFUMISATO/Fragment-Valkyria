#pragma once
/*****************************************************************//**
 * \file   ClearScore.h
 * \brief  クリアスコア表示クラス
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "SpriteBase.h"
 /**
  * \brief プロジェクト名
  */
namespace FragmentValkyria {
   /**
    * \brief クリア関係
    */
   namespace Clear {
      class ClearScore :public Sprite::SpriteBase {
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体クラスの参照
          */
         ClearScore(Game::GameMain& gameMain);
         /**
          * \brief 初期化処理
          */
         void Init()override;
         /**
          * \brief 描画処理
          */
         void Draw()override;
         /**
          * \brief スプライト種別の取得
          * \return 種別:クリアスコア
          */
         virtual SpriteType GetSprType() const { return SpriteType::ClearScore; }

      };
   }
}
