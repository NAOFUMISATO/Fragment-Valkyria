#pragma once
/*****************************************************************//**
 * \file   Laser.h
 * \brief  レーザーの処理を回すクラス
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "ObjectBase.h"

namespace FragmentValkyria {

	namespace Enemy {

		class Laser : public Object::ObjectBase {
            using Vector4 = AppFrame::Math::Vector4;
		public:
			Laser(Game::GameMain& gameMain);

			virtual ~Laser() override = default;
			/**
			* \brief オブジェクトの種類を返す
			* \return レーザー
			*/
			ObjectType GetObjType() const override { return ObjectType::Laser; };
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

            void end(Vector4 endPos) { _end = endPos; }
            inline Vector4 end() { return _end; }

		private:

            Vector4 _end{ Vector4(0.0, 0.0, 0.0) };
            int _stateCnt{ 0 };
		public:
            /**
            * \class レーザーの状態の基底クラス
            * \brief 各レーザーの状態はこれを派生して定義する
            */
            class StateBase : public AppFrame::State::StateBaseRoot
            {
            public:
                /**
                 * \brief コンストラクタ
                 * \param owner プレイヤーの参照
                 */
                StateBase(Laser& owner) : _owner{ owner } {};
                /**
                 * \brief 描画処理
                 */
                virtual void Draw() override;

            protected:
                Laser& _owner;   //!< レーザーの参照
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
                 * \param owner レーザーの参照
                 */
                StateIdle(Laser& owner) : StateBase{ owner } {};
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
