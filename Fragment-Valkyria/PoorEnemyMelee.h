#pragma once
/*****************************************************************//**
 * \file   PoorEnemyMelee.h
 * \brief  近接攻撃タイプの雑魚敵クラス
 * 
 * \author NAOFUMISATO
 * \date   February 2022
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
       * \class 近接攻撃タイプの雑魚敵クラス
       * \brief 近接攻撃タイプの雑魚敵の処理を行う
       */
      class PoorEnemyMelee :public PoorEnemyBase {
         using Vector4 = AppFrame::Math::Vector4;
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体クラスの参照
          */
         PoorEnemyMelee(Game::GameMain& gameMain);
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
            StateRush(PoorEnemyMelee& owner) : StateBase{ owner }, _owner{ owner } {};
            /**
             * \brief 入口処理
             */
            void Enter() override;
            /**
             * \brief 更新処理
             */
            void Update() override;

         private:
            PoorEnemyMelee& _owner;    //!< 近接攻撃タイプの雑魚敵クラスの参照
            Vector4 _moved{ 0,0,0 };   //!< 突進する方向の正規化ベクトル
         };
      };
   }
}
