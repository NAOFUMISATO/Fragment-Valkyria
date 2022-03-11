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

      private:
         int _grHandle{ -1 };
         std::vector<int> _grHandles{ 0 };
      };
   }
}
