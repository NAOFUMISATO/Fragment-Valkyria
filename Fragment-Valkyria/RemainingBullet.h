#pragma once
#include "SpriteBase.h"

namespace FragmentValkyria {
   namespace Player {
      class RemainingBullet :public Sprite::SpriteBase {
      public:
         RemainingBullet(Game::GameMain& gameMain);

         void Init() override;
         void Update() override;
         void Draw()override;

         virtual SpriteType GetSprType() const { return SpriteType::Sprite; }

      private:
         void StockCheck(int stock);
         std::tuple<int, int, int, int, int> _xPositions{ 0,0,0,0,0 };
         std::tuple<bool, bool, bool, bool, bool> _stockFlag{ 
            true,true ,true ,true ,true 
         };
      };

   }

}
