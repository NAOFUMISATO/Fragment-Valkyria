#pragma once
/*****************************************************************//**
 * \file   ParamPoorEnemy.h
 * \brief  雑魚敵の値管理クラス
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
       * \class 雑魚敵の値管理クラス
       * \brief 雑魚敵の値の読み込み、取得を行う
       */
      const class ParamPoorEnemy :public AppFrame::Param::ParamBase {
      public:
         /**
          * \brief コンストラクタ
          * \param key jsonファイル名と一致する文字列
          */
         ParamPoorEnemy(std::string_view key);
      };
   }
}
