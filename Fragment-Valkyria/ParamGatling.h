#pragma once
#include "AppFrame.h"

namespace FragmentValkyria {
   namespace Game {
      class GameMain;
   }
   namespace Param {
      const class ParamGatling :public AppFrame::Param::ParamBase {
      public:
         ParamGatling(Game::GameMain& gameMain, std::string_view key);
      };
   }
}
