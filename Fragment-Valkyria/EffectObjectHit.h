#pragma once
#include "EffectBase.h"

namespace FragmentValkyria {
   namespace Effect {
      class EffectObjectHit:public EffectBase {
      public:
         EffectObjectHit(Game::GameMain& gameMain,std::string_view key);
         void Init()override;
         void Update()override;
         virtual EffectType GetEfcType() const override { return EffectType::ObjectHit; }
      };
   }
}
