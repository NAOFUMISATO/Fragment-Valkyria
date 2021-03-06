#pragma once
/*****************************************************************//**
 * \file   EffectBossFall.h
 * \brief  ボスの落下エフェクトクラス
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
       * \class ボスの落下エフェクトクラス
       * \brief ボスの落下エフェクトの発生
       */
      class EffectBossFall :public EffectBase {
      public:
         /**
          * \brief コンストラクタ
          * \param key エフェクトハンドルを登録した文字列
          */
         EffectBossFall( std::string_view key);
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
          * \return 種別:ボスの落下
          */
         virtual EffectType GetEfcType() const override { return EffectType::BossFall; }
      };
   }
}
