#pragma once
/*****************************************************************//**
 * \file   EffectPreliminaryLight.h
 * \brief  ボスの予備動作エフェクトクラス
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
      * \class ボスの予備動作エフェクトクラス
      * \brief ボスの予備動作エフェクトの発生
      */
      class EffectPreliminaryLight :public EffectBase {
      public:
         /**
          * \brief コンストラクタ
          * \param key エフェクトハンドルを登録した文字列
          */
         EffectPreliminaryLight( std::string_view key);
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
          * \return 種別:予備動作
          */
         virtual EffectType GetEfcType() const override { return EffectType::PreliminaryLight; }
      };
   }
}
