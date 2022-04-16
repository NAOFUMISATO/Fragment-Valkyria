#pragma once
#include "AppFrame.h"

namespace FragmentValkyria {
   namespace Game {
      class GameMain;
   }
   namespace Param {
      const class ParamCollision :public AppFrame::Param::ParamBase {
      public:
         ParamCollision(Game::GameMain& gameMain, std::string_view key);
      };
   }
}
