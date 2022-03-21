#pragma once
/*****************************************************************//**
 * \file   PoorEnemyAlmightyCreator.h
 * \brief  全ての攻撃を行ってくる雑魚敵の生成管理クラス
 * 
 * \author NAOFUMISATO
 * \date   March 2022
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
       * \class 全ての攻撃を行ってくる雑魚敵の生成クラス
       * \brief 全ての攻撃を行ってくる雑魚敵のアニメーション、状態の登録を行う
       */
      class PoorEnemyAlmightyCreator : public CreatorBase {
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体の参照
          */
         PoorEnemyAlmightyCreator(Game::GameMain& gameMain);

      private:
         /**
          * \brief 全ての攻撃を行ってくる雑魚敵のアニメーション、状態の登録を行う
          * \return 全ての攻撃を行ってくる雑魚敵のインスタンス
          */
         virtual std::unique_ptr<Object::ObjectBase> Create() override;
      };
   }
}