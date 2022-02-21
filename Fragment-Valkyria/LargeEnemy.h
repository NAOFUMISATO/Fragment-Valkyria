#pragma once
/*****************************************************************//**
 * \file   LargeEnemy.h
 * \brief  ラージエネミーの処理を回すクラス
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "ObjectBase.h"
/**
 * \brief プロジェクト名
 */
namespace FragmentValkyria {
    /**
     * \brief 敵関係
     */
    namespace Enemy {
        /**
         * \class ラージエネミークラス
         * \brief ラージエネミークラスを管理する
         */
        class LargeEnemy : public Object::ObjectBase {
            using Vector4 = AppFrame::Math::Vector4;
            using Matrix44 = AppFrame::Math::Matrix44;
            using InputManager = AppFrame::Input::InputManager;
        public:
            /**
             * \brief コンストラクタ
             * \param gameMain ゲーム本体の参照
             */
            LargeEnemy(Game::GameMain& gameMain);
            /**
             * \brief デフォルトデストラクタ
             */
            virtual ~LargeEnemy() override = default;
            /**
             * \brief オブジェクトの種別を返す
             * \return ラージエネミー
             */
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
            /**
             * \brief ガトリングの生成
             */
            void CreateGatling();
            /**
             * \brief レーザーの生成
             */
            void CreateLaser();
            /**
             * \brief ボスの体力の取得
             * \return ボスの体力
             */
            double hp() { return _hp; }
        private:
            /**
             * \brief 落下オブジェクトを生成する
             */
            void CreateFallObject();
            /**
             * \brief 落下オブジェクトに当たったか確認
             */
            void HitCheckFromFallObject();
            /**
             * \brief プレイヤーの遠隔弱攻撃の弾に当たったか確認
             */
            void HitCheckFromBullet();
            /**
             * \brief 移動処理
             * \param moved 移動量のベクトル
             */
            void Move(const Vector4& moved);
            /**
             * \brief 回転処理
             * \param rotating 回転するか
             */
            void Rotate(bool& rotating);
            /**
             * \brief 回転の角速度の設定
             */
            void SetAddRotate();
            int _stateCnt{ 0 };              //!< 各状態に入ってからの進捗
            int _gatlingCnt{ 0 };            //!< ガトリングの弾を打つ回数
            int _collision{ 0 };             //!< モデルのコリジョンフレーム番号
            int _freezeTime{ 0 };            //!< 死亡してからゲームクリアまでのフレーム数
            bool _fallObjectflag{ false };   //!< 落下オブジェクトを
            bool _gatlingFlag{ false };
            bool _moving{ false };
            bool _firstRotating{ true };
            bool _endRotating{ true };
            double _rotateDot{ 0.0 };
            double _addRotate{ 1.0 };
            double _hp{ 100.0 };
            int _createNum{ 0 };

            Vector4 _moved{ 0.0, 0.0, 0.0 }; //!< 移動量のベクトル

            std::vector<std::pair<double, Vector4>> _objectDistance;

            std::vector<std::string> _actionList;
            std::vector<std::string> _action;
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
               void FootStepSound();
               bool _footRightStep{ false };       //!< 足音処理のフラグ
               bool _footLeftStep{ false };       //!< 足音処理のフラグ
                int _footCnt{ 0 };
                bool _endGetplyPos{ true };
            };
            /**
            * \class レーザー攻撃状態クラス
            * \brief レーザー攻撃状態の処理を回す
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
            private:
                bool _createLaser{ false };   //!< レーザーを生成したか
            };
        };
    }
}
