#pragma once
#include "AppFrame.h"

namespace FragmentValkyria {
   namespace Game {
      class GameMain;
   }
   namespace UI {
      class UIComponent {
      public:
         UIComponent(Game::GameMain& gameMain);
         ~UIComponent();

         void Init();

      };
   }
}
