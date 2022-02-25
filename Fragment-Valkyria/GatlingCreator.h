#pragma once
/*****************************************************************//**
 * \file   GatlingCreator.h
 * \brief  
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "CreatorBase.h"

namespace FragmentValkyria {

   namespace Create {

      class GatlingCreator : public CreatorBase {
         public:
          GatlingCreator(Game::GameMain& gameMain);
      private:
         /**
          * \brief プレイヤーの位置、アニメーション、状態の登録を行う
          * \param game ゲーム本体クラスの参照
          * \return プレイヤーのインスタンス
          */
         virtual std::unique_ptr<Object::ObjectBase> Create() override;

      };
   }
}
