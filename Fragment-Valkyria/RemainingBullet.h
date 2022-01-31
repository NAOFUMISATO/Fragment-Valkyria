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
      };

   }

}
