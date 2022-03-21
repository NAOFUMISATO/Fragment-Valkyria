#pragma once
/*****************************************************************//**
 * \file   EffectBossCharge.h
 * \brief  ボスのビームチャージエフェクトクラス
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
       * \class ボスのビームチャージエフェクトクラス
       * \brief ボスのビームチャージエフェクトの描画を行う
       */
      class EffectBossCharge :public EffectBase {
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体クラスの参照
          * \param key ResourceServerに登録した任意の文字列のエフェクトキー
          */
         EffectBossCharge(Game::GameMain& gameMain, std::string_view key);
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
          * \return 種別:ボスのビームチャージ
          */
         virtual EffectType GetEfcType() const override { return EffectType::BossCharge; }
      };
   }
}
