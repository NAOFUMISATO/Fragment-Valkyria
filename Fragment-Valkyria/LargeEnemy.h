#pragma once
/*****************************************************************//**
 * \file   LargeEnemy.h
 * \brief  ラージエネミーの処理を回すクラス
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "ObjectBase.h"
namespace FragmentValkyria {

    namespace Enemy {

        class LargeEnemy : public Object::ObjectBase {
            using Vector4 = AppFrame::Math::Vector4;
            using Matrix44 = AppFrame::Math::Matrix44;
            using InputManager = AppFrame::Input::InputManager;
        public:
            LargeEnemy(Game::GameMain& gameMain);
            virtual ~LargeEnemy() override = default;

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


            void CreateGatling();

            void CreateLaser();

            double hp() { return _hp; }

        private:

            void CreateFallObject();

            void HitCheckFromFallObject();

            void HitCheckFromBullet();

            void Move();
            void Rotate(bool& rotating);
            void SetAddRotate();

            int _stateCnt{ 0 };
            int _gatlingCnt{ 0 };
            int _collision{ 0 };
            const int GatlingFrame{ 60 };
            bool _fallObjectflag{ false };
            bool _moving{ false };
            bool _firstRotating{ true };
            bool _endRotating{ true };
            double _rotateDot{ 0.0 };
            double _addRotate{ 1.0 };
            double _hp{ 100.0 };
            int _freezeTime{ 0 };

            Vector4 _moved{ 0.0, 0.0, 0.0 };


        public:
            /**
            * \class ラージエネミーの状態の基底クラス
            * \brief 各ラージエネミーの状態はこれを派生して定義する
            */
            class StateBase : public AppFrame::State::StateBaseRoot
            {
            public:
                /**
                 * \brief コンストラクタ
                 * \param owner ラージエネミーの参照
                 */
                StateBase(LargeEnemy& owner) : _owner{ owner } {};
                /**
                 * \brief 描画処理
                 */
                virtual void Draw() override;

            protected:
                LargeEnemy& _owner;   //!< ラージエネミーの参照
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
                 * \param owner ラージエネミーの参照
                 */
                StateIdle(LargeEnemy& owner) : StateBase{ owner } {};
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
            * \class オブジェクト落下状態クラス
            * \brief オブジェクト落下の処理を回す
            */
            class StateFallObject : public StateBase
            {
            public:
                /**
                 * \brief コンストラクタ
                 * \param owner ラージエネミーの参照
                 */
                StateFallObject(LargeEnemy& owner) : StateBase{ owner } {};
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
            * \class ガトリング攻撃状態クラス
            * \brief ガトリング攻撃状態の処理を回す
            */
            class StateGatling : public StateBase
            {
            public:
                /**
                 * \brief コンストラクタ
                 * \param owner ラージエネミーの参照
                 */
                StateGatling(LargeEnemy& owner) : StateBase{ owner } {};
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
            * \class ガトリング攻撃状態クラス
            * \brief ガトリング攻撃状態の処理を回す
            */
            class StateDie : public StateBase
            {
            public:
                /**
                 * \brief コンストラクタ
                 * \param owner ラージエネミーの参照
                 */
                StateDie(LargeEnemy& owner) : StateBase{ owner } {};
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
            class StateMove : public StateBase
            {
            public:
                /**
                 * \brief コンストラクタ
                 * \param owner ラージエネミーの参照
                 */
                StateMove(LargeEnemy& owner) : StateBase{ owner } {};
                /**
                 * \brief 入口処理
                 */
                void Enter() override;
                /**
                 * \brief 更新処理
                 */
                void Update() override;

            private:
                bool _endGetplyPos{ true };
            };
            /**
            * \class ガトリング攻撃状態クラス
            * \brief ガトリング攻撃状態の処理を回す
            */
            class StateLaser : public StateBase
            {
            public:
                /**
                 * \brief コンストラクタ
                 * \param owner ラージエネミーの参照
                 */
                StateLaser(LargeEnemy& owner) : StateBase{ owner } {};
                /**
                 * \brief 入口処理
                 */
                void Enter() override;
                /**
                 * \brief 更新処理
                 */
                void Update() override;
            };
        };
    }
}
