#pragma once
#include "EffectBase.h"
/**
 * \brief プロジェクト名
 */
namespace FragmentValkyria {
   /**
    * \brief エフェクト関係
    */
   namespace Effect {
      /**
       * \class ボスの歩きエフェクトクラス
       * \brief ボスの歩きエフェクトの発生
       */
      class EffectBossFall :public EffectBase {
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体クラスの参照
          * \param key エフェクトハンドルを登録した文字列
          */
         EffectBossFall(Game::GameMain& gameMain, std::string_view key);
         /**
          * \brief 初期化処理
          */
         void Init()override;
         /**
          * \brief 更新処理
          */
         void Update()override;
         /**
          * \brief エフェクトの種別を返す
          * \return 種別:ボスの歩き
          */
         virtual EffectType GetEfcType() const override { return EffectType::BossFall; }
      };
   }
}
