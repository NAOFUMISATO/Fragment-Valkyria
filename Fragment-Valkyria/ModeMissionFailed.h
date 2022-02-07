#pragma once
#include "ModeBase.h"

namespace FragmentValkyria {

   namespace Mode {
      class ModeMissionFailed :public ModeBase {
      public:
         ModeMissionFailed(Game::GameMain& gameMain);

         void Init()override;
         void Enter()override;

         void Input(AppFrame::Input::InputManager& input)override;

         void Update() override;

         void Render() override;

      private:
         int _grHandle{ -1 };
         
      };
   }
}
