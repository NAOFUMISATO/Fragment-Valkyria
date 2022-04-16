#pragma once
#include "AppFrame.h"

namespace FragmentValkyria {
   namespace Game {
      class GameMain;
   }
   namespace Param {
      const class ParamLargeEnemyUI :public AppFrame::Param::ParamBase {
      public:
         ParamLargeEnemyUI(Game::GameMain& gameMain, std::string_view key);
      };
   }
}
