#pragma once
/*****************************************************************//**
 * \file   PoorEnemyGatling.h
 * \brief  ガトリング攻撃をしてくる雑魚敵の処理を回すクラス
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "LargeEnemy.h"
namespace FragmentValkyria {
	namespace Enemy {

		class PoorEnemyGatling : public Enemy::LargeEnemy {
            using InputManager = AppFrame::Input::InputManager;
        public:
			PoorEnemyGatling(Game::GameMain& gameMain);
			virtual ~PoorEnemyGatling() override = default;

            virtual ObjectType GetObjType() const override { return ObjectType::LargeEnemy; };
            /**
            * \brief 初期化処理
            */
            virtual void Init() override;
            /**
             * \brief 入力処理
             * \param input 入力一括管理クラスの参照
             */
            virtual void Input(InputManager& input) override;
            /**
             * \brief 更新処理
             */
            virtual void Update() override;
            /**
             * \brief 描画処理
             */
            void Draw() override;

        private:

        public:
            /**
            * \class ガトリング攻撃をしてくる雑魚敵の状態の基底クラス
            * \brief 各ガトリング攻撃をしてくる雑魚敵の状態はこれを派生して定義する
            */
            class StateBase : public AppFrame::State::StateBaseRoot
            {
            public:
                /**
                 * \brief コンストラクタ
                 * \param owner ガトリング攻撃をしてくる雑魚敵の参照
                 */
                StateBase(PoorEnemyGatling& owner) : _owner{ owner } {};
                /**
                 * \brief 描画処理
                 */
                virtual void Draw() override;

            protected:
                PoorEnemyGatling& _owner;   //!< ガトリング攻撃をしてくる雑魚敵の参照
            };
            /**
            * \class 待機状態クラス
            * \brief 待機状態の処理を回す
            */
            class StateIdle : public StateBase
            {
            public:
                /**
                 * \brief コンストラクタ
                 * \param owner ガトリング攻撃をしてくる雑魚敵の参照
                 */
                StateIdle(PoorEnemyGatling& owner) : StateBase{ owner } {};
                /**
                 * \brief 入口処理
                 */
                void Enter() override;
                /**
                 * \brief 入力処理
                 * \param input 入力一括管理クラスの参照
                 */
                void Input(InputManager& input) override;
                /**
                 * \brief 更新処理
                 */
                void Update() override;
            };
            /**
            * \class 移動状態クラス
            * \brief 移動状態の処理を回す
            */
            class StateMove : public StateBase
            {
            public:
                /**
                 * \brief コンストラクタ
                 * \param owner ガトリング攻撃をしてくる雑魚敵の参照
                 */
                StateMove(PoorEnemyGatling& owner) : StateBase{ owner } {};
                /**
                 * \brief 入口処理
                 */
                void Enter() override;
                /**
                 * \brief 入力処理
                 * \param input 入力一括管理クラスの参照
                 */
                void Input(InputManager& input) override;
                /**
                 * \brief 更新処理
                 */
                void Update() override;
            };
		};
	}

}
