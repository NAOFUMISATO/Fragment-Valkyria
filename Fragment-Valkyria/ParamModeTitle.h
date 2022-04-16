#pragma once
#include "AppFrame.h"

namespace FragmentValkyria {
   namespace Game {
      class GameMain;
   }
   namespace Param {
      const class ParamModeTitle :public AppFrame::Param::ParamBase {
      public:
         ParamModeTitle(Game::GameMain& gameMain,std::string_view key);
      };
   }
}
