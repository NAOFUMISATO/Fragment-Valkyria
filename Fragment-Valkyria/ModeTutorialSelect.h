#pragma once
/*****************************************************************//**
 * \file   ModeTutorialSelect.h
 * \brief  モードチュートリアル選択クラス
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "ModeBase.h"
/**
 * \brief プロジェクト名
 */
namespace FragmentValkyria {
   // 二重インクルード防止
   namespace Game {
      class GameMain;
   }
   /**
    * \brief モード関係
    */
   namespace Mode {
      /**
       * \class モードチュートリアル選択クラス
       * \brief チュートリアルを行うかどうか選択する
       */
      class ModeTutorialSelect :public ModeBase {
         using InputManager = AppFrame::Input::InputManager;
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体クラスの参照
          */
         ModeTutorialSelect(Game::GameMain& gameMain);
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
         void Input(AppFrame::Input::InputManager& input)override;
         /**
          * \brief 更新処理
          */
         void Update() override;
         /**
          * \brief 描画処理
          */
         void Render() override;

      private:
         int _cusorX{ 0 };                                            //!< カーソルX位置
         int _cusorY{ 0 };                                            //!< カーソルY位置
         std::unique_ptr<AppFrame::State::StateServer> _stateServer;  //!< 状態一括管理クラスのポインタ
         std::unordered_map<std::string, int> _handleMap;             //!< 画像ハンドルを保存する連想配列

      public:
         /**
          * \class モードチュートリアルの状態基底クラス
          * \brief 各状態はこのクラスを派生して定義する
          */
         class StateBase : public AppFrame::State::StateBaseRoot {
         public:
            /**
             * \brief コンストラクタ
             * \param owner モードチュートリアルの参照
             */
            StateBase(ModeTutorialSelect& owner) : _owner{ owner } {};
            /**
             * \brief 描画処理
             */
            void Draw() override;

         protected:
            ModeTutorialSelect& _owner;   //!< モードチュートリアルの参照
         };

         class StateYes : public StateBase {
         public:
            /**
             * \brief コンストラクタ
             * \param owner モードチュートリアルの参照
             */
            StateYes(ModeTutorialSelect& owner) :StateBase{ owner } {};
            /**
             * \brief 入口処理
             */
            void Enter() override;
            /**
             * \brief 入力処理
             * \param input 入力一括管理クラスの参照
             */
            void Input(InputManager& input) override;
         };

         class StateNo : public StateBase {
         public:
            /**
             * \brief コンストラクタ
             * \param owner モードチュートリアルの参照
             */
            StateNo(ModeTutorialSelect& owner) : StateBase{ owner } {};
            /**
             * \brief 入口処理
             */
            void Enter() override;
            /**
             * \brief 入力処理
             * \param input 入力一括管理クラスの参照
             */
            void Input(InputManager& input) override;
         };
      };
   }
}
