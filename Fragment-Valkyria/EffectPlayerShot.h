#pragma once
/*****************************************************************//**
 * \file   EffectPlayerShot.h
 * \brief  プレイヤー射撃エフェクトクラス
 * 
 * \author NAOFUMISATO
 * \date   January 2022
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
       * \class プレイヤー射撃エフェクトクラス
       * \brief 射撃エフェクトの発生
       */
      class EffectPlayerShot :public EffectBase{
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体クラスの参照
          */
         EffectPlayerShot(Game::GameMain& gameMain,std::string_view key);

         /**
          * \brief 初期化処理
          */
         void Init()override;
         /**
          * \brief 更新処理
          */
         void Update()override;
         /**
          * \brief オブジェクトの種別の取得
          * \return プレイヤー射撃エフェクト
          */
         EffectType GetEfcType() const override { return EffectType::PlayerShot; }

      private:
         bool _play{ true };
      };
   }

}
