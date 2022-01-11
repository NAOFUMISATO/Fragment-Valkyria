#pragma once
/*****************************************************************//**
 * \file   EffectBase.h
 * \brief  エフェクトの基底クラス
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "AppFrame.h"
/**
 * \brief プロジェクト名
 */
namespace FragmentValkyria {
   //二重インクルード防止
   namespace Game {
      class GameMain;
   }
   /**
    * \brief エフェクト関係
    */
   namespace Effect {
      /**
       * \class エフェクトの基底クラス
       * \brief 各エフェクトはこのクラスを派生して定義する
       */
      class EffectBase :public AppFrame::Effect::EffectBaseRoot {
      public:
         /**
          * \brief エフェクトの種別
          */
         enum class EffectType {
            Effect=0,   //!< エフェクト
            PlayerShot  //!< プレイヤー射撃
         };
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体クラスの参照
          */
         EffectBase(Game::GameMain& gameMain);
         /**
          * \brief デストラクタ
          */
         virtual ~EffectBase() override ;

         /**
          * \brief 初期化及び_loadHandleにエフェクトハンドルを登録する
          * \param key ResourceServerに登録済みの任意の文字列
          */
         void Init(std::string_view key) override ;
         /**
          * \brief 入力処理
          * \param input 入力一括管理クラスの参照
          */
         void Input(AppFrame::Input::InputManager& input) override {};
         /**
          * \brief 更新処理
          */
         void Update() override {};
         /**
          * \brief 描画処理
          */
         void Draw() override ;
         /**
          * \brief エフェクトの種別の取得
          * \return 派生先で定義
          */
         virtual EffectType GetEfcType() const = 0;

      protected:
         Game::GameMain& _gameMain;   //!< ゲーム本体クラスの参照
      };
}
}
