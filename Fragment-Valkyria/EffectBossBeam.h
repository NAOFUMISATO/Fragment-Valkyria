#pragma once
#include "EffectBase.h"

namespace FragmentValkyria {
   namespace Effect {
      class EffectBossBeam :public EffectBase {
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体クラスの参照
          * \param key ResourceServerに登録した任意の文字列のエフェクトキー
          */
         EffectBossBeam(Game::GameMain& gameMain, std::string_view key);
         /**
          * \brief 初期化処理
          */
         void Init()override;
         /**
          * \brief 更新処理
          */
         void Update()override;
         /**
          * \brief エフェクトの種類を返す
          * \return 種別:ガトリング弾
          */
         virtual EffectType GetEfcType() const override { return EffectType::GatlingBullet; }

      };
   }
}
