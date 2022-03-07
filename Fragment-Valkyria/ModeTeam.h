#pragma once
#include "ModeBase.h"

namespace FragmentValkyria {
   namespace Mode {
      class ModeTeam :public ModeBase {
      public:
         ModeTeam(Game::GameMain& gameMain);
         void Init()override;
         void Enter()override;
         void Update()override;
         void Render()override;

      private:
         int _grHandle{ -1 };
         int _fadeCnt{ 0 };
         bool _cntInit{ false };
      };
   }
}
