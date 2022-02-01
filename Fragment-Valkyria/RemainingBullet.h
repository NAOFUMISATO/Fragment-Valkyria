#pragma once
#include "SpriteBase.h"

namespace FragmentValkyria {
   namespace Player {
      class RemainingBullet :public Sprite::SpriteBase {
         using StockFlag = std::tuple<bool, bool, bool, bool, bool>;
         using BulletPosX = std::tuple<int, int, int, int, int>;
      public:
         RemainingBullet(Game::GameMain& gameMain);

         void Init() override;
         void Update() override;
         void Draw()override;

         virtual SpriteType GetSprType() const { return SpriteType::UI; }

      private:
         void StockCheck(int stock);
         StockFlag _stockFlag{ true,true ,true ,true ,true };
         BulletPosX _bulletPosX{ 0,0,0,0,0 };
      };

   }

}
