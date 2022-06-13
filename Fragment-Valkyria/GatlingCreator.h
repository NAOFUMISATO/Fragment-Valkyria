#pragma once
/*****************************************************************//**
 * \file   GatlingCreator.h
 * \brief  ガトリング攻撃の弾の生成管理クラス
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
    * \brief  オブジェクト生成関係
    */
   namespace Create {
      /**
       * \class ガトリング攻撃の弾の生成管理クラス
       * \brief ガトリング攻撃の弾の位置、状態の登録を行う
       */
      class GatlingCreator : public CreatorBase {
      public:
         /**
          * \brief コンストラクタ
          */
         GatlingCreator() {};
      private:
         /**
          * \brief ガトリング攻撃の弾の位置、状態の登録を行う
          * \return ガトリング攻撃の弾のインスタンス
          */
         virtual std::unique_ptr<Object::ObjectBase> Create() override;
      };
   }
}
