#pragma once
/*****************************************************************//**
 * \file   ParamGatling.h
 * \brief  ガトリングの値管理クラス
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
       * \class ガトリングの値管理クラス
       * \brief ガトリングの値の読み込み、取得を行う
       */
      const class ParamGatling :public AppFrame::Param::ParamBase {
      public:
         /**
          * \brief コンストラクタ
          * \param key jsonファイル名と一致する文字列
          */
         ParamGatling(std::string_view key);
      };
   }
}
