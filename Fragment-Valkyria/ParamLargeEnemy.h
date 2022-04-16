#pragma once
#include "AppFrame.h"

namespace FragmentValkyria {
   namespace Game {
      class GameMain;
   }
   namespace Param {
      const class ParamLargeEnemy :public AppFrame::Param::ParamBase {
      public:
         ParamLargeEnemy(Game::GameMain& gameMain, std::string_view key);
      };
   }
}
