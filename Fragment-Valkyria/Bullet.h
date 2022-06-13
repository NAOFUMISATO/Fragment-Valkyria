#pragma once
/*****************************************************************//**
 * \file   Bullet.h
 * \brief  プレイヤーの遠隔弱攻撃の弾の処理を回すクラス
 * 
 * \author YUSUKEAHAMEDO
 * \date   January 2022
 *********************************************************************/
#include "ObjectBase.h"
#include "EffectWeakBullet.h"
#include "ParamCollision.h"
/**
 * \brief プロジェクト名
 */
namespace FragmentValkyria {
   /**
    * \brief  プレイヤー関係
    */
   namespace Player {
      /**
       * \class プレイヤーの遠隔弱攻撃の弾の処理を回すクラス
       * \brief プレイヤーの遠隔弱攻撃の弾の処理を回す
       */
      class Bullet : public Object::ObjectBase {
         using Vector4 = AppFrame::Math::Vector4;
         using InputManager = AppFrame::Input::InputManager;
      public:
         /**
          * \brief コンストラクタ
          */
         Bullet();
         /**
          * \brief オブジェクトの種類を返す
          * \return 弱攻撃弾
          */
         ObjectType GetObjType() const override { return ObjectType::Bullet; };
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
         /**
          * \brief 弱攻撃弾エフェクトクラスの参照の取得
          * \return 弱攻撃弾エフェクトクラスの参照
          */
         Effect::EffectWeakBullet& efcBullet() const { return *_efcBullet; }

      private:
         /**
          * \brief 移動処理
          */
         void Move();
         /**
          * \brief ラージエネミーと当たっているか確認
          */
         void HitCheckFromLargeEnemy();
         /**
          * \brief ガトリング攻撃をしてくる雑魚敵と当たっているか確認
          */
         void HitCheckFromPoorEnemyGatling();
         /**
          * \brief ステージ外にいるか確認
          */
         void OutCheckFromStage();

         std::unique_ptr<Param::ParamCollision> _collParam;     //!< 当たり判定クラスの値管理クラスのポインタ
         std::unique_ptr<Effect::EffectWeakBullet> _efcBullet;  //!< 弾エフェクトのポインタ
         Vector4 _moved{ Vector4() };                           //!< 移動量のベクトル
#ifdef _DEBUG
         double _radius;                                        //!< デバッグ描画を行う弾の当たり判定
#endif

      public:
         /**
         * \class プレイヤーの遠隔弱攻撃の弾の状態の基底クラス
         * \brief 各プレイヤーの遠隔弱攻撃の弾の状態はこれを派生して定義する
         */
         class StateBase : public AppFrame::State::StateBaseRoot {
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
         class StateShoot : public StateBase {
         public:
            /**
             * \brief コンストラクタ
             * \param owner プレイヤーの参照
             */
            StateShoot(Bullet& owner) : StateBase{ owner } {};
            /**
             * \brief 入口処理
             */
            void Enter()override;
            /**
             * \brief 更新処理
             */
            void Update() override;
         };
      };
   }
}
