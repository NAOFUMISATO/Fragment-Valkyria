#pragma once
/*****************************************************************//**
 * \file   PoorEnemyMeleeCreator.h
 * \brief  近接攻撃をしてくる雑魚敵の生成管理クラス
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "CreatorBase.h"
/**
 * \brief プロジェクト名
 */
namespace FragmentValkyria {
   /**
    * \brief オブジェクト生成関係
    */
   namespace Create {
      /**
       * \class 近接攻撃をしてくる雑魚敵の生成管理クラス
       * \brief 近接攻撃をしてくる雑魚敵のアニメーション、状態の登録を行う
       */
      class PoorEnemyMeleeCreator : public CreatorBase {
      public:
         /**
          * \brief コンストラクタ
          */
         PoorEnemyMeleeCreator() {};

      private:
         /**
          * \brief 近接攻撃をしてくる雑魚敵のアニメーション、状態の登録を行う
          * \return 近接攻撃をしてくる雑魚敵のインスタンス
          */
         virtual std::unique_ptr<Object::ObjectBase> Create() override;
      };
   }
}