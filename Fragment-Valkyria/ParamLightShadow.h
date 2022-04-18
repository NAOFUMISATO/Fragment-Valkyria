#pragma once
/*****************************************************************//**
 * \file   ParamLightShadow.h
 * \brief  ライティング及び落ち影の処理を行うクラス
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
       * \brief ライティング及び落ち影の処理を行うクラスの値の読み込み、取得を行う
       * \class ライティング及び落ち影の処理を行うクラスの値管理クラス
       */
      const class ParamLightShadow :public AppFrame::Param::ParamBase {
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体クラスの参照
          * \param key jsonファイル名と一致する文字列
          */
         ParamLightShadow(Game::GameMain& gameMain, std::string_view key);
      };
   }
}
