#pragma once
#include "SpriteBase.h"

namespace FragmentValkyria {
   namespace Player {
      class RemainingPortion :public Sprite::SpriteBase {
      public:
         RemainingPortion(Game::GameMain& gameMain);

         void Init()override;
         void Update()override;
         void Draw()override;
         virtual SpriteType GetSprType() const { return SpriteType::Sprite; }

      private:
         void StockCheck(int stock);
         std::tuple<int, int, int> _xPositions{ 0,0,0 };
         std::tuple<bool, bool, bool> _stockFlag{
            true,true ,true 
         };
      };
   }
}
