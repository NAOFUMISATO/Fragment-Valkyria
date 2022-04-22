#pragma once
/*****************************************************************//**
 * \file   ParamModeClear.h
 * \brief  クリア画面の値管理クラス
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
       * \brief クリア画面の値の読み込み、取得を行う
       * \class クリア画面の値管理クラス
       */
      const class ParamModeClear :public AppFrame::Param::ParamBase {
      public:
         /**
          * \brief コンストラクタ
          * \param key jsonファイル名と一致する文字列
          */
         ParamModeClear(std::string_view key);
      };
   }
}
