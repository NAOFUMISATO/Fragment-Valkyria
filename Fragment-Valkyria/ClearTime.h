#pragma once
#include "SpriteBase.h"

namespace FragmentValkyria{
   namespace Clear {
      class ClearTime :public Sprite::SpriteBase{
         using Uint = unsigned int;
      public:
         ClearTime(Game::GameMain& gameMain);

         void Init()override;
         void Input(AppFrame::Input::InputManager& input)override;
         void Update()override;
         void Draw()override;
         virtual SpriteType GetSprType()const { return SpriteType::Sprite; }
      private:
         std::tuple<Uint, Uint, Uint, Uint, Uint> _xPositions{ 0,0,0,0,0 };
         std::tuple<Uint, Uint, Uint, Uint, Uint> _animeNos{ 0,0,0,0,0 };
      };
   }
}
