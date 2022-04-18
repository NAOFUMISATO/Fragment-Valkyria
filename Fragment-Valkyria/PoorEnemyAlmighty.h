#pragma once
/*****************************************************************//**
 * \file   PoorEnemyAlmighty.h
 * \brief  全ての攻撃を行う雑魚敵クラス
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "PoorEnemyBase.h"
/**
 * \brief プロジェクト名
 */
namespace FragmentValkyria {
   /**
    * \brief 敵関係
    */
   namespace Enemy {
      /**
       * \class 全ての攻撃を行う雑魚敵クラス
       * \brief 全ての攻撃を行う雑魚敵の処理を行う
       */
      class PoorEnemyAlmighty :public PoorEnemyBase {
         using Vector4 = AppFrame::Math::Vector4;
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体クラスの参照
          */
         PoorEnemyAlmighty(Game::GameMain& gameMain);
         /**
          * \brief 初期化処理
          */
         void Init()override;

      private:
         /**
          * \brief 突進攻撃の処理
          * \param moved 突進する方向の正規化ベクトル
          */
         void Rush(const Vector4& moved);
         /**
          * \brief ガトリング弾の生成処理
          */
         void CreateGatling();

         Vector4 _gatlingMoveDirection{ Vector4(0.0, 0.0, 0.0) }; //!< ガトリング弾を放つ正規化ベクトル

      public:
         /**
          * \class 突進攻撃状態クラス
          * \brief 突進攻撃状態の処理を行う
          */
         class StateRush : public StateBase {
         public:
            /**
             * \brief コンストラクタ
             * \param owner 近接攻撃タイプの雑魚敵クラスの参照
             */
            StateRush(PoorEnemyAlmighty& owner) : StateBase{ owner }, _owner{ owner } {};
            /**
             * \brief 入口処理
             */
            void Enter() override;
            /**
             * \brief 更新処理
             */
            void Update() override;

         private:
            PoorEnemyAlmighty& _owner;    //!< 近接攻撃タイプの雑魚敵クラスの参照
            Vector4 _moved{ 0,0,0 };   //!< 突進する方向の正規化ベクトル
         };
         /**
         * \class 移動状態クラス
         * \brief 移動状態の処理を回す
         */
         class StateGatling : public StateBase {
         public:
            /**
             * \brief コンストラクタ
             * \param owner ガトリング攻撃をしてくる雑魚敵の参照
             */
            StateGatling(PoorEnemyAlmighty& owner) : StateBase{ owner }, _owner{ owner } {};
            /**
             * \brief 入口処理
             */
            void Enter() override;
            /**
             * \brief 更新処理
             */
            void Update() override;

         private:
            PoorEnemyAlmighty& _owner;     //!< 遠隔攻撃タイプの雑魚敵クラスの参照
            int _remainingGatiling{ 5 };   //!< ガトリングの残弾数
         };
      };
   }
}
