#pragma once
/*****************************************************************//**
 * \file   EffectObjectHit.h
 * \brief  オブジェクト衝突時のエフェクトクラス
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
       * \class オブジェクト衝突時のエフェクトクラス
       * \brief オブジェクト衝突時のエフェクトの発生
       */
      class EffectObjectHit:public EffectBase {
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体クラスの参照
          * \param key エフェクトハンドルを登録した文字列
          */
         EffectObjectHit(Game::GameMain& gameMain,std::string_view key);
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
          * \return 種別:オブジェクト衝突
          */
         virtual EffectType GetEfcType() const override { return EffectType::ObjectHit; }
      };
   }
}
