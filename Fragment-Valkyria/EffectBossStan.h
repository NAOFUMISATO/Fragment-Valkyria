#pragma once
/*****************************************************************//**
 * \file   EffectBossStan.h
 * \brief  ボスのスタンエフェクトクラス
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
       * \class ボスのスタンエフェクトクラス
       * \brief ボスのスタンエフェクトの発生
       */
      class EffectBossStan :public EffectBase {
      public:
         /**
          * \brief コンストラクタ
          * \param key エフェクトハンドルを登録した文字列
          */
         EffectBossStan(std::string_view key);
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
          * \return 種別:ボスのスタン
          */
         virtual EffectType GetEfcType() const override { return EffectType::BossStan; }
      };
   }
}
