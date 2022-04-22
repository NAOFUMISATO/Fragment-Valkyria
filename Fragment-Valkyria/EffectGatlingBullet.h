#pragma once
/*****************************************************************//**
 * \file   EffectGatlingBullet.h
 * \brief  敵のガトリング弾エフェクトクラス
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
       * \class ボスのガトリング攻撃エフェクトクラス
       * \brief ガトリング攻撃エフェクトの発生
       */
      class EffectGatlingBullet :public EffectBase {
      public:
         /**
          * \brief コンストラクタ
          * \param key エフェクトハンドルを登録した文字列
          */
         EffectGatlingBullet(std::string_view key);
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
          * \return 種別:ガトリング弾
          */
         virtual EffectType GetEfcType() const override { return EffectType::GatlingBullet; }
      };
   }

}
