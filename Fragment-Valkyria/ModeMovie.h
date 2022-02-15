#pragma once
#include "ModeBase.h"

namespace FragmentValkyria {
   namespace Mode {
      class ModeMovie : public ModeBase {
      public:
         ModeMovie(Game::GameMain& gameMain);
         void Init()override;
         void Enter()override;
         void Input(AppFrame::Input::InputManager& input)override;
         void Update()override;
         void Render()override;
      
      private:
         int _mvHandle{ -1 };
      };
   }
}