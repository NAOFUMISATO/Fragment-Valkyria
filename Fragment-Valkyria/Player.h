#pragma once
/*****************************************************************//**
 * \file   Player.h
 * \brief  プレイヤーの処理を回すクラス
 *
 * \author NAOFUMISATO, YUSUKEAHAMEDO
 * \date   December 2021
 *********************************************************************/
#include "ObjectBase.h"
#include "ParamCollision.h"
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
          */
         Player();
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
          * \brief 無敵時間の取得
          * \return 無敵時間
          */
         inline int invincibleCnt() { return _invincibleCnt; }
         /**
          * \brief エイム中かの取得
          * \return エイム中ならばtrue、でなければfalse
          */
         inline bool isAim() { return _isAim; }
         /**
          * \brief 死亡モーション中かの取得
          * \return 死亡モーション中ならばtrue,でなければfalse
          */
         inline bool isDeadMotion() { return _isDeadMotion; }
         /**
          * \brief オブジェクトを持ち上げられるかの取得
          * \return オブジェクトを持ち上げられるならtrue,持ち上げられなければfalse
          */
         inline bool isLift() { return _isLift; }
         /**
          * \brief オブジェクトを打てるかを取得
          * \return オブジェクトを打てるならtrue,打てなければfalse
          */
         inline bool objectShoot() { return _objectShoot; }
         /**
          * \brief オブジェクトを打てるか設定
          * \param shoot オブジェクトを打てるならtrue,打てなければfalse
          */
         inline void objectShoot(bool shoot) { _objectShoot = shoot; }
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
          * \brief プレイヤーの回転の処理
          */
         void Rotate();
         /**
          * \brief 落下オブジェクトを持ち上げられる範囲にいるか確認
          */
         void HitCheckFromFallObjectRange();
         /**
          * \brief 待機状態の落下オブジェクトと当たっているか確認
          */
         void HitCheckFromIdleFallObject(std::string_view state);
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
          * \brief 雑魚敵と当たっているか確認
          */
         void HitCheckFromPoorEnemy();
         /**
          * \brief 遠隔弱攻撃をする
          */
         void WeakAttack();

         std::unique_ptr<Param::ParamCollision> _collParam; //!< 当たり判定クラスの値管理クラスのポインタ
         int _freezeTime{ 0 };                              //!< ノックバックする時間
         int _bulletStock{ 5 };                             //!< 遠隔弱攻撃の残り弾数
         int _invincibleCnt{ 0 };                           //!< 無敵時間
         int _portionStock{ 3 };                            //!< ポーションの数
         int _stateCnt{ 0 };                                //!< 各状態に入った時のモードサーバーのフレームカウント数
         int _invincibleModeCnt{ 0 };                       //!< 無敵状態に入った時のモードサーバーのフレームカウント数
         int _coolTime{ 0 };                                //!< 遠隔弱攻撃のクールタイム
         int _blendMode{ DX_BLENDMODE_ALPHA };              //!< ブレンドモード
         bool _isAim{ false };                              //!< エイム中か
         bool _isDeadMotion{ false };                       //!< 死亡モーション中か
         bool _isLift{ true };                              //!< オブジェクトを持ち上げられるか
         bool _objectShoot{ false };                        //!< オブジェクトを打てるか
         double _hp{ 100.0 };                               //!< ヒットポイント
         Vector4 _moved{ Vector4() };                       //!< 移動量のベクトル
         Vector4 _knockBack{ Vector4() };                   //!< ノックバック量のベクトル
         Vector4 _direction{ Vector4() };                   //!< 前進方向のベクトル
         Vector4 _rotateDir{ Vector4() };                   //!< プレイヤーの向かせたい向き
         Matrix44 _rightRotation{ Matrix44() };             //!< ベクトルを90度回転させるマトリクス
         Matrix44 _leftRotation{ Matrix44() };              //!< ベクトルを-90度回転させるマトリクス
         Matrix44 _backRotation{ Matrix44() };              //!< ベクトルを180度回転させるマトリクス

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
             * \brief 更新処理
             */
            virtual void Update() override;
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
          * \class 歩き状態クラス
          * \brief 歩き状態の処理を回す
          */
         class StateWalk : public StateBase {
         public:
            /**
             * \brief コンストラクタ
             * \param owner プレイヤーの参照
             */
            StateWalk(Player& owner) : StateBase{ owner } {};
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
            /**
             * \brief 足音処理
             */
            void FootStepSound();
            int _footCnt{ 0 };                  //!< 足音が鳴り出すフレームの保存用
            bool _footRightStep{ false };       //!< 右足音処理のフラグ
            bool _footLeftStep{ false };        //!< 左足音処理のフラグ
         };
         /**
          * \class オブジェクト射撃準備状態クラス
          * \brief オブジェクト射撃準備状態の処理を回す
          */
         class StateShootReady : public StateBase {
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
              * \brief 出口処理
              */
             void Exit()override;

         private:
            bool _changeAnim{ false };              //!< アニメーションを変えたか
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
              * \brief 更新処理
              */
             void Update() override;
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
              * \brief 更新処理
              */
             void Update() override;
         private:
             int _timeOver{ 0 };    //!< 死亡状態になってからゲームオーバーまでのフレーム数
         };
         /**
          * \class 遠隔弱攻撃射撃準備状態クラス
          * \brief 遠隔弱攻撃射撃準備状態の処理を回す
          */
         class StateWeakShootReady : public StateBase {
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
             /**
              * \brief 出口処理
              */
             void Exit() override;
         };
         /**
          * \class 装填状態クラス
          * \brief 装填状態の処理を回す
          */
         class StateReload : public StateBase {
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
              * \brief 更新処理
              */
             void Update() override;

         private:
             int _reloadCnt{ 0 };      //!< リロード状態のカウント
         };
         /**
          * \class 回復状態クラス
          * \brief 回復状態の処理を回す
          */
         class StateRecovery : public StateBase {
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
            int _recoveryCnt{ 0 };    //!< 回復状態のカウント
         };
      };
   }
}
