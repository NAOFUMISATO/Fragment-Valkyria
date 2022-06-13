#pragma once
/*****************************************************************//**
 * \file   PlayerCreator.h
 * \brief  プレイヤーの生成管理クラス
 *
 * \author NAOFUMISATO, YUSUKEAHAMEDO
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
      class PlayerCreator : public CreatorBase {
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体の参照
          */
          PlayerCreator(Game::GameMain& gameMain);
      private:
         /**
          * \brief カメラの位置、状態とプレイヤーの位置、アニメーション、状態の登録を行う
          * \return プレイヤーのインスタンス
          */
         virtual std::unique_ptr<Object::ObjectBase> Create() override;
      };
   }
}