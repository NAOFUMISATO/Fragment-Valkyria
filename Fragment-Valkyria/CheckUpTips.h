#pragma once
#include "SpriteBase.h"

namespace FragmentValkyria {
   namespace Clear {
      class CheckUpTips :public Sprite::SpriteBase {
         using InputManager = AppFrame::Input::InputManager;
      public:
         CheckUpTips(Game::GameMain& gameMain);
         /**
          * \brief 初期化処理
          */
         void Init()override;
         /**
          * \brief 入力処理
          * \param input 入力一括管理クラスの参照
          */
         void Input(InputManager& input)override;
         /**
          * \brief 更新処理
          */
         void Update()override;
         /**
          * \brief 描画処理
          */
         void Draw()override;
         /**
          * \brief スプライト種別の取得
          * \return 種別:クリア案内表示テキスト
          */
         virtual SpriteType GetSprType() const { return SpriteType::ClearTips; }

      private:
         int _width{ 0 };
         int _grHandle{ -1 };
         bool _isAppear{ false };
         std::unique_ptr<AppFrame::State::StateServer> _stateServer;  //!< 状態一括管理クラスのポインタ 

      public:
         class StateBase : public AppFrame::State::StateBaseRoot {
         public:

            StateBase(CheckUpTips& owner) : _owner{ owner } {};
            /**
             * \brief 描画処理
             */
            void Draw() override;

         protected:
            CheckUpTips& _owner;   //!< モードタイトルの参照
         };

         class StateFadeIn : public StateBase {
         public:
            StateFadeIn(CheckUpTips& owner) :StateBase{ owner } {};
            /**
             * \brief 入口処理
             */
            void Enter() override;
            /**
             * \brief 更新処理
             */
            void Update()override;
            /**
             * \brief 出口処理
             */
            void Exit()override;
         };

         class StateJudge : public StateBase {
         public:
            StateJudge(CheckUpTips& owner) : StateBase{ owner } {};
            /**
            * \brief 更新処理
            */
            void Update()override;

         };

         class StateFadeOut : public StateBase {
         public:
            StateFadeOut(CheckUpTips& owner) : StateBase{ owner } {};
            /**
             * \brief 入口処理
             */
            void Enter() override;
            /**
             * \brief 更新処理
             */
            void Update()override;
         };
      };
   }
}
