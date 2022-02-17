#pragma once
#include "SpriteBase.h"

namespace FragmentValkyria {
   namespace Enemy {
      class LargeEnemyHP :public Sprite::SpriteBase {
         using Color = AppFrame::Data::Color;
         using Rect = AppFrame::Data::Rect;
      public:
         LargeEnemyHP(Game::GameMain& gameMain);
         void Init()override;
         void Update()override;
         void Draw()override;
         virtual SpriteType GetSprType() const { return SpriteType::Sprite; }

         private:
            double _hp{ 1000.0 };
            double _hpRange{ 10000.0 };
            double _rate{ 0.0 };              //!< 背面バーの増減速度
            double _oldFrontHP{ 0.0 };        //!< 1フレーム前の前面バーのプレイヤーHP
            double _oldBackHP{ 0.0 };         //!< 1フレーム前の背面バーのプレイヤーHP
            bool _rateReset{ true };          //!< 背面バーの増減速度をリセットするか
            bool _shake{ false };             //!< HPバーを振動させるか
            Color _frontColor{ 255,255,255 }; //!< 前面バーのカラー
            Rect _frontBar{ 0,0,0,0 };        //!< 前面バー矩形
            Color _backColor{ 255,255,255 };  //!< 背面バーのカラー
            Rect _backBar{ 0,0,0,0 };         //!< 背面バー矩形
            Rect _offSet{ 0,0,0,0 };          //!< 矩形描画のオフセット
      };
   }
}
