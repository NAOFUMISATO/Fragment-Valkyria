#pragma once
#include "AppFrame.h"

namespace FragmentValkyria {
   namespace Game {
      class GameMain;
   }
   namespace Param {
      const class ParamPlayerUI :public AppFrame::Param::ParamBase {
      public:
         ParamPlayerUI(Game::GameMain& gameMain, std::string_view key);
      };
   }
}
