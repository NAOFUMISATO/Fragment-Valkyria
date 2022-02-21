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
             * \brief ��������
             */
            void Enter() override;
            /**
             * \brief �X�V����
             */
            void Update() override;
         };

         class StateRush : public PoorEnemyBase::StateBase
         {
         public:

            StateRush(PoorEnemyMelee& owner) : PoorEnemyBase::StateBase{ owner } {};
            /**
             * \brief ��������
             */
            void Enter() override;
            /**
             * \brief �X�V����
             */
            void Update() override;
            /**
             * \brief �`�揈��
             */
            void Draw() override;

         };
      };
   }
}
