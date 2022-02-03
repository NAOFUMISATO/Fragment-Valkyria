#pragma once
#include "ModeBase.h"

namespace FragmentValkyria {

   namespace Mode {
      class ModeMissionCompleted :public ModeBase {
      public:
         ModeMissionCompleted(Game::GameMain& gameMain);

         void Init()override;
         void Enter()override;

         void Input(AppFrame::Input::InputManager& input)override;

         void Update() override;

         void Render() override;

      };
   }
}
