#pragma once
/*****************************************************************//**
 * \file   RemainingBullet.h
 * \brief  プレイヤーの残弾表示クラス
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "SpriteBase.h"
/**
 * \brief プロジェクト名
 */
namespace FragmentValkyria {
   /**
    * \brief プレイヤー
    */
   namespace Player { 
      /**
       * class プレイヤーの残弾表示クラス
       * \brief プレイヤーの弱攻撃の残弾を表示する
       */
      class RemainingBullet :public Sprite::SpriteBase {
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体クラスの参照
          */
         RemainingBullet(Game::GameMain& gameMain);
         /**
          * \brief 初期化処理
          */
         void Init() override;
         /**
          * \brief 更新処理
          */
         void Update() override;
         /**
          * \brief 描画処理
          */
         void Draw()override;
         /**
          * \brief スプライト種別の取得
          * \return 種別:残り弱攻撃弾数
          */
         virtual SpriteType GetSprType() const { return SpriteType::RemainingBullet; }

      private:
         /**
          * \brief 1発毎装填されているかを確認する
          * \param stock 現在の弾数
          */
         void StockCheck(int stock);
         std::tuple<int, int, int, int, int> _xPositions{ 0,0,0,0,0 };  //!< 表示している1発毎のＸ座標
         std::tuple<bool, bool, bool, bool, bool> _stockFlag{           //!< 1発毎装填されているかのフラグ
            true,true ,true ,true ,true 
         };
      };

   }

}
