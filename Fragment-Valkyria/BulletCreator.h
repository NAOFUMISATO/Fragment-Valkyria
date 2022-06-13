#pragma once
/*****************************************************************//**
 * \file   BulletCreator.h
 * \brief  遠隔弱攻撃の生成管理クラス
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
       * \class 遠隔弱攻撃の生成管理クラス
       * \brief 遠隔弱攻撃の位置、状態の登録を行う
       */
      class BulletCreator : public CreatorBase {
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体の参照
          */
         BulletCreator(Game::GameMain& gameMain);

      private:
         /**
          * \brief 遠隔弱攻撃の位置、状態の登録を行う
          * \return 遠隔弱攻撃のインスタンス
          */
         virtual std::unique_ptr<Object::ObjectBase> Create() override;
      };
   }
}
