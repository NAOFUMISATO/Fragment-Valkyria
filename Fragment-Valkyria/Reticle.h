#pragma once
#include "SpriteBase.h"

namespace FragmentValkyria {
   namespace Player {
      class Reticle :public Sprite::SpriteBase {
      public:
         Reticle(Game::GameMain& gameMain);
         void Init()override;
         void Update()override;
         void Draw()override;
         virtual SpriteType GetSprType()const { return SpriteType::Sprite; }
      private:
         bool _isAim{ false };
      };
   }
}
