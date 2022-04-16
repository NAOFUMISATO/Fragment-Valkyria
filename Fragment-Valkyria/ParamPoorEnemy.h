#pragma once
#include "AppFrame.h"

namespace FragmentValkyria {
   namespace Game {
      class GameMain;
   }
   namespace Param {
      const class ParamPoorEnemy :public AppFrame::Param::ParamBase {
      public:
         ParamPoorEnemy(Game::GameMain& gameMain, std::string_view key);
      };
   }
}
