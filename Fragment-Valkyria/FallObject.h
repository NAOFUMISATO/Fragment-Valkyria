#pragma once
/*****************************************************************//**
 * \file   FallObject.h
 * \brief  落下オブジェクトの処理を回すクラス
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "ObjectBase.h"
#include "EffectObjectUp.h"
#include "ParamCollision.h"
/**
 * \brief プロジェクト名
 */
namespace FragmentValkyria {
   /**
    * \brief 敵関係
    */
   namespace Enemy {
      /**
       * \class 落下オブジェクトの処理を回すクラス
       * \brief 落下オブジェクトの処理を回す
       */
      class FallObject : public Object::ObjectBase {
         using InputManager = AppFrame::Input::InputManager;
         using Vector4 = AppFrame::Math::Vector4;
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体の参照
          */
         FallObject(Game::GameMain& gameMain);
         /**
          * \brief 初期化処理
          */
         virtual void Init() override;
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
         /**
          * \brief オブジェクトの種類を返す
          * \return 種別:落下オブジェクト
          */
         ObjectType GetObjType() const override { return ObjectType::FallObject; };
         /**
          * \brief 残留オブジェクトか確認
          * \return 残留オブジェクトかどうか
          */
         bool residual() { return _residual; }
         /**
          * \brief 残留オブジェクトかどうか設定
          * \param residual 残留オブジェクトかどうか
          */
         void residual(bool residual) { _residual = residual; }
         /**
          * \brief カプセルを形成する一つ目の座標の取得
          * \return カプセルを形成する一つ目の座標
          */
         Vector4 capsulePos1() { return _capsulePos1; }
         /**
          * \brief カプセルを形成する二つ目の座標の取得
          * \return カプセルを形成する二つ目の座標
          */
         Vector4 capsulePos2() { return _capsulePos2; }
         /**
          * \brief モデルのコリジョンフレームの名前の設定
          * \param collision モデルのコリジョンフレームの名前
          */
         void collisionName(std::string_view collision) { _collisionName = collision; }
         /**
          * \brief モデルのコリジョンフレームの名前の取得
          * \return モデルのコリジョンフレームの名前
          */
         std::string_view collisionName() { return _collisionName; }

      private:
         /**
          * \brief プレイヤーが浮く範囲内にいるか確認
          */
         void HitCheckFromPlayerPoint();
         /**
          * \brief ラージエネミーのモデルと当たっているか確認
          */
         void HitCheckFromLargeEnemy();
         /**
          * \brief レーザーと当たっているか確認
          */
         void HitCheckFromLaser();
         /**
          * \brief ガトリング攻撃をしてくる雑魚敵と当たっているか確認
          */
         void HitCheckFromPoorEnemy();
         /**
          * \brief ステージ外にいるか確認
          */
         void OutStageCheck();
         /**
          * \brief プレイヤーがノックバックしているか確認
          */
         void CheckPlayerKnockBack();
         /**
          * \brief 浮かせながら回転させる処理
          */
         void Save();
         /**
          * \brief 上に上げていく処理
          */
         void Up();
         /**
          * \brief 打った時の注視点に向かって進む処理
          */
         void Shoot();
         /**
          * \brief カプセルの位置の更新
          */
         void SetCapsulePos();

         std::unique_ptr<Param::ParamCollision> _collParam;        //!< 当たり判定クラスの値管理クラスのポインタ
         std::unique_ptr<Effect::EffectObjectUp> _efcUp;           //!< オブジェクト上昇クラスのポインタ
         double _fallTimer{ 0.0 };                                 //!< 落下状態の進捗
         double _upDownAngle{ 0.0 };                               //!< ふわふわさせる時のサインの値を取るときの角度
         double _rotateAngle{ 0.0 };                               //!< 不規則な回転さをせる時のサインの値を取るときの角度
         bool _saveFlag{ false };                                  //!< ふわふわ浮かせるか
         bool _isFall{ true };                                     //!< 落下状態かどうか
         bool _residual{ true };                                   //!< 残留オブジェクトか
         bool _efcBorn{ false };                                   //!< エフェクトは出現したか
         int _collision{ 0 };                                      //!< モデルのコリジョンフレーム番号
         Vector4 _shootVec{ Vector4(0.0, 0.0, 0.0) };              //!< 打った時の注視点へ向かうベクトル
         Vector4 _plyRightHandVec{ Vector4(0.0, 0.0, 0.0) };       //!< 打った時の注視点へ向かうベクトル
         Vector4 _capsulePos1{ Vector4(0.0, 0.0, 0.0) };           //!< カプセルを形成する一つ目の位置
         Vector4 _capsulePos2{ Vector4(0.0, 0.0, 0.0) };           //!< カプセルを形成する二つ目の位置
         std::string_view _collisionName{ "" };                    //!< モデルのコリジョンフレームの名前
         std::vector<int> _fallPointHandles{ -1 };                 //!< 落下地点ビルボード画像ハンドル

      public:
         /**
         * \class 落下オブジェクトの状態の基底クラス
         * \brief 各落下オブジェクトの状態はこれを派生して定義する
         */
         class StateBase : public AppFrame::State::StateBaseRoot {
         public:
            /**
             * \brief コンストラクタ
             * \param owner 落下オブジェクトの参照
             */
            StateBase(FallObject& owner) : _owner{ owner } {};
            /**
             * \brief 描画処理
             */
            virtual void Draw() override;

         protected:
            FallObject& _owner;   //!< 落下オブジェクトの参照
         };
         /**
          * \class 待機状態クラス
          * \brief 待機状態の処理を回す
          */
         class StateIdle : public StateBase {
         public:
            /**
             * \brief コンストラクタ
             * \param owner 落下オブジェクトの参照
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
         class StateFall : public StateBase {
         public:
            /**
             * \brief コンストラクタ
             * \param owner 落下オブジェクトの参照
             */
            StateFall(FallObject& owner) : StateBase{ owner } {};
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
          * \class 上昇状態のクラス
          * \brief 上昇状態の処理を回す
          */
         class StateUp : public StateBase {
         public:
            /**
             * \brief コンストラクタ
             * \param owner 落下オブジェクトの参照
             */
            StateUp(FallObject& owner) : StateBase{ owner } {};
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
          * \class 浮く状態のクラス
          * \brief 浮く状態の処理を回す
          */
         class StateSave : public StateBase {
         public:
            /**
             * \brief コンストラクタ
             * \param owner 落下オブジェクトの参照
             */
            StateSave(FallObject& owner) : StateBase{ owner } {};
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
          * \class 発射状態のクラス
          * \brief 発射状態の処理を回す
          */
         class StateShoot : public StateBase {
         public:
            /**
             * \brief コンストラクタ
             * \param owner 落下オブジェクトの参照
             */
            StateShoot(FallObject& owner) : StateBase{ owner } {};
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
         class StateDie : public StateBase {
         public:
            /**
             * \brief コンストラクタ
             * \param owner 落下オブジェクトの参照
             */
            StateDie(FallObject& owner) : StateBase{ owner } {};
            /**
             * \brief 更新処理
             */
            void Update() override;
         };
      };
   }
}
