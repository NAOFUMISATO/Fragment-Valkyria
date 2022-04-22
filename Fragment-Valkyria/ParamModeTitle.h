#pragma once
/*****************************************************************//**
 * \file   ParamModeTitle.h
 * \brief  タイトル画面の値管理クラス
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
       * \class タイトル画面の値管理クラス
       * \brief タイトル画面の値の読み込み、取得を行う
       */
      const class ParamModeTitle :public AppFrame::Param::ParamBase {
      public:
         /**
          * \brief コンストラクタ
          * \param key jsonファイル名と一致する文字列
          */
         ParamModeTitle(std::string_view key);
      };
   }
}
