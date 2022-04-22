#pragma once
/*****************************************************************//**
 * \file   EffectWeakBullet.h
 * \brief  プレイヤー弱攻撃エフェクトクラス
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
       * \class プレイヤー弱攻撃エフェクトクラス
       * \brief プレイヤー弱攻撃エフェクトの発生
       */
      class EffectWeakBullet :public EffectBase {
      public:
         /**
          * \brief コンストラクタ
          * \param key エフェクトハンドルを登録した文字列
          */
         EffectWeakBullet( std::string_view key);
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
          * \return 種別:プレイヤー弱攻撃
          */
         virtual EffectType GetEfcType() const override { return EffectType::WeakBullet; }
      };
   }
}
