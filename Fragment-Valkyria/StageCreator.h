#pragma once
/*****************************************************************//**
 * \file   StageCreator.h
 * \brief  ステージの生成管理クラス
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "CreatorBase.h"
 /**
  * \brief プロジェクト名
  */
namespace FragmentValkyria {
   /**
    * \brief オブジェクト生成関係
    */
   namespace Create {
      /**
       * \class ステージの生成管理クラス
       * \brief スカイスフィア及び地面の生成を行う
       */
      class StageCreator : public CreatorBase {
      public:
          
          StageCreator(Game::GameMain& gameMain);

      private:
         /**
          * \brief スカイスフィア及び地面の生成を行う
          * \param gameMain ゲーム本体クラスの参照
          * \return ステージのインスタンス
          */
         virtual std::unique_ptr<Object::ObjectBase> Create();
      };
   }
}
