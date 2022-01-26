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
         std::tuple<int, int, int> _lightHandles{ -1,-1,-1 };
         std::tuple<int, int, int> _shadowHandles{ -1,-1,-1 };
         std::tuple<Vector4, Vector4, Vector4> _lightPositions{
            {0,0,0}, {0,0,0}, {0,0,0} };
      };
   }

}
