#pragma once
/*****************************************************************//**
 * \file   GameMain.h
 * \brief  ゲーム本体クラス
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "AppFrame.h"
 /**
  * \brief プロジェクト名
  */
namespace FragmentValkyria {
   constexpr auto ScreenWidth = 1920;   //!< 横画面サイズ
   constexpr auto ScreenHeight = 1080;  //!< 縦画面サイズ
   constexpr auto ScreenDepth = 32;     //!< ビット数
   // 二重インクルード防止
   namespace Create {
      class ObjectFactory;
   }
   namespace Stage {
      class LoadStageFromJson;
   }
   /**
    * \brief ゲーム本体
    */
   namespace Game {
      /**
       * \class ゲーム本体クラス
       * \brief ゲームのメイン処理を回す
       */
      class GameMain : public AppFrame::Game::GameBase {
         using GameBase = AppFrame::Game::GameBase;
      public:
         /**
          * \brief 初期化処理
          * \param hInstance WinMainの第一引数
          * \return 初期化に成功したらtrue、失敗したらfalseを返す
          */
         bool Initialize(HINSTANCE hInstance)override;
         /**
          * \brief 実行
          */
         void Run()override;
         /**
          * \brief 停止
          */
         void ShutDown()override;
         /**
          * \brief 入力処理
          */
         void Input()override;
         /**
          * \brief 更新処理
          */
         void Update()override;
         /**
          * \brief 描画処理
          */
         void Render()override;

         /**
          * \brief 計測時間の取得
          * \return インゲームで計測している時間
          */
         inline unsigned int ingameTimer() const { return _ingameTimer; }
         /**
          * \brief 計測時間の設定
          * \param timer 設定する時間
          */
         inline void ingameTimer(unsigned int timer) { _ingameTimer = timer; }
         /**
          * \brief 計測時間を進める
          */
         inline void IngameTimerCount() { _ingameTimer++; }
         /**
          * \brief 画面設定の値を返す
          * \return 画面横サイズ、画面縦サイズ、画面ビット数
          */
         inline virtual std::tuple<int, int, int>GraphSize() { return { ScreenWidth,ScreenHeight,ScreenDepth }; }
         /**
          * \brief オブジェクトの生成一括管理クラスの取得
          * \return オブジェクトの生成一括管理クラスの参照
          */
         Create::ObjectFactory& objFactory() const { return *_objFactory; }
         /**
          * \brief ステージ情報管理クラスの取得
          * \return ステージ情報管理クラスの参照
          */
         Stage::LoadStageFromJson& loadStage() const { return *_loadStage; }

      private:
         unsigned int _ingameTimer{ 0 };
         std::unique_ptr<Create::ObjectFactory> _objFactory;  //!< オブジェクト生成一括管理クラスのユニークポインタ
         std::unique_ptr<Stage::LoadStageFromJson> _loadStage;//!< ステージ情報管理クラスのユニークポインタ
      };
   }
}
