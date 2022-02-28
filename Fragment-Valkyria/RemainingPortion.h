#pragma once
/*****************************************************************//**
 * \file   RemainingPortion.h
 * \brief  残り回復ポーション描画クラス
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
    * \brief プレイヤー関係
    */
   namespace Player {
      /**
       * \class 残り回復ポーション描画クラス
       * \brief 残り回復ポーション数を描画する
       */
      class RemainingPortion :public Sprite::SpriteBase {
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体クラスの参照
          */
         RemainingPortion(Game::GameMain& gameMain);
         /**
          * \brief 初期化処理
          */
         void Init()override;
         /**
          * \brief 更新処理
          */
         void Update()override;
         /**
          * \brief 描画処理
          */
         void Draw()override;
         /**
          * \brief スプライトの種別を取得
          * \return 種別:残り回復アイテム数
          */
         virtual SpriteType GetSprType() const { return SpriteType::RemainingPortion; }

      private:
         /**
          * \brief 現在の残回復ポーション数をチェックする
          * \param stock 現在の残回復ポーション数
          */
         void StockCheck(int stock);
         std::tuple<int, int, int> _xPositions{ 0,0,0 };             //!< 各ポーションX座標
         std::tuple<bool, bool, bool> _stockFlag{ true,true ,true }; //!< 残ポーション数のフラグ
      };
   }
}
