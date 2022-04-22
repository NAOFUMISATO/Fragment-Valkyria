#pragma once
/*****************************************************************//**
 * \file   ParamFallObject.h
 * \brief  落下オブジェクトの値管理クラス
 * 
 * \author NAOFUMISATO
 * \date   April 2022
 *********************************************************************/
#include "AppFrame.h"
/**
 * \brief プロジェクト名
 */
namespace FragmentValkyria {
   /**
    * \brief 値管理関係
    */
   namespace Param {
      /**
       * \class 落下オブジェクトの値管理クラス
       * \brief 落下オブジェクトの値の読み込み、取得を行う
       */
      const class ParamFallObject :public AppFrame::Param::ParamBase {
      public:
         /**
          * \brief 落下オブジェクトの値の読み込み、取得を行う
          * \class 落下オブジェクトの値管理クラス
          */
         ParamFallObject(std::string_view key);
      };
   }
}
