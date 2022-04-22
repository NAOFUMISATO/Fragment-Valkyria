#pragma once
/*****************************************************************//**
 * \file   PoorEnemyGatling.h
 * \brief  遠隔攻撃タイプの雑魚敵クラス
 * 
 * \author AHMD2000,NAOFUMISATO
 * \date   January 2022
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
       * \class 遠隔攻撃タイプの雑魚敵クラス
       * \brief 遠隔攻撃タイプの雑魚敵の処理を回す
       */
      class PoorEnemyGatling : public PoorEnemyBase {
         using Vector4 = AppFrame::Math::Vector4;
         using InputManager = AppFrame::Input::InputManager;
      public:
         /**
          * \brief コンストラクタ
          */
         PoorEnemyGatling();
         /**
          * \brief 初期化処理
          */
         void Init()override;

      private:
         /**
          * \brief ガトリング弾の生成処理
          */
         void CreateGatling();

         Vector4 _gatlingMoveDirection{ Vector4(0.0, 0.0, 0.0) }; //!< ガトリング弾を放つ正規化ベクトル

      public:
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
            StateGatling(PoorEnemyGatling& owner) : StateBase{ owner }, _owner{ owner } {};
            /**
             * \brief 入口処理
             */
            void Enter() override;
            /**
             * \brief 更新処理
             */
            void Update() override;

         private:
            PoorEnemyGatling& _owner;     //!< 遠隔攻撃タイプの雑魚敵クラスの参照
            int _remainingGatiling{ 5 };  //!< ガトリングの残弾数
         };
      };
   }
}
