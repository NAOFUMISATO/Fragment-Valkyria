#pragma once
/*****************************************************************//**
 * \file   PoorEnemyBase.h
 * \brief  雑魚敵の基底クラス
 * 
 * \author NAOFUMISATO
 * \date   February 2022
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
       * \class 雑魚敵の基底クラス
       * \brief 各雑魚敵はこのクラスを派生して定義する
       */
      class PoorEnemyBase :public Object::ObjectBase {
         using Vector4 = AppFrame::Math::Vector4;
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体クラスの参照
          */
         PoorEnemyBase(Game::GameMain& gameMain);
         /**
          * \brief 初期化処理
          */
         void Init()override;
         /**
          * \brief 更新処理
          */
         void Update()override;
         /**
          * \brief 描画処理
          */
         void Draw()override;
         /**
          * \brief オブジェクト種別の取得
          * \return 種別:PoorEnemy
          */
         virtual ObjectType GetObjType() const override { return ObjectType::PoorEnemy; }
         
      protected:
         /**
          * \brief 回転処理
          */
         void Rotate();
         /**
          * \brief プレイヤーの弱攻撃に当たった場合の処理
          */
         void HitCheckFromBullet();
         /**
          * \brief プレイヤーのオブジェクト発射攻撃に当たった場合の処理
          */
         void HitCheckFromFallObject();
         void DamageExpression();

         int _collNum{ 0 };                     //!< コリジョン番号保存用
         int _damageCnt{ 0 };                   //!< ダメージ表現のフレームカウント保存用
         double _hp{ 0.0 };                     //!< 体力
         std::vector<std::string> _actionList;  //!< 各状態の文字列を全て登録する動的配列
         std::vector<std::string> _action;      //!< 各状態の文字列を登録し、ランダムに行動させるための動的配列

      public:
         /**
          * \class 各状態の基底クラス
          * \brief 各状態はこのクラスを派生して定義する
          */
         class StateBase : public AppFrame::State::StateBaseRoot {
         public:
            /**
             * \brief コンストラクタ
             * \param owner 雑魚敵の基底クラスの参照
             */
            StateBase(PoorEnemyBase& owner) : _owner{ owner } {};
            /**
             * \brief 描画処理
             */
            virtual void Draw() override;

         protected:
            PoorEnemyBase& _owner;   //!< 雑魚敵基底クラスの参照
            int _stateCnt;           //!< 状態遷移時に使用するフレーム保存用
         };

         /**
          * \class 待機状態クラス
          * \brief 待機状態の処理を回す
          */
         class StateIdle : public StateBase{
         public:
            /**
             * \brief コンストラクタ
             * \param owner 雑魚敵の基底クラスの参照
             */
            StateIdle(PoorEnemyBase& owner) : StateBase{ owner } {};
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
          * \class サイドステップ状態クラス
          * \brief サイドステップ状態の処理を回す
          */
         class StateSideStep : public StateBase {
         public:
            /**
             * \brief コンストラクタ
             * \param owner 雑魚敵の基底クラスの参照
             */
            StateSideStep(PoorEnemyBase& owner) : StateBase{ owner } {};
            /**
             * \brief 入口処理
             */
            void Enter() override;
            /**
             * \brief 更新処理
             */
            void Update() override;

         private:
            /**
             * \brief サイドステップを行う空間があるかチェックする
             */
            void SideStepDecide();

            Vector4 _moveOnDir{ 0,0,0 };   //!< ステップ方向に正規化したベクトルの保存用
            Vector4 _moveOnPos{ 0,0,0 };   //!< ステップ後の最終位置ベクトル保存用
         };

         /**
          * \class 落下状態クラス
          * \brief 落下状態の処理を回す
          */
         class StateFall : public StateBase{
         public:
            /**
             * \brief コンストラクタ
             * \param owner 雑魚敵の基底クラスの参照
             */
            StateFall(PoorEnemyBase& owner) : StateBase{ owner } {};
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
         class StateDie : public StateBase{
         public:
            /**
             * \brief コンストラクタ
             * \param owner 雑魚敵の基底クラスの参照
             */
            StateDie(PoorEnemyBase& owner) : StateBase{ owner } {};
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
            void Draw()override;

         private:
            float _opacityRate{ 1.0f }; //!< 不透明度
            int _timeOver{ 0 };         //!< 死亡モーション後から消滅するまでの猶予フレーム
         };
      };
   }
}
