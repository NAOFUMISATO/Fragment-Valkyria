#pragma once
/*****************************************************************//**
 * \file   Bullet.h
 * \brief  プレイヤーの遠隔弱攻撃の弾の処理を回すクラス
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/

#include "ObjectBase.h"

namespace FragmentValkyria {

   namespace Player {

      class Bullet : public Object::ObjectBase {
            using Vector4 = AppFrame::Math::Vector4;
            using InputManager = AppFrame::Input::InputManager;
      public:
         Bullet(Game::GameMain& gameMain);

         virtual ~Bullet() override = default;

         ObjectType GetObjType() const override { return ObjectType::Bullet; };

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
            void Move();

            void HitCheckFromLargeEnemy();

            void HitCheckFromPoorEnemyGatling();

            void OutCheckFromStage();

            Vector4 _moved{ Vector4() };

        public:
            /**
            * \class プレイヤーの遠隔弱攻撃の弾の状態の基底クラス
            * \brief 各プレイヤーの遠隔弱攻撃の弾の状態はこれを派生して定義する
            */
            class StateBase : public AppFrame::State::StateBaseRoot
            {
            public:
                /**
                 * \brief コンストラクタ
                 * \param owner プレイヤーの参照
                 */
                StateBase(Bullet& owner) : _owner{ owner } {};
                /**
                 * \brief 描画処理
                 */
                virtual void Draw() override;

            protected:
                Bullet& _owner;   //!< プレイヤーの参照
            };
            /**
             * \class 発射状態クラス
             * \brief 発射状態の処理を回す
             */
            class StateShoot : public StateBase
            {
            public:
                /**
                 * \brief コンストラクタ
                 * \param owner プレイヤーの参照
                 */
                StateShoot(Bullet& owner) : StateBase{ owner } {};
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
             * \class 発射状態クラス
             * \brief 発射状態の処理を回す
             */
            class StateDie : public StateBase
            {
            public:
                /**
                 * \brief コンストラクタ
                 * \param owner プレイヤーの参照
                 */
                StateDie(Bullet& owner) : StateBase{ owner } {};
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
