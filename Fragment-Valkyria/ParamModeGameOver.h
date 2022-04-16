#pragma once
#include "AppFrame.h"

namespace FragmentValkyria {
   namespace Game {
      class GameMain;
   }
   namespace Param {
      const class ParamModeGameOver :public AppFrame::Param::ParamBase {
      public:
         ParamModeGameOver(Game::GameMain& gameMain, std::string_view key);
      };
   }
}
