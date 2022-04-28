#pragma once
/*****************************************************************//**
 * \file   ModeGameOver.h
 * \brief  モードゲームオーバークラス
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "AppFrame.h"
/**
 * \brief プロジェクト名
 */
namespace FragmentValkyria {
   /**
    * \brief モード関係
    */
   namespace Mode {
      /**
       * \class モードゲームオーバークラス
       * \brief ゲームオーバー画面を表示する
       */
      class ModeGameOver:public AppFrame::Mode::ModeBase {
         using InputManager = AppFrame::Input::InputManager;
      public:
         /**
          * \brief コンストラクタ
          */
         ModeGameOver();
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
         void Input(InputManager& input)override;
         /**
          * \brief 更新処理
          */
         void Update() override;
         /**
          * \brief 描画処理
          */
         void Render() override;

      private:
         std::vector<int> _continueDrawHandles;                        //!< 描画用画像ハンドル
         std::vector<int> _exitDrawHandles;                            //!< 描画用画像ハンドル
         std::unordered_map<std::string, std::vector<int>> _handleMap; //!< 画像ハンドルを格納する連想配列
         std::unique_ptr<AppFrame::State::StateServer> _stateServer;   //!< 状態一括管理クラスのポインタ

      public:
         /**
          * \class 状態の基底クラス
          * \brief 各状態はこのクラスを派生して定義する
          */
         class StateBase : public AppFrame::State::StateBaseRoot {
         public:
            /**
             * \brief コンストラクタ
             * \param owner モードタイトルの参照
             */
            StateBase(ModeGameOver& owner) : _owner{ owner } {};
            /**
             * \brief 描画処理
             */
            void Draw() override;

         protected:
            ModeGameOver& _owner;
         };
         /**
          * \class コンテニュー選択状態クラス
          * \brief コンテニュー選択時の処理を行う
          */
         class StateContinue : public StateBase {
         public:
            /**
             * \brief コンストラクタ
             * \param owner ゲームオーバークラスの参照
             */
            StateContinue(ModeGameOver& owner) :StateBase{ owner } {};
            /**
             * \brief 入口処理
             */
            void Enter() override;
            /**
             * \brief 入力処理
             * \param input 入力一括管理クラスの参照
             */
            void Input(InputManager& input) override;
            /**
             * \brief 出口処理
             */
            void Exit()override;
         };
         /**
          * \class タイトルへ戻る選択状態クラス
          * \brief タイトルへ戻る選択時の処理を行う
          */
         class StateExit : public StateBase {
         public:
            /**
             * \brief コンストラクタ
             * \param owner ゲームオーバークラスの参照
             */
            StateExit(ModeGameOver& owner) : StateBase{ owner } {};
            /**
             * \brief 入口処理
             */
            void Enter() override;
            /**
             * \brief 入力処理
             * \param input 入力一括管理クラスの参照
             */
            void Input(InputManager& input) override;
            /**
             * \brief 出口処理
             */
            void Exit() override;
         };
      };
   }
}
