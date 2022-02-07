#pragma once
#include "ModeBase.h"

namespace FragmentValkyria {
   namespace Mode {
      class ModePause :public ModeBase {
      public:
         ModePause(Game::GameMain& gameMain);

         void Init()override;
         void Enter() override;
         void Input(AppFrame::Input::InputManager& input)override;

         void Update() override;

         void Render() override;

      private:
         int _grHandle{ -1 };
         AppFrame::Math::Vector4 _pausePos{ 0,0,0 };
      };
   }
}
