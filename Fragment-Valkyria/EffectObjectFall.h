#pragma once
/*****************************************************************//**
 * \file   EffectObjectFall.h
 * \brief  オブジェクト落下エフェクトクラス
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
       * \class オブジェクト落下エフェクトクラス
       * \brief 落下エフェクトの発生
       */
      class EffectObjectFall :public EffectBase {
      public:
         /**
          * \brief コンストラクタ
          * \param key エフェクトハンドルを登録した文字列
          */
         EffectObjectFall(std::string_view key);
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
          * \return 種別:オブジェクト落下
          */
         virtual EffectType GetEfcType() const override { return EffectType::ObjectFall; }
      };
   }
}
