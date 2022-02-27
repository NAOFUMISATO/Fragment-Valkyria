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
          * \brief レーザーを打つ位置を設定
          */
         void SetLaserPosition();
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
          * \brief 各行動への移動処理
          */
         void Action();
         /**
          * \brief 角速度による回転処理
          * \param rotating 回転するか
          */
         void AugularRotate(bool& rotating);
         /**
          * \brief 面積による回転
          */
         void AreaRotate(bool& rotating);
         /**
          * \brief 回転の角速度の設定
          */
         void SetAddRotate();

         int _gatlingCnt{ 0 };                                      //!< ガトリングの弾を打つ回数
         int _collision{ 0 };                                       //!< モデルのコリジョンフレーム番号
         int _freezeTime{ 0 };                                      //!< 死亡してからゲームクリアまでのフレーム数
         bool _firstRotating{ true };                               //!< 移動中最初に移動方向に回転するか
         bool _endRotating{ true };                                 //!< 移動中最後にプレイヤーの方向に回転するか
         bool _rotating{ false };                                   //!< 回転処理をするか
         bool _attack{ false };                                     //!< 攻撃をしているか
         double _fanAngle{ 0.0 };                                   //!< 扇状ガトリング攻撃をするときの向きを取得するときベクトルを回転させる角度
         double _rotateDot{ 0.0 };                                  //!< 向かせたい方向のベクトルとフォワードベクトルを90度回転させたベクトルの内積の結果
         double _addRotate{ 0.0 };                                  //!< 角速度
         double _hp{ 0.0 };                                         //!< ヒットポイント
         int _createNum{ 0 };                                       //!< 生成されている落下オブジェクトの数
         Vector4 _moved{ 0.0, 0.0, 0.0 };                           //!< 移動量のベクトル
         Vector4 _rotateDir{ 0.0, 0.0, 0.0 };                       //!< 回転の向きのベクトル
         std::vector<std::pair<double, Vector4>> _objectDistance;   //!< 各落下オブジェクトからプレイヤーへの距離の2乗と各落下オブジェクトの位置のペアの動的配列
         std::vector<std::string> _actionList;                      //!< 各行動状態への文字列の動的配列
         std::vector<std::string> _action;                          //!< 行動させる範囲を制限した行動状態への文字列の動的配列
      public:
         /**
         * \class ラージエネミーの状態の基底クラス
         * \brief 各ラージエネミーの状態はこれを派生して定義する
         */
         class StateBase : public AppFrame::State::StateBaseRoot {
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
            int _stateCnt{ 0 };   //!< 各状態へ入った時のフレームカウント保存用
         };
         /**
         * \class 待機状態クラス
         * \brief 待機状態の処理を回す
         */
         class StateIdle : public StateBase {
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
             * \brief 更新処理
             */
            void Update() override;
         };
         /**
         * \class オブジェクト落下状態クラス
         * \brief オブジェクト落下状態の処理を回す
         */
         class StateFallObject : public StateBase {
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
             * \brief 更新処理
             */
            void Update() override;
         };
         /**
         * \class ガトリング攻撃状態クラス
         * \brief ガトリング攻撃状態の処理を回す
         */
         class StateGatling : public StateBase {
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
         private:
            int _gatlingFrameCnt{ 0 };      //!< ガトリング攻撃をするための回転していない時に経過させるフレームカウント
         };
         /**
         * \class 扇状ガトリング攻撃状態クラス
         * \brief 扇状ガトリング攻撃状態の処理を回す
         */
         class StateFanGatling : public StateBase {
         public:
            /**
             * \brief コンストラクタ
             * \param owner ラージエネミーの参照
             */
            StateFanGatling(LargeEnemy& owner) : StateBase{ owner } {};
            /**
             * \brief 入口処理
             */
            void Enter() override;
            /**
             * \brief 更新処理
             */
            void Update() override;
         private:
            int _fanGatlingFrameCnt{ 0 };   //!< 扇状ガトリング攻撃をするための回転をしていない時に経過させるフレームカウント
         };
         /**
         * \class 死状態クラス
         * \brief 死亡状態の処理を回す
         */
         class StateDie : public StateBase {
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
         class StateMove : public StateBase {
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
            bool _footLeftStep{ false };        //!< 足音処理のフラグ
            int _footCnt{ 0 };                  //!< 
            bool _endGetplyPos{ true };         //!< 移動後にプレイヤーへの向きのベクトルを取得するか
         };
         /**
         * \class レーザー攻撃状態クラス
         * \brief レーザー攻撃状態の処理を回す
         */
         class StateLaser : public StateBase {
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
            bool _createLaser{ false };       //!< レーザーを生成するか
         };
      };
   }
}
