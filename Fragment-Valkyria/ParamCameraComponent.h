#pragma once
#include "AppFrame.h"

namespace FragmentValkyria {
   namespace Game {
      class GameMain;
   }
   namespace Param {
      const class ParamCameraComponent :public AppFrame::Param::ParamBase {
      public:
         ParamCameraComponent(Game::GameMain& gameMain, std::string_view key);
      };
   }
}
