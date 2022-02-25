#pragma once
/*****************************************************************//**
 * \file   PoorEnemyGatling.h
 * \brief  ガトリング攻撃をしてくる雑魚敵の処理を回すクラス
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "PoorEnemyBase.h"
namespace FragmentValkyria {
   namespace Enemy {

      class PoorEnemyGatling : public PoorEnemyBase {
            using Vector4 = AppFrame::Math::Vector4;
            using InputManager = AppFrame::Input::InputManager;
        public:
            PoorEnemyGatling(Game::GameMain& gameMain);
            void Init()override;
        private:
            void CreateGatling();

            Vector4 _moved{ Vector4(0.0, 0.0, 0.0) };
            Vector4 _gatlingMoveDirection{ Vector4(0.0, 0.0, 0.0) };

      public:
            /**
            * \class 移動状態クラス
            * \brief 移動状態の処理を回す
            */
            class StateGatling : public StateBase{
            public:
                /**
                 * \brief コンストラクタ
                 * \param owner ガトリング攻撃をしてくる雑魚敵の参照
                 */
                StateGatling(PoorEnemyGatling& owner) : StateBase{ owner }, _owner{ owner } {};
                /**
                 * \brief 入口処理
                 */
                void Enter() override;
                /**
                 * \brief 更新処理
                 */
                void Update() override;

            private:
               PoorEnemyGatling& _owner;
               int _gatlingCnt{ 5 };
            };
      };
   }

}
