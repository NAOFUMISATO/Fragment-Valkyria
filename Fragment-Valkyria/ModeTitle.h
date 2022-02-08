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
         using handles = std::vector<int>;
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
         virtual void Enter()override;
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
         std::unique_ptr<AppFrame::State::StateServer> _stateServer;
         int _alpha{ 0 };
         bool _pushAnyBotton{ false };
         std::pair<int, int> _cusorPos{ 0,0 };
         std::tuple<int, handles, handles, int, int, int, int> _grHandles{
           -1,0,0,-1,-1,-1,-1
         };

      public:

         class StateBase : public AppFrame::State::StateBaseRoot {
         public:

            StateBase(ModeTitle& owner) : _owner{ owner } {};

            void Draw() override;

         protected:
            ModeTitle& _owner;
         };

         class StateAnyBotton : public StateBase{
         public:

            StateAnyBotton(ModeTitle& owner) :StateBase{ owner } {};

            void Enter() override;

            void Input(InputManager& input) override;

            void Exit()override;

         };

         class StateStartSelect : public StateBase{
         public:

            StateStartSelect(ModeTitle& owner) : StateBase{ owner } {};

            void Enter() override;

            void Input(InputManager& input) override;

         };

         class StateOptionSelect : public StateBase {
         public:

            StateOptionSelect(ModeTitle& owner) : StateBase{ owner } {};

            void Enter() override;

            void Input(InputManager& input) override;

         };

         class StateEndSelect : public StateBase{
         public:

            StateEndSelect(ModeTitle& owner) : StateBase{ owner } {};

            void Enter() override;

            void Input(InputManager& input) override;

         };
      };
   }
}
