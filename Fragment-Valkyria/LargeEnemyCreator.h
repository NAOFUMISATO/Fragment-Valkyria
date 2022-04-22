#pragma once
/*****************************************************************//**
 * \file   LargeEnemyCreator.h
 * \brief  ラージエネミーの生成管理クラス
 * 
 * \author AHMD2000
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
       * \class ラージエネミーの生成管理クラス
       * \brief ラージエネミーの位置、アニメーション、状態の登録を行う
       */
      class LargeEnemyCreator : public CreatorBase {
      public:
         /**
          * \brief コンストラクタ
          */
         LargeEnemyCreator() {};
      private:
         /**
          * \brief ラージエネミーの位置、アニメーション、状態の登録を行う
          * \return ラージエネミーのインスタンス
          */
         virtual std::unique_ptr<Object::ObjectBase> Create() override;
      };
      }
}
