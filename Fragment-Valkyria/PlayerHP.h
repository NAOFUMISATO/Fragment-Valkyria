#pragma once
#include "SpriteBase.h"

namespace FragmentValkyria {
   namespace Game {
      class GameMain;
   }
   namespace Player {
      using Color = AppFrame::Data::Color;
      using Rect = AppFrame::Data::Rect;
      class PlayerHP :public Sprite::SpriteBase{
      public:
         PlayerHP(Game::GameMain& gameMain);

         void Init()override;
         void Update()override;
         void Draw()override;

      private:
         double _hp{ 100.0 };
         double _hpRange{ 100.0 };
         Color _frontColor{255,255,255};
         Rect _frontBar{0,0,0,0};
         Rect _offSet{0,0,0,0};
      };
   }

}
