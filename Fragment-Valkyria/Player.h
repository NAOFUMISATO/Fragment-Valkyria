#pragma once
/*****************************************************************//**
 * \file   Player.h
 * \brief  プレイヤーの処理を回すクラス
 *
 * \author NAOFUMISATO, AHMD2000
 * \date   December 2021
 *********************************************************************/
#include "ObjectBase.h"
 /**
  * \brief プロジェクト名
  */
namespace FragmentValkyria {
   // 二重インクルード防止
   namespace Camera {
      class CameraComponent;
   }
   namespace Model {
      class ModelAnimeComponent;
   }
   /**
    * \brief プレイヤー関係
    */
   namespace Player {
      /**
       * \class プレイヤークラス
       * \brief プレイヤーを管理する
       */
      class Player : public Object::ObjectBase {
         using Vector4 = AppFrame::Math::Vector4;
         using Matrix44 = AppFrame::Math::Matrix44;
         using InputManager = AppFrame::Input::InputManager;
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体の参照
          */
         Player(Game::GameMain& gameMain);
         /**
          * \brief デフォルトデストラクタ
          */
         virtual ~Player() override = default;
         /**
          * \brief オブジェクトの種類を返す
          * \return プレイヤー
          */
         ObjectType GetObjType() const override { return ObjectType::Player; };
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
          * \brief ワールド行列の取得
          */
         void ComputeWorldTransform() override;
         /**
          * \brief プレイヤーの体力の取得
          * \return プレイヤーの体力
          */
         inline double hp() { return _hp; }
         /**
          * \brief 無敵時間の取得
          * \return 無敵時間
          */
         inline int invincibleCnt() { return _invincibleCnt; }
      private:
         /**
          * \brief 移動処理
          * \param moved 移動量のベクトル
          */
         void Move(const Vector4& moved);
         /**
          * \brief 射撃準備状態の回転処理
          */
         void ShootRotate();
         /**
          * \brief 落下オブジェクトを持ち上げられる範囲にいるか確認
          */
         void HitCheckFromFallObjectRange();
         /**
          * \brief 待機状態の落下オブジェクトと当たっているか確認
          */
         void HitCheckFromIdleFallObject();
         /**
          * \brief ガトリングと当たっているか確認
          */
         void HitCheckFromGatling();
         /**
          * \brief 落下中の落下オブジェクトと当たっているか確認
          */
         void HitCheckFromFallObject();
         /**
          * \brief レーザーと当たっているか確認
          */
         void HitCheckFromLaser();
         /**
          * \brief ラージエネミーと当たっているか確認
          */
         void HitCheckFromLargeEnemy();
         /**
          * \brief 遠隔弱攻撃をする
          */
         void WeakAttack();

         int _freezeTime{ 0 };                      //!< ノックバックする時間
         int _bulletStock{ 5 };                     //!< 遠隔弱攻撃の残り弾数
         int _invincibleCnt{ 0 };                   //!< 無敵時間
         int _portion{ 3 };                         //!< ポーションの数
         double _hp{ 100.0 };                       //!< ヒットポイント
         Vector4 _moved{ Vector4() };               //!< 移動量のベクトル
         Vector4 _knockBack{ Vector4() };           //!< ノックバック量のベクトル
         Vector4 _direction{ Vector4() };           //!< 前進方向のベクトル
         Matrix44 _rightRotation{ Matrix44() };     //!< ベクトルを90度回転させるマトリクス
         Matrix44 _leftRotation{ Matrix44() };      //!< ベクトルを-90度回転させるマトリクス
         Matrix44 _backRotation{ Matrix44() };      //!< ベクトルを180度回転させるマトリクス
      public:
         /**
          * \class プレイヤー状態の基底クラス
          * \brief 各プレイヤーの状態はこれを派生して定義する
          */
         class StateBase : public AppFrame::State::StateBaseRoot {
         public:
            /**
             * \brief コンストラクタ
             * \param owner プレイヤーの参照
             */
            StateBase(Player& owner) : _owner{ owner } {};
            /**
             * \brief 描画処理
             */
            virtual void Draw() override;

