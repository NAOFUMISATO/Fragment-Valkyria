#pragma once
/*****************************************************************//**
 * \file   ParamPlayer.h
 * \brief  プレイヤーの値管理クラス
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
       * \class プレイヤーの値管理クラス
       * \brief プレイヤーの値の読み込み、取得を行う
       */
      const class ParamPlayer :public AppFrame::Param::ParamBase {
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体クラスの参照
          * \param key jsonファイル名と一致する文字列
          */
         ParamPlayer(Game::GameMain& gameMain, std::string_view key);
      };
   }
}
