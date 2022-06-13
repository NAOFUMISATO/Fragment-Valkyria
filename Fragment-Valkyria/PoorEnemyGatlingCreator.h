#pragma once
/*****************************************************************//**
 * \file   PoorEnemyGatlingCreator.h
 * \brief  ガトリング攻撃をしてくる雑魚敵の生成管理クラス
 * 
 * \author YUSUKEAHAMEDO
 * \date   January 2022
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
       * \class ガトリング攻撃をしてくる雑魚敵の生成管理クラス
       * \brief ガトリング攻撃をしてくる雑魚敵のアニメーション、状態の登録を行う
       */
      class PoorEnemyGatlingCreator : public CreatorBase {
      public:
         /**
          * \brief コンストラクタ
          */
         PoorEnemyGatlingCreator() {};

      private:
         /**
          * \brief ガトリング攻撃をしてくる雑魚敵のアニメーション、状態の登録を行う
          * \return ガトリング攻撃をしてくる雑魚敵のインスタンス
          */
         virtual std::unique_ptr<Object::ObjectBase> Create() override;
      };
   }
}
