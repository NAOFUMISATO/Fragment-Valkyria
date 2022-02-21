#pragma once
#include "PoorEnemyBase.h"

namespace FragmentValkyria {
   namespace Enemy {
      class PoorEnemyMelee :public PoorEnemyBase {
      public:
         PoorEnemyMelee(Game::GameMain& gameMain);
         void Init()override;
      private:
         void Rush();
      public:
         class StateIdle : public PoorEnemyBase::StateIdle
         {
         public:

            StateIdle(PoorEnemyMelee& owner) : PoorEnemyBase::StateIdle{ owner } {};
            /**
             * \brief 入口処理
             */
            void Enter() override;
            /**
             * \brief 更新処理
             */
            void Update() override;
         };

         class StateRush : public PoorEnemyBase::StateBase
         {
         public:

            StateRush(PoorEnemyMelee& owner) : PoorEnemyBase::StateBase{ owner } {};
            /**
             * \brief 入口処理
             */
            void Enter() override;
            /**
             * \brief 更新処理
             */
            void Update() override;
            /**
             * \brief 描画処理
             */
            void Draw() override;

         };
      };
   }
}
