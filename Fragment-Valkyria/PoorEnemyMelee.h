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
             * \brief ��������
             */
            void Enter() override;
            /**
             * \brief �X�V����
             */
            void Update() override;

         private:
            PoorEnemyMelee& _owner;
         };

         class StateRush : public StateBase {
         public:

            StateRush(PoorEnemyMelee& owner) : StateBase{ owner }, _owner{ owner } {};
            /**
             * \brief ��������
             */
            void Enter() override;
            /**
             * \brief �X�V����
             */
            void Update() override;

         private:
            PoorEnemyMelee& _owner;
            Vector4 _moved{ 0,0,0 };
         };
      };
   }
}
