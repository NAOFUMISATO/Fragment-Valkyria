#pragma once
/*****************************************************************//**
 * \file   ModeGameOver.h
 * \brief  モードゲームオーバークラス
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
       * \class モードゲームオーバークラス
       * \brief ゲームオーバー画面を表示する
       */
      class ModeGameOver:public ModeBase {
         using InputManager = AppFrame::Input::InputManager;
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体クラスの参照
          */
         ModeGameOver(Game::GameMain& gameMain);
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
         std::vector<int> _continueDrawHandles;
         std::vector<int> _exitDrawHandles;
         std::unordered_map<std::string, std::vector<int>> _handleMap;
         std::unique_ptr<AppFrame::State::StateServer> _stateServer;


      public:
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

         class StateContinue : public StateBase {
         public:
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

         class StateExit : public StateBase {
         public:
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
