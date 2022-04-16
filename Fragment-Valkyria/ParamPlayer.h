#pragma once
#include "AppFrame.h"

namespace FragmentValkyria {
   namespace Game {
      class GameMain;
   }
   namespace Param {
      const class ParamPlayer :public AppFrame::Param::ParamBase {
      public:
         ParamPlayer(Game::GameMain& gameMain, std::string_view key);
      };
   }
}
