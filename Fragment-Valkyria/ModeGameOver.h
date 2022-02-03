#pragma once
#include "ModeBase.h"

namespace FragmentValkyria {
   namespace Mode {
      class ModeGameOver:public ModeBase {
      public:
         ModeGameOver(Game::GameMain& gameMain);

         void Init()override;

         void Enter()override;
        
         void Input(AppFrame::Input::InputManager& input)override;
        
         void Update() override;
        
         void Render() override;

         void Exit()override;

      private:
         int _grHandle{ -1 };
      };
   }
}
