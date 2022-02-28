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
       * \class ボスのガトリング攻撃エフェクトクラス
       * \brief ガトリング攻撃エフェクトの発生
       */
      class EffectGatlingMuzzleFlash :public EffectBase {
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体クラスの参照
          * \param key エフェクトハンドルを登録した文字列
          */
         EffectGatlingMuzzleFlash(Game::GameMain& gameMain, std::string_view key);
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
          * \return 種別:ガトリングの発火炎(マズルフラッシュ)
          */
         virtual EffectType GetEfcType() const override { return EffectType::GatlingMuzzleFlash; }
      };
   }

}
