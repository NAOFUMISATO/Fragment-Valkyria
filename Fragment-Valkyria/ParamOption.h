#pragma once
#include "AppFrame.h"

namespace FragmentValkyria {
   namespace Game {
      class GameMain;
   }
   namespace Param {
      const class ParamOption :public AppFrame::Param::ParamBase {
      public:
         ParamOption(Game::GameMain& gameMain, std::string_view key);
      };
   }
}
