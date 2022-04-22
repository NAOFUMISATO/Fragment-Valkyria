#pragma once
/*****************************************************************//**
 * \file   EffectPoorCrash.h
 * \brief  雑魚敵の破壊エフェクトクラス
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
       * \class 雑魚敵の破壊エフェクトクラス
       * \brief 雑魚敵の破壊エフェクトの発生
       */
      class EffectPoorCrash :public EffectBase {
      public:
         /**
          * \brief コンストラクタ
          * \param key ResourceServerに登録した任意の文字列のエフェクトキー
          */
         EffectPoorCrash(std::string_view key);
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
