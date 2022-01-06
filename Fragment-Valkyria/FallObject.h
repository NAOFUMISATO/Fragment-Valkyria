#pragma once
/*****************************************************************//**
 * \file   FallObject.h
 * \brief  
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "ObjectBase.h"

namespace FragmentValkyria {

	namespace FallObject {

		class FallObject : public Object::ObjectBase {
            using InputManager = AppFrame::Input::InputManager;
		public:
			FallObject(Game::GameMain& gameMain);
			virtual ~FallObject() override = default;

			/**
		    * \brief オブジェクトの種類を返す
		    * \return 落ちてくるオブジェクト
		    */
			ObjectType GetObjType() const override { return ObjectType::FallObject; };

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

            const double Gravity{ 0.01 };
            double _fallTimer{ 0.0 };

        public:
            /**
            * \class プレイヤー状態の基底クラス
            * \brief 各プレイヤーの状態はこれを派生して定義する
            */
            class StateBase : public AppFrame::State::StateBaseRoot
            {
            public:
                /**
                 * \brief コンストラクタ
                 * \param owner 落ちてくるオブジェクトの参照
                 */
                StateBase(FallObject& owner) : _owner{ owner } {};
                /**
                 * \brief 描画処理
                 */
                virtual void Draw() override;

            protected:
                FallObject& _owner;   //!< プレイヤーの参照
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
                 * \param owner 落ちてくるオブジェクトの参照
                 */
                StateIdle(FallObject& owner) : StateBase{ owner } {};
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
             * \class 落下状態クラス
             * \brief 落下状態の処理を回す
             */
            class StateFall : public StateBase
            {
            public:
                /**
                 * \brief コンストラクタ
                 * \param owner プレイヤーの参照
                 */
                StateFall(FallObject& owner) : StateBase{ owner } {};
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
