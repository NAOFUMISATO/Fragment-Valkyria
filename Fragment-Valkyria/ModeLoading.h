#pragma once
#include "ModeBase.h"

namespace FragmentValkyria {
   namespace Mode {
      class ModeLoading :public ModeBase {
      public:
         ModeLoading(Game::GameMain& gameMain);
         void Init()override;
         void Update()override;
         void Render()override;
         void Exit()override;

      private:
         int _grHandle{ -1 };
         bool _load{ false };
         std::vector<int> _grHandles{ 0 };
      };
   }
}
