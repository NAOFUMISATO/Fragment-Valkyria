#pragma once
#include "ObjectBase.h"

namespace FragmentValkyria {
   namespace Enemy {
      class PoorEnemyBase :public Object::ObjectBase {
         using Vector4 = AppFrame::Math::Vector4;
      public:
         PoorEnemyBase(Game::GameMain& gameMain);

         void Init()override;
         void Update()override;
         void Draw()override;

      protected:
         void Fall();
         void Rotate();
         void CreateGatling();

         void HitCheckFromBullet();
         void HitCheckFromFallObject();

         int _stateCnt{ 0 };
         int _collision{ 0 };
         double _hp{ 20.0 };
         std::vector<std::string> _actionList;
         std::vector<std::string> _action;
         Vector4 _moved{ Vector4(0.0, 0.0, 0.0) };
         Vector4 _gatlingMoveDirection{ Vector4(0.0, 0.0, 0.0) };

      public:
         class StateBase : public AppFrame::State::StateBaseRoot
         {
         public:
            
            StateBase(PoorEnemyBase& owner) : _owner{ owner } {};
            /**
             * \brief 描画処理
             */
            virtual void Draw() override;

         protected:
            PoorEnemyBase& _owner;
         };
        
         class StateIdle : public StateBase
         {
         public:
            
            StateIdle(PoorEnemyBase& owner) : StateBase{ owner } {};
            /**
             * \brief 入口処理
             */
            void Enter() override;
            /**
             * \brief 更新処理
             */
            void Update() override;
         };
         
         class StateFall : public StateBase
         {
         public:
            
            StateFall(PoorEnemyBase& owner) : StateBase{ owner } {};
            /**
             * \brief 入口処理
             */
            void Enter() override;
            /**
             * \brief 更新処理
             */
            void Update() override;
         };
         
         class StateDie : public StateBase
         {
         public:
            
            StateDie(PoorEnemyBase& owner) : StateBase{ owner } {};
            /**
             * \brief 入口処理
             */
            void Enter() override;
            /**
             * \brief 更新処理
             */
            void Update() override;
         private:
            int _timeOver{ 0 };
         };
      };
   }
}
