#pragma once
/*****************************************************************//**
 * \file   PlayerHP.h
 * \brief  プレイヤーHPの描画クラス
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "SpriteBase.h"
#include "ParamPlayer.h"
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
      public:
         /**
          * \brief コンストラクタ
          */
         PlayerHP();
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
          * \return スプライト種別:プレイヤーHP
          */
         virtual SpriteType GetSprType() const { return SpriteType::PlayerHP; }

      private:
         /**
          * \brief HPバー振動の処理
          * \param count ゲームのフレームカウント
          */
         void BarShake(unsigned int count);

         std::unique_ptr<Param::ParamPlayer> _playerParam;  //!< プレイヤークラスの値管理クラスのポインタ
         double _hp{ 100.0 };                               //!< ボスHP
         double _hpRange{ 100.0 };                          //!< ボス最大HP
         double _rate{ 0.0 };                               //!< 背面バーの増減速度
         double _oldFrontHP{ 0.0 };                         //!< 1フレーム前の前面バーのボスHP
         double _oldBackHP{ 0.0 };                          //!< 1フレーム前の背面バーのボスHP
         bool _rateReset{ true };                           //!< 背面バーの増減速度をリセットするか
         bool _shake{ false };                              //!< HPバーを振動させるか
         unsigned int _shakeCnt{0};                         //!< HPバー振幅フレームの保存
         Color _frontColor{ 255,255,255 };                  //!< 前面バーのカラー
         Rect _frontBar{ 0,0,0,0 };                         //!< 前面バー矩形
         Color _backColor{ 255,255,255 };                   //!< 背面バーのカラー
         Rect _backBar{ 0,0,0,0 };                          //!< 背面バー矩形
         Rect _offSet{ 0,0,0,0 };                           //!< 矩形描画のオフセット
      };
   }
}
