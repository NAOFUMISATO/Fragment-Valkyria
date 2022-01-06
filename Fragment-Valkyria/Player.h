#pragma once
/*****************************************************************//**
 * \file   Player.h
 * \brief  プレイヤークラス
 *
 * \author NAOFUMISATO
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

      private:
         /**
          * \brief 移動処理
          */
         virtual void Move();

         const double MoveSpeed{ GetLoadJson().GetParam("player", "movespeed") };      //!< 移動速度
         double _rotateSpeed{ 0 };       //!< 回転速度

         Vector4 _moved{ Vector4() };
         Vector4 _direction{ Vector4() };

         Matrix44 _rightRotation{ Matrix44() };
         Matrix44 _leftRotation{ Matrix44() };
         Matrix44 _backRotation{ Matrix44() };

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
            void Draw() override;

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
      };
   }
}