         protected:
            Player& _owner;   //!< プレイヤーの参照
         };
         /**
          * \class 待機状態クラス
          * \brief 待機状態の処理を回す
          */
         class StateIdle : public StateBase {
         public:
            /**
             * \brief コンストラクタ
             * \param owner プレイヤーの参照
             */
            StateIdle(Player& owner) : StateBase{ owner } {};
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
          * \class 走り状態クラス
          * \brief 走り状態の処理を回す
          */
         class StateRun : public StateBase {
         public:
            /**
             * \brief コンストラクタ
             * \param owner プレイヤーの参照
             */
            StateRun(Player& owner) : StateBase{ owner } {};
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

         private:
            void FootStepSound();
            int _footCnt{ 0 };
            bool _footRightStep{ false };       //!< 足音処理のフラグ
            bool _footLeftStep{ false };       //!< 足音処理のフラグ
         };
         /**
          * \class 攻撃状態クラス
          * \brief 攻撃状態の処理を回す
          */
         class StateAttack : public StateBase {
         public:
            /**
             * \brief コンストラクタ
             * \param owner プレイヤーの参照
             */
            StateAttack(Player& owner) : StateBase{ owner } {};
            /**
             * \brief 入口処理
             */
            void Enter() override;
            /**
             * \brief 更新処理
             */
            void Update() override;
            /**
             * \brief 描画処理
             */
            void Draw() override;
         };
         /**
          * \class 射撃準備状態クラス
          * \brief 射撃準備状態の処理を回す
          */
         class StateShootReady : public StateBase
         {
         public:
             /**
              * \brief コンストラクタ
              * \param owner プレイヤーの参照
              */
             StateShootReady(Player& owner) : StateBase{ owner } {};
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
             /**
              * \brief 描画処理
              */
             void Draw() override;
         };
         /**
          * \class ノックバック状態クラス
          * \brief ノックバック状態の処理を回す
          */
         class StateKnockBack : public StateBase
         {
         public:
             /**
              * \brief コンストラクタ
              * \param owner プレイヤーの参照
              */
             StateKnockBack(Player& owner) : StateBase{ owner } {};
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
             /**
              * \brief 描画処理
              */
             void Draw() override;
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
              * \param owner プレイヤーの参照
              */
             StateDie(Player& owner) : StateBase{ owner } {};
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
             /**
              * \brief 描画処理
              */
             void Draw() override;
         private:
             int _timeOver{ 0 };
         };
         /**
          * \class 射撃準備状態クラス
          * \brief 射撃準備状態の処理を回す
          */
         class StateWeakShootReady : public StateBase
         {
         public:
             /**
              * \brief コンストラクタ
              * \param owner プレイヤーの参照
              */
             StateWeakShootReady(Player& owner) : StateBase{ owner } {};
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
         private:
             int _coolTime{ 0 };

         };
         /**
          * \class 装填状態クラス
          * \brief 装填状態の処理を回す
          */
         class StateReload : public StateBase
         {
         public:
             /**
              * \brief コンストラクタ
              * \param owner プレイヤーの参照
              */
             StateReload(Player& owner) : StateBase{ owner } {};
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

         private:
             int _reloadCnt{ 0 };
         };
         /**
          * \class 回復状態クラス
          * \brief 回復状態の処理を回す
          */
         class StateRecovery : public StateBase
         {
         public:
             /**
              * \brief コンストラクタ
              * \param owner プレイヤーの参照
              */
             StateRecovery(Player& owner) : StateBase{ owner } {};
             /**
              * \brief 入口処理
              */
             void Enter() override;
             /**
              * \brief 更新処理
              */
             void Update() override;

         private:
             int _recoveryCnt{ 0 };
         };
      };
   }
}
