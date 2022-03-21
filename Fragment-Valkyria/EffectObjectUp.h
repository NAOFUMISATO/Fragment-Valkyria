#pragma once
/*****************************************************************//**
 * \file   EffectObjectUp.h
 * \brief  オブジェクト上昇時のエフェクトクラス
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
       * \class オブジェクト上昇時のエフェクトクラス
       * \brief オブジェクト上昇時のエフェクトの発生
       */
      class EffectObjectUp :public EffectBase {
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体クラスの参照
          * \param key ResourceServerに登録した任意の文字列のエフェクトキー
          */
         EffectObjectUp(Game::GameMain& gameMain, std::string_view key);
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
          * \return 種別:オブジェクト上昇
          */
         virtual EffectType GetEfcType() const override { return EffectType::ObjectUp; }
      };
   }
}
