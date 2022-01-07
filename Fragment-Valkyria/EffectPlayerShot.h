#pragma once
#include "EffectBase.h"

namespace FragmentValkyria {
   namespace Effect {
      class EffectPlayerShot :public EffectBase{
      public:
         EffectPlayerShot(Game::GameMain& gameMain);
         ~EffectPlayerShot()override;

         EffectType GetEfcType() const override { return EffectType::PlayerShot; }

         void Init()override;
         void Input(AppFrame::Input::InputManager& input)override;
         void Update()override;
         void Draw()override;

      };
   }

}
