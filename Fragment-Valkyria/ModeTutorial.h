#pragma once
/*****************************************************************//**
 * \file   ModeTutorial.h
 * \brief  モードチュートリアルクラス
 * 
 * \author NAOFUMISATO
 * \date   March 2022
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
       * \class モードチュートリアルクラス
       * \brief チュートリアルを行う
       */
      class ModeTutorial :public ModeInGameBase {
         using InputManager = AppFrame::Input::InputManager;
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体クラスの参照
          */
         ModeTutorial(Game::GameMain& gameMain);
         /**
         * \brief 初期化処理
         */
         void Init() override;
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
          * \brief 生存状態のチュートリアルTIPSはあるか
          * \return あればtrue,無ければfalse
          */
         bool TipsAlive();
         /**
          * \brief チュートリアルをクリアしていれば処理を行う
          * \param key チュートリアルに関連付けた文字列
          */
         void ClearJudge(std::string_view key);
         /**
          * \brief チュートリアルTIPSの生成を行う
          * \param key 生成するチュートリアルに関連付けた文字列
          */
         void TipsBorn(std::string_view key);
         /**
          * \brief 落下オブジェクトがないなら再度出現させる
          */
         void FallObjectRespawn();
         int _tutorialProgress{ 1 }; //!< チュートリアルの進捗
         bool _born{ true };         //!< TIPS生成フラグ
         bool _isAppear{ false };    //!< TIPS出現フラグ
      };
   }
}
