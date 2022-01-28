#pragma once
#include "SpriteBase.h"

namespace FragmentValkyria {
   namespace Game {
      class GameMain;
   }
   namespace Player {
      using Color = AppFrame::Data::Color;
      using Rect = AppFrame::Data::Rect;
      using Triangle = AppFrame::Data::Triangle;
      class PlayerHP :public Sprite::SpriteBase {
      public:
         PlayerHP(Game::GameMain& gameMain);

         void Init()override;
         void Update()override;
         void Draw()override;

         virtual SpriteType GetSprType() const { return SpriteType::UI; }

      private:
         double _hp{ 100.0 };
         double _hpRange{ 100.0 };
         double _rate{ 0.0 };
         double _oldFrontHP{ 0.0 };
         double _oldBackHP{ 0.0 };
         bool _rateReset{ true };
         bool _shake{ false };
         unsigned int _shakeCnt{0};
         Color _frontColor{ 255,255,255 };
         Rect _frontBar{ 0,0,0,0 };
         Triangle _frontTriangle{ {0,0},{0,0},{0,0} };
         Color _backColor{ 255,255,255 };
         Rect _backBar{ 0,0,0,0 };
         Triangle _backTriangle{ {0,0},{0,0},{0,0} };
         Rect _offSet{ 0,0,0,0 };
      };
   }
}
