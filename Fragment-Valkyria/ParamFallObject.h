#pragma once
#include "AppFrame.h"

namespace FragmentValkyria {
   namespace Game {
      class GameMain;
   }
   namespace Param {
      const class ParamFallObject :public AppFrame::Param::ParamBase {
      public:
         ParamFallObject(Game::GameMain& gameMain, std::string_view key);
      };
   }
}
