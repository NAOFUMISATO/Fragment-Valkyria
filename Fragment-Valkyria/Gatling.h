#pragma once
/*****************************************************************//**
 * \file   Gatling.h
 * \brief  
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/

#include "ObjectBase.h"

namespace FragmentValkyria {

	namespace Enemy {

		class Gatling : public Object::ObjectBase {
            using Vector4 = AppFrame::Math::Vector4;
            using InputManager = AppFrame::Input::InputManager;
		public:
			Gatling(Game::GameMain& gameMain);

			virtual ~Gatling() override = default;

            /**
            * \brief オブジェクトの種類を返す
            * \return プレイヤー
            */
            ObjectType GetObjType() const override { return ObjectType::Gatling; };
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

            Vector4 _moved{ Vector4(0.0, 0.0, 0.0) };
            Vector4 _moveDirection{ Vector4(0.0, 0.0, 0.0) };

            const double Speed{ 2.0 };
            double _radian{ 100.0 };

		public:
            /**
            * \class ガトリングの状態の基底クラス
            * \brief 各ガトリングの状態はこれを派生して定義する
            */
            class StateBase : public AppFrame::State::StateBaseRoot
            {
            public:
                /**
                 * \brief コンストラクタ
                 * \param owner プレイヤーの参照
                 */
                StateBase(Gatling& owner) : _owner{ owner } {};
                /**
                 * \brief 描画処理
                 */
                virtual void Draw() override;

            protected:
                Gatling& _owner;   //!< プレイヤーの参照
            };
            /**
             * \class 追撃状態クラス
             * \brief 追撃状態の処理を回す
             */
            class StateChase : public StateBase
            {
            public:
                /**
                 * \brief コンストラクタ
                 * \param owner プレイヤーの参照
                 */
                StateChase(Gatling& owner) : StateBase{ owner } {};
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
