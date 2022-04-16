#pragma once
#include "AppFrame.h"

namespace FragmentValkyria {
   namespace Game {
      class GameMain;
   }
   namespace Param {
      const class ParamModeClear :public AppFrame::Param::ParamBase {
      public:
         ParamModeClear(Game::GameMain& gameMain, std::string_view key);
      };
   }
}
