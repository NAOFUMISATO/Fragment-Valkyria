#pragma once
/*****************************************************************//**
 * \file   LaserCreator.h
 * \brief  レーザー生成管理クラス
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "CreatorBase.h"
/**
 * \brief  プロジェクト名
 */
namespace FragmentValkyria {
   /**
    * \brief  オブジェクト生成関係
    */
   namespace Create {
      /**
       * \class レーザー生成管理クラス
       * \brief レーザーの位置、状態の登録を行う
       */
      class LaserCreator : public CreatorBase
      {
         using Vector4 = AppFrame::Math::Vector4;
      public:
         /**
          * \brief コンストラクタ
          */
         LaserCreator() {};
      private:
         /**
         * \brief レーザーの位置、状態の登録を行う
         * \return レーザーのインスタンス
         */
         virtual std::unique_ptr<Object::ObjectBase> Create() override;
      };
   }
}
