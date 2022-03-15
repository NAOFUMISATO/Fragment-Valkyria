#pragma once
#include "EffectBase.h"

namespace FragmentValkyria {
   namespace Effect {
      class EffectBossCrash :public EffectBase {
         using Vector4 = AppFrame::Math::Vector4;
      public:
         EffectBossCrash(Game::GameMain& gameMain, std::string_view key);
         void Init()override;
         void Update()override;
         /**
          * \brief エフェクトの種類を返す
          * \return 種別:ボスの破壊
          */
         virtual EffectType GetEfcType() const override { return EffectType::BossCrash; }

      private:
         int _efcCnt{ 0 };
         std::vector<Vector4> _bornPositions;
      };
   }
}
