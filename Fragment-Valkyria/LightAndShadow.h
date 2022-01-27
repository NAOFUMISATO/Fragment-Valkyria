#pragma once
#include "AppFrame.h"

namespace FragmentValkyria {
   namespace Game {
      class GameMain;
   }
   namespace Lighting {
      class LightAndShadow {
         using Vector4 = AppFrame::Math::Vector4;
      public:
         LightAndShadow(Game::GameMain& gameMain);

         void Init();
         void Update();
         void Render();

      private:
         Game::GameMain& _gameMain;
         std::pair<int, int> _lightHandles{ -1,-1 };
         std::pair<Vector4, Vector4> _lightPositions{ {0,0,0},{0,0,0} };
         int _shadowHandle{ -1 };
      };
   }

}
