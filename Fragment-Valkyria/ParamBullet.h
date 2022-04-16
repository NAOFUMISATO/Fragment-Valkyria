#pragma once
#include "AppFrame.h"

namespace FragmentValkyria {
   namespace Game {
      class GameMain;
   }
   namespace Param {
      const class ParamBullet :public AppFrame::Param::ParamBase {
      public:
         ParamBullet(Game::GameMain& gameMain, std::string_view key);
      };
   }
}
