#pragma once
/*****************************************************************//**
 * \file   ModeBase.h
 * \brief  各モードの基底クラス
 * 
 * \author NAOFUMISATO
 * \date   January 2022
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
    * \brief モード関係
    */
   namespace Mode {
      class ModeBase : public AppFrame::Mode::ModeBaseRoot {
      public:
         ModeBase(Game::GameMain& gameMain);

      protected:
         Game::GameMain& _gameMain; //!< ゲームクラスの参照
      };
   }
}
