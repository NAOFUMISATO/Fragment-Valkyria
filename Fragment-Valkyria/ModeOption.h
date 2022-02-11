#pragma once
/*****************************************************************//**
 * \file   ModeOption.h
 * \brief  モードオプションクラス
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
       * \class モードオプションクラス
       * \brief オプション画面を表示し、各設定を行う
       */
      class ModeOption :public ModeBase {
         using InputManager = AppFrame::Input::InputManager;
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体クラスの参照
          */
         ModeOption(Game::GameMain& gameMain);
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
         void Update()override;
         /**
          * \brief 描画処理
          */
         void Render()override;

      private:
         std::unique_ptr<AppFrame::State::StateServer> _stateServer;//!< 状態の一括管理クラスのポインタ
         double _cameraSens{ 0 };                                   //!< カメラ感度
         double _aimSens{ 0 };                                      //!< エイム感度
         int _deadZone{ 0 };                                        //!< デッドゾーン
         std::pair<int, int> _cameraCusorPos{ 0,0 };                //!< カメラ感度バーカーソル座標
         std::pair<int, int> _aimCusorPos{ 0,0 };                   //!< エイム感度バーカーソル座標
         std::pair<int, int> _deadzoneCusorPos{ 0,0 };              //!< デッドゾーンバーカーソル座標
         std::pair<int, int> _selectCusorPos{ 0,0 };                //!< 選択エフェクト画像の座標
         std::tuple<int, int, int, int, int, int, int> _grHandles{  //!< 画像ハンドル群
            -1,-1,-1,-1,-1,-1,-1
         };

      public:
         /**
          * \class モードオプションの状態基底クラス
          * \brief 各状態はこのクラスを派生して定義する
          */
         class StateBase : public AppFrame::State::StateBaseRoot {
         public:
            /**
             * \brief コンストラクタ
             * \param owner モードオプションの参照
             */
            StateBase(ModeOption& owner) : _owner{ owner } {};
            /**
             * \brief 更新処理
             */
            void Update()override;
            /**
             * \brief 描画処理
             */
            void Draw() override;

         protected:
            ModeOption& _owner;   //!< モードオプションの参照
         };

         /**
          * \class カメラ感度設定選択状態クラス
          * \brief カメラ感度の設定を行う
          */
         class StateCameraSencivitySelect : public StateBase{
         public:
            /**
             * \brief コンストラクタ
             * \param owner モードオプションの参照
             */
            StateCameraSencivitySelect(ModeOption& owner) :StateBase{ owner } {};
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

         /**
          * \class エイム感度設定選択状態クラス
          * \brief エイム感度の設定を行う
          */
         class StateAimSencivitySelect : public StateBase{
         public:
            /**
             * \brief コンストラクタ
             * \param owner モードオプションの参照
             */
            StateAimSencivitySelect(ModeOption& owner) : StateBase{ owner } {};
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

         /**
          * \class デッドゾーン設定選択状態クラス
          * \brief デッドゾーンの設定を行う
          */
         class StateDeadZoneSelect : public StateBase{
         public:
            /**
             * \brief コンストラクタ
             * \param owner モードオプションの参照
             */
            StateDeadZoneSelect(ModeOption& owner) : StateBase{ owner } {};
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

         /**
          * \class モードオプション削除選択状態クラス
          * \brief モードオプション削除を行う
          */
         class StateReturnSelect : public StateBase {
         public:
            /**
             * \brief コンストラクタ
             * \param owner モードオプションの参照
             */
            StateReturnSelect(ModeOption& owner) : StateBase{ owner } {};
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