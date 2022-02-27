#pragma once
/*****************************************************************//**
 * \file   LargeEnemyHP.h
 * \brief  ボス体力バー描画クラス
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
    * \brief 敵関係
    */
   namespace Enemy {
      /**
       * \class ボス体力バー描画クラス
       * \brief ボス体力バーの描画を行う
       */
      class LargeEnemyHP :public Sprite::SpriteBase {
         using Color = AppFrame::Data::Color;
         using Rect = AppFrame::Data::Rect;
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体クラスの参照
          */
         LargeEnemyHP(Game::GameMain& gameMain);
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
          * \brief スプライト種別の取得
          * \return 種別:スプライト
          */
         virtual SpriteType GetSprType() const { return SpriteType::Sprite; }

         private:
            double _hp{ 0.0 };                //!< 体力
            double _rate{ 0.0 };              //!< 背面バーの増減速度
            double _oldFrontHP{ 0.0 };        //!< 1フレーム前の前面バーのプレイヤーHP
            double _oldBackHP{ 0.0 };         //!< 1フレーム前の背面バーのプレイヤーHP
            bool _rateReset{ true };          //!< 背面バーの増減速度をリセットするか
            bool _shake{ false };             //!< HPバーを振動させるか
            Rect _frontBar{ 0,0,0,0 };        //!< 前面バー矩形
            Rect _backBar{ 0,0,0,0 };         //!< 背面バー矩形
            Rect _offSet{ 0,0,0,0 };          //!< 矩形描画のオフセット
            Color _backColor{ 255,255,255 };  //!< 背面バーのカラー
            Color _frontColor{ 255,255,255 }; //!< 前面バーのカラー
      };
   }
}
