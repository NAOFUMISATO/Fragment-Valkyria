#pragma once
#include "SpriteBase.h"
/**
 * \brief プロジェクト名
 */
namespace FragmentValkyria {
   namespace Game {
      class GameMain;
   }
   /**
    * \brief チュートリアル関係
    */
   namespace Tutorial {
      class TutorialTips :public Sprite::SpriteBase {
         using InputManager = AppFrame::Input::InputManager;
      public:
         TutorialTips(Game::GameMain& gameMain,std::string_view tipsName);
         /**
          * \brief 初期化処理
          */
         void Init()override;
         /**
          * \brief 入力処理
          * \param input 入力一括管理クラスの参照
          */
         void Input(AppFrame::Input::InputManager& input) override;
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
          * \return 種別:チュートリアル表示テキスト
          */
         virtual SpriteType GetSprType() const { return SpriteType::TutorialTips; }
         /**
          * \brief 出現したかどうかを返す
          * \return 出現したならtrue,でないならばfalse
          */
         bool isAppear() { return _isAppear; }
         
         bool IsTipsClear(std::string_view key) { return _tipsClear[key.data()]; }

      private:
         int _width{ 0 };
         bool _isAppear{ false };
         std::string _tipsName{ "" };
         std::unordered_map<std::string, int> _tipsNum;
         std::unordered_map<std::string, bool> _tipsClear;
         std::unique_ptr<AppFrame::State::StateServer> _stateServer;  //!< 状態一括管理クラスのポインタ 

      public:
         class StateBase : public AppFrame::State::StateBaseRoot {
         public:
            
            StateBase(TutorialTips& owner) : _owner{ owner } {};
            /**
             * \brief 描画処理
             */
            void Draw() override;

         protected:
            TutorialTips& _owner;   //!< モードタイトルの参照
         };

         class StateFadeIn : public StateBase {
         public:
            StateFadeIn(TutorialTips& owner) :StateBase{ owner } {};
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
            StateJudge(TutorialTips& owner) : StateBase{ owner } {};
            /**
             * \brief 入口処理
             */
            void Enter() override;
            /**
             * \brief 入力処理
             * \param input 入力一括管理クラスの参照
             */
            void Input(InputManager& input) override;

         private:
            void MoveJudge(InputManager& input);
            void CameraJudge(InputManager& input);
            void WeakShootJudge(InputManager& input);
            void ReloadJudge(InputManager& input);
            void ObjectShootJudge(InputManager& input);
            void HealJudge(InputManager& input);
            bool _isWeakReady{ false };
            bool _isObjectReady{ false };
         };

         class StateFadeOut : public StateBase {
         public:
            StateFadeOut(TutorialTips& owner) : StateBase{ owner } {};
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
