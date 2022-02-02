#pragma once
#include "ModeBase.h"

namespace FragmentValkyria {

   namespace Mode {
      class ModeClearResult : public ModeBase {
      public:
         ModeClearResult(Game::GameMain& gameMain);

         void Init()override;
         void Enter()override;
         void Input(AppFrame::Input::InputManager& input) override;
         void Update()override;
         void Render()override;
         void Exit()override;

      private:
         bool _born{ true };
         int _bgHandle{ -1 };
      };
   }
}
