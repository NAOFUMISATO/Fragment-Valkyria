#pragma once
/*****************************************************************//**
 * \file   ParamModeOption.h
 * \brief   オプション画面の値管理クラス
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
       * \class オプション画面の値管理クラス
       * \brief オプション画面の値の読み込み、取得を行う
       */
      const class ParamModeOption :public AppFrame::Param::ParamBase {
      public:
         /**
          * \brief コンストラクタ
          * \param key jsonファイル名と一致する文字列
          */
         ParamModeOption(std::string_view key);
      };
   }
}
