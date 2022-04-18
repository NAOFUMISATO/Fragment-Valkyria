#pragma once
/*****************************************************************//**
 * \file   ParamLargeEnemyUI.h
 * \brief  ボスUIの値管理クラス
 * 
 * \author NAOFUMISATO
 * \date   April 2022
 *********************************************************************/
#include "AppFrame.h"
/**
 * \brief プロジェクト名
 */
namespace FragmentValkyria {
   // 二重インクルード防止
   namespace Game {
      class GameMain;
   }
   /**
    * \brief 値管理関係
    */
   namespace Param {
      /**
       * \brief ボスUIの値の読み込み、取得を行う
       * \class ボスUIの値管理クラス
       */
      const class ParamLargeEnemyUI :public AppFrame::Param::ParamBase {
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体クラスの参照
          * \param key jsonファイル名と一致する文字列
          */
         ParamLargeEnemyUI(Game::GameMain& gameMain, std::string_view key);
      };
   }
}
