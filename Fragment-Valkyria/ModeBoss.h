#pragma once
/*****************************************************************//**
 * \file   ModeBoss.h
 * \brief  モードボスクラス
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "ModeInGameBase.h"
 /**
  * \brief プロジェクト名
  */
namespace FragmentValkyria {
   /**
    * \brief モード関係
    */
   namespace Mode {
      /**
       * \class モードボスクラス
       * \brief インゲームの処理を回す
       */
      class ModeBoss : public ModeInGameBase {
      public:
         /**
          * \brief コンストラクタ
          * \param game ゲーム本体の参照
          */
         ModeBoss(Game::GameMain& gameMain);
         /**
          * \brief 初期化処理
          */
         void Init()override {};
         /**
          * \brief 入口処理
          */
         void Enter() override;
         /**
          * \brief 入力処理
          * \param input 入力一括管理クラスの参照
          */
         void Input(AppFrame::Input::InputManager& input) override;
         /**
          * \brief 更新処理
          */
         void Update() override;
         /**
          * \brief 描画処理
          */
         void Render() override;

      private:
         /**
          * \brief ライティングによる演出を行う
          */
         void LightStaging();
         bool _cntInit{ false }; //!< フレームカウント初期化フラグ
         bool _plus{ true };     //!< ライティングの赤要素を上昇させるかのフラグ
         int _lightOnCount{0};   //!< 何度点滅させるか
         int _lightCnt{ 0 };     //!< フレームカウント保存用
         float _red{0.1f};       //!< 赤
         float green{ 0 };       //!< 緑
         float blue{ 0 };        //!< 青
      };
   }
}