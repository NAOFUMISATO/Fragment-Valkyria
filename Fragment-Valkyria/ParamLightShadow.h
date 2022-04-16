#pragma once
#include "AppFrame.h"

namespace FragmentValkyria {
   namespace Game {
      class GameMain;
   }
   namespace Param {
      const class ParamLightShadow :public AppFrame::Param::ParamBase {
      public:
         ParamLightShadow(Game::GameMain& gameMain, std::string_view key);
      };
   }
}
