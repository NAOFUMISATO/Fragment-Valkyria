#pragma once
/*****************************************************************//**
 * \file   EffectBossBeam.h
 * \brief  ボスのビームエフェクトクラス
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
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
       * \class ボスのビームエフェクトクラス
       * \brief ボスのビームエフェクトを再生する
       */
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
          * \return 種別:ボスビーム攻撃
          */
         virtual EffectType GetEfcType() const override { return EffectType::BossBeam; }
      };
   }
}
