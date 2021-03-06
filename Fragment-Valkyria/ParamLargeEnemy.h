#pragma once
/*****************************************************************//**
 * \file   ParamLargeEnemy.h
 * \brief  ボスの値管理クラス
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
       * \brief ボスの値の読み込み、取得を行う
       * \class ボスの値管理クラス
       */
      const class ParamLargeEnemy :public AppFrame::Param::ParamBase {
      public:
         /**
          * \brief ボスの値の読み込み、取得を行う
          * \class ボスの値管理クラス
          */
         ParamLargeEnemy(std::string_view key);
      };
   }
}
