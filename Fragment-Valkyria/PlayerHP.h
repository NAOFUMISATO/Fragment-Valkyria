#pragma once
/*****************************************************************//**
 * \file   PlayerHP.h
 * \brief  プレイヤーHPの描画クラス
 * 
 * \author NAOFUMISATO
 * \date   January 2022
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
       * \class プレイヤーHPの描画クラス
       * \brief プレイヤーHPのバーの増減等を管理する
       */
      class PlayerHP :public Sprite::SpriteBase {
         using Color = AppFrame::Data::Color;
         using Rect = AppFrame::Data::Rect;
         using Triangle = AppFrame::Data::Triangle;
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体クラスの参照
          */
         PlayerHP(Game::GameMain& gameMain);
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
          * \brief スプライト種別:UIの取得
          * \return スプライト種別:UI
          */
         virtual SpriteType GetSprType() const { return SpriteType::UI; }

      private:
         /**
          * \brief HPバー振動の処理
          */
         void BarShake(unsigned int count);
         double _hp{ 100.0 };                          //!< プレイヤーHP
         double _hpRange{ 100.0 };                     //!< プレイヤー最大HP
         double _rate{ 0.0 };                          //!< 背面バーの増減速度
         double _oldFrontHP{ 0.0 };                    //!< 1フレーム前の前面バーのプレイヤーHP
         double _oldBackHP{ 0.0 };                     //!< 1フレーム前の背面バーのプレイヤーHP
         bool _rateReset{ true };                      //!< 背面バーの増減速度をリセットするか
         bool _shake{ false };                         //!< HPバーを振動させるか
         unsigned int _shakeCnt{0};                    //!< HPバー振幅フレームの保存
         Color _frontColor{ 255,255,255 };             //!< 前面バーのカラー
         Rect _frontBar{ 0,0,0,0 };                    //!< 前面バー矩形
         Triangle _frontTriangle{ {0,0},{0,0},{0,0} }; //!< 前面バー三角形
         Color _backColor{ 255,255,255 };              //!< 背面バーのカラー
         Rect _backBar{ 0,0,0,0 };                     //!< 背面バー矩形
         Triangle _backTriangle{ {0,0},{0,0},{0,0} };  //!< 背面バー三角形
         Rect _offSet{ 0,0,0,0 };                      //!< 矩形描画のオフセット
      };
   }
}
