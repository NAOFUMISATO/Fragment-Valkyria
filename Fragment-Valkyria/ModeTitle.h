#pragma once
/*****************************************************************//**
 * \file   ModeTitle.h
 * \brief  モードタイトルクラス
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "AppFrame.h"
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
       * \class モードタイトルクラス
       * \brief ゲームのタイトル処理を回す
       */
      class ModeTitle : public ModeBase {
         using InputManager = AppFrame::Input::InputManager;
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体の参照
          */
         ModeTitle(Game::GameMain& gameMain);
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
         void LogoAnimation();
         std::unique_ptr<AppFrame::State::StateServer> _stateServer;  //!< 状態一括管理クラスのポインタ
         int _logoCnt{ 0 };                                           //!< タイトルロゴのアニメーションカウント
         int _firstInputCnt{ 0 };                                     //!< プレスエニイボタンから移行した後、連続入力によるゲーム開始を防ぐカウント
         int _logoHandle{ -1 };                                       //!< タイトルロゴの描画ハンドル
         bool _cntInit{ false };                                      //!< カウント初期化フラグ
         bool _playSound{ false };
         bool _pushAnyButton{ false };                                //!< タイトルのプレスエニイボタンの押下フラグ
         std::vector<int> _startDrawHandles{ 0 };                     //!< ゲーム開始選択の描画用ハンドル
         std::vector<int> _optionDrawHandles{ 0 };                    //!< オプション選択の描画用ハンドル
         std::vector<int> _endDrawHandles{ 0 };                       //!< ゲーム終了選択の描画用ハンドル
         std::unordered_map<std::string, std::vector<int>> _handleMap;//!< 画像ハンドルを保存する連想配列

      public:
         /**
          * \class モードタイトルの状態基底クラス
          * \brief 各状態はこのクラスを派生して定義する
          */
         class StateBase : public AppFrame::State::StateBaseRoot {
         public:
            /**
             * \brief コンストラクタ
             * \param owner モードタイトルの参照
             */
            StateBase(ModeTitle& owner) : _owner{ owner } {};
            /**
             * \brief 描画処理
             */
            void Draw() override;
         protected:
            ModeTitle& _owner;   //!< モードタイトルの参照
         };

         /**
          * \class タイトルAnyBotton選択状態クラス
          * \brief タイトルAnyBottonを表示する
          */
         class StateAnyButton : public StateBase{
         public:
            /**
             * \brief コンストラクタ
             * \param owner モードタイトルの参照
             */
            StateAnyButton(ModeTitle& owner) :StateBase{ owner } {};
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
          * \class ゲームスタート選択状態クラス
          * \brief ゲームを開始する
          */
         class StateStartSelect : public StateBase{
         public:
            /**
             * \brief コンストラクタ
             * \param owner モードタイトルの参照
             */
            StateStartSelect(ModeTitle& owner) : StateBase{ owner } {};
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

         /**
          * \class ゲームオプション選択状態クラス
          * \brief ゲームオプションを表示する
          */
         class StateOptionSelect : public StateBase {
         public:
            /**
             * \brief コンストラクタ
             * \param owner モードタイトルの参照
             */
            StateOptionSelect(ModeTitle& owner) : StateBase{ owner } {};
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

         /**
          * \class ゲーム終了選択状態クラス
          * \brief ゲームを終了する
          */
         class StateEndSelect : public StateBase{
         public:
            /**
             * \brief コンストラクタ
             * \param owner モードタイトルの参照
             */
            StateEndSelect(ModeTitle& owner) : StateBase{ owner } {};
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
