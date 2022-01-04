#pragma once
/*****************************************************************//**
 * \file   ModeInGame.h
 * \brief  モードインゲームクラス
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "AppFrame.h"
#include "GameMain.h"
 /**
  * \brief プロジェクト名
  */
namespace FragmentValkyria {
   // 二重インクルード防止
   namespace Object {
      class ObjectServer;
   }
   namespace Create {
      class ObjectFactory;
   }
   /**
    * \brief モード関係
    */
   namespace Mode {
      /**
       * \class モードインゲームクラス
       * \brief インゲームの処理を回す
       */
      class ModeInGame : public AppFrame::Mode::ModeBase {
      public:
         /**
          * \brief コンストラクタ
          * \param game ゲーム本体の参照
          */
         ModeInGame(Game::GameMain& game);
         /**
          * \brief デフォルトデストラクタ
          */
         ~ModeInGame() = default;
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
         /**
          * \brief 出口処理
          */
         void Exit() override;

         /**
          * \brief オブジェクト一括管理クラスの参照をゲームメイン経由で取得
          * \return オブジェクト一括管理クラスの参照
          */
         Object::ObjectServer& objServer() const { return _gameMain.objServer(); }
         /**
          * \brief オブジェクト生成一括管理クラスの参照をゲームメイン経由で取得
          * \return オブジェクト生成一括管理クラスの参照
          */
         Create::ObjectFactory& objFactory() const { return _gameMain.objFactory(); }

      private:
         Game::GameMain& _gameMain;  //!< ゲーム本体クラスの参照

#ifdef _DEBUG
         short _padLeftX{ 0 };
         short _padLeftY{ 0 };
#endif
      };
   }
}