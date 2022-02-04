#pragma once
/*****************************************************************//**
 * \file   PoorEnemyGatling.h
 * \brief  ガトリング攻撃をしてくる雑魚敵の処理を回すクラス
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "ObjectBase.h"
namespace FragmentValkyria {
	namespace Enemy {

		class PoorEnemyGatling : public Object::ObjectBase {
            using Vector4 = AppFrame::Math::Vector4;
            using InputManager = AppFrame::Input::InputManager;
        public:
			PoorEnemyGatling(Game::GameMain& gameMain);
			virtual ~PoorEnemyGatling() override = default;

            virtual ObjectType GetObjType() const override { return ObjectType::PoorEnemyGatling; };
            /**
            * \brief 初期化処理
            */
            virtual void Init() override;
            /**
             * \brief 更新処理
             */
            virtual void Update() override;
            /**
             * \brief 描画処理
             */
            void Draw() override;

        private:
            void Fall();
            void Rotate();
            void CreateGatling();

            void HitCheckFromBullet();
            void HitCheckFromFallObject();

            Vector4 _moved{ Vector4(0.0, 0.0, 0.0) };

            int _stateCnt{ 0 };
            double _hp{ 20.0 };

            int _collision{ 0 };

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
                 * \brief 更新処理
                 */
                void Update() override;
            };
            /**
            * \class 落下状態クラス
            * \brief 落下状態の処理を回す
            */
            class StateFall : public StateBase
            {
            public:
                /**
                 * \brief コンストラクタ
                 * \param owner ガトリング攻撃をしてくる雑魚敵の参照
                 */
                StateFall(PoorEnemyGatling& owner) : StateBase{ owner } {};
                /**
                 * \brief 入口処理
                 */
                void Enter() override;
                /**
                 * \brief 更新処理
                 */
                void Update() override;
            };
            /**
            * \class 移動状態クラス
            * \brief 移動状態の処理を回す
            */
            class StateGatling : public StateBase
            {
            public:
                /**
                 * \brief コンストラクタ
                 * \param owner ガトリング攻撃をしてくる雑魚敵の参照
                 */
                StateGatling(PoorEnemyGatling& owner) : StateBase{ owner } {};
                /**
                 * \brief 入口処理
                 */
                void Enter() override;
                /**
                 * \brief 更新処理
                 */
                void Update() override;
            private:
                int _gatlingCnt{ 5 };
            };
            /**
            * \class 死亡状態クラス
            * \brief 死亡状態の処理を回す
            */
            class StateDie : public StateBase
            {
            public:
                /**
                 * \brief コンストラクタ
                 * \param owner ガトリング攻撃をしてくる雑魚敵の参照
                 */
                StateDie(PoorEnemyGatling& owner) : StateBase{ owner } {};
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
