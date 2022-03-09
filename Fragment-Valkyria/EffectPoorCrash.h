#pragma once
#include "EffectBase.h"

namespace FragmentValkyria {
   namespace Effect {
      class EffectPoorCrash :public EffectBase {
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲームの参照
          * \param key ResourceServerに登録した任意の文字列のエフェクトキー
          */
         EffectPoorCrash(Game::GameMain& gameMain, std::string_view key);
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
          * \return 種別:雑魚敵の破壊
          */
         virtual EffectType GetEfcType() const override { return EffectType::PoorCrash; }
      };
   }
}
