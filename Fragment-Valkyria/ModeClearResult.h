#pragma once
/*****************************************************************//**
 * \file   ModeClearResult.h
 * \brief  モードクリア画面クラス
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "ModeBase.h"
/**
 * \brief プロジェクト名
 */
namespace FragmentValkyria {
   /**
    * \brief モード関係
    */
   namespace Mode {
      /**
       * \class モードクリア画面クラス
       * \brief クリア画面及びクリアタイムの表示を行う
       */
      class ModeClearResult : public ModeBase {
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体クラスの参照を取得
          */
         ModeClearResult(Game::GameMain& gameMain);
         /**
          * \brief 初期化処理
          */
         void Init()override;
         /**
          * \brief 入口処理
          */
         void Enter()override;
         /**
          * \brief 入力処理
          * \param input 入力一括管理クラスの参照
          */
         void Input(AppFrame::Input::InputManager& input) override;
         /**
          * \brief 更新処理
          */
         void Update()override;
         /**
          * \brief 描画処理
          */
         void Render()override;
         /**
          * \brief 出口処理
          */
         void Exit()override;

      private:
         bool _born{ true };  //!< クリアタイムクラスのインスタンス生成フラグ
         int _bgHandle{ -1 }; //!< 背景画像ハンドル
      };
   }
}
