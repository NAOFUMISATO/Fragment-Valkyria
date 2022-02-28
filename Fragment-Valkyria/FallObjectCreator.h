#pragma once
/*****************************************************************//**
 * \file   FallObjectCreator.h
 * \brief  落下オブジェクトの生成管理クラス
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "CreatorBase.h"
#include "FallObject.h"
/**
 * \brief プロジェクト名
 */
namespace FragmentValkyria {
   /**
    * \brief オブジェクト生成関係
    */
   namespace Create {
      /**
       * \class 落下オブジェクトの生成管理クラス
       * \brief 落下オブジェクトのアニメーション、状態の登録を行う
       */
      class FallObjectCreator : public CreatorBase {
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体の参照
          */
         FallObjectCreator(Game::GameMain& gameMain);
      private:
         /**
         * \brief 落下オブジェクトのアニメーション、状態の登録を行う
         * \return 落下オブジェクトのインスタンス
         */
         virtual std::unique_ptr<Object::ObjectBase> Create() override;
      };
   }
}
