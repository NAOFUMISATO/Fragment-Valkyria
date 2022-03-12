#pragma once
#include "ModeInGameBase.h"

namespace FragmentValkyria {
   namespace Mode {
      class ModeTutorial :public ModeInGameBase {
         using InputManager = AppFrame::Input::InputManager;
      public:
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
         bool TipsAlive();
         void ClearJudge(std::string_view key);
         void TipsBorn(std::string_view key);
         void FallObjectRespawn();
         int _tutorialProgress{ 1 };
         bool _born{ true };
         bool _isAppear{ false };
      };
   }
}
