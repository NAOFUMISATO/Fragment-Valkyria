#pragma once
/*****************************************************************//**
 * \file   Gatling.h
 * \brief  ガトリング攻撃の弾の処理を回すクラス
 * 
 * \author YUSUKEAHAMEDO
 * \date   January 2022
 *********************************************************************/
#include "ObjectBase.h"
#include "EffectGatlingBullet.h"
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
       * \class ガトリング攻撃の弾の処理を回すクラス
       * \brief ガトリング攻撃の弾の処理を回す
       */
      class Gatling : public Object::ObjectBase
      {
         using Vector4 = AppFrame::Math::Vector4;
         using InputManager = AppFrame::Input::InputManager;
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体の参照
          */
         Gatling(Game::GameMain& gameMain);
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
         * \brief 更新処理
         */
         virtual void Update() override;
         /**
         * \brief 描画処理
         */
         void Draw() override;
         /**
          * \brief ガトリング弾エフェクトの参照の取得
          * \return ガトリング弾エフェクトの参照
          */
         Effect::EffectGatlingBullet& efcBullet() const { return *_efcGatling; }

      private:
         /**
          * \brief 移動処理
          */
         void Move();
         /**
          * \brief 落下オブジェクトのモデルと当たっているか確認
          */
         void HitCheckFromObjectModel();
         /**
          * \brief プレイヤーと当たっているか確認
          */
         void HitCheckFromPlayer();
         /**
          * \brief ステージ外にいるか確認
          */
         void OutStageCheck();

         std::unique_ptr<Param::ParamCollision> _collParam;         //!< 当たり判定クラスの値管理クラスのポインタ
         std::unique_ptr<Effect::EffectGatlingBullet> _efcGatling;  //!< エフェクトガトリング弾のユニークポインタ
         Vector4 _moved{ Vector4(0.0, 0.0, 0.0) };                  //!< 移動量のベクトル
         Vector4 _moveDirection{ Vector4(0.0, 0.0, 0.0) };          //!< 移動する方向のベクトル

      public:
         /**
         * \class ガトリングの状態の基底クラス
         * \brief 各ガトリングの状態はこれを派生して定義する
         */
         class StateBase : public AppFrame::State::StateBaseRoot {
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
         class StateChase : public StateBase {
         public:
            /**
             * \brief コンストラクタ
             * \param owner プレイヤーの参照
             */
            StateChase(Gatling& owner) : StateBase{ owner } {};
            /**
             * \brief 入口処理
             */
            void Enter()override;
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
             * \param owner プレイヤーの参照
             */
            StateDie(Gatling& owner) : StateBase{ owner } {};
            /**
             * \brief 更新処理
             */
            void Update() override;
            /**
             * \brief 描画処理
             */
            void Draw() override;
         };

      };
   }
}
