#pragma once
#include "EffectBase.h"

namespace FragmentValkyria {
   namespace Effect {
      class EffectPlayerShot :public EffectBase{
      public:
         EffectPlayerShot();
         ~EffectPlayerShot();

         EffectType GetEfcType() const override { return EffectType::PlayerShot; }

         void Init()override;
         void Input(AppFrame::Input::InputManager& input)override;
         void Update()override;
         void Draw()override;

      };
   }

}
