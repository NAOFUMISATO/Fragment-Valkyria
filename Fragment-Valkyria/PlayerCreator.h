#pragma once
/*****************************************************************//**
 * \file   PlayerCreator.h
 * \brief  プレイヤーの生成管理クラス
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
       * \class プレイヤーの生成管理クラス
       * \brief プレイヤーの位置、アニメーション、状態の登録を行う
       */
      class PlayerCreator :public CreatorBase {
         /**
          * \brief プレイヤーの位置、アニメーション、状態の登録を行う
          * \param game ゲーム本体クラスの参照
          * \return プレイヤーのインスタンス
          */
         virtual std::unique_ptr<Object::ObjectBase> Create(Game::GameMain& game);
      };
   }
}