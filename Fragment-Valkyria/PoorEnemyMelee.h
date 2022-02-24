#pragma once
#include "PoorEnemyBase.h"

namespace FragmentValkyria {
   namespace Enemy {
      class PoorEnemyMelee :public PoorEnemyBase {
         using Vector4 = AppFrame::Math::Vector4;
      public:
         PoorEnemyMelee(Game::GameMain& gameMain);
         void Init()override;

      public:
         class StateIdle : public PoorEnemyBase::StateIdle {
         public:
            StateIdle(PoorEnemyMelee& owner) : PoorEnemyBase::StateIdle{ owner }, _owner{ owner } {};
            /**
             * \brief 入口処理
             */
            void Enter() override;
            /**
             * \brief 更新処理
             */
            void Update() override;

         private:
            PoorEnemyMelee& _owner;
         };

         class StateRush : public StateBase {
         public:

            StateRush(PoorEnemyMelee& owner) : StateBase{ owner }, _owner{ owner } {};
            /**
             * \brief 入口処理
             */
            void Enter() override;
            /**
             * \brief 更新処理
             */
            void Update() override;

         private:
            PoorEnemyMelee& _owner;
            Vector4 _moved{ 0,0,0 };
         };
      };
   }
}
