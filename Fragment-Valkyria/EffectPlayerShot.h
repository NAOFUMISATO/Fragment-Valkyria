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
         EffectPlayerShot(Game::GameMain& gameMain);
         /**
          * \brief デストラクタ
          */
         ~EffectPlayerShot()override;

         /**
          * \brief 初期化及び_loadHandleにエフェクトハンドルを登録する
          * \param key ResourceServerに登録済みの任意の文字列
          */
         void Init(std::string_view key)override;
         /**
          * \brief 入力処理
          * \param input 入力一括管理クラスの参照
          */
         void Input(AppFrame::Input::InputManager& input)override {};
         /**
          * \brief 更新処理
          */
         void Update()override;
         /**
          * \brief 描画処理
          */
         void Draw()override;
         /**
          * \brief オブジェクトの種別の取得
          * \return プレイヤー射撃エフェクト
          */
         EffectType GetEfcType() const override { return EffectType::PlayerShot; }
      };
   }

}
