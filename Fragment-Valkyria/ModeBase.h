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
   namespace Object {
      class ObjectServer;
   }
   namespace Sprite {
      class SpriteServer;
   }
   /**
    * \brief モード関係
    */
   namespace Mode {
      /**
       * \class 各モードの基底クラス
       * \brief 各モードはこのクラスを派生して定義する
       */
      class ModeBase : public AppFrame::Mode::ModeBaseRoot {
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体クラスの参照
          */
         ModeBase(Game::GameMain& gameMain);
         /**
          * \brief オブジェクト一括管理クラスの参照をゲームベース経由で取得
          * \return オブジェクト一括管理クラスの参照
          */
         Object::ObjectServer& GetObjServer() const;
         /**
          * \brief スプライト一括管理クラスの参照をゲームベース経由で取得
          * \return スプライト一括管理クラスの参照
          */
         Sprite::SpriteServer& GetSprServer() const;

      protected:
         Game::GameMain& _gameMain; //!< ゲームクラスの参照
      };
   }
}
