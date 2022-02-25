#pragma once
/*****************************************************************//**
 * \file   LaserCreator.h
 * \brief  レーザー生成管理クラス
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/

#include "CreatorBase.h"

namespace FragmentValkyria {

   namespace Create {

      class LaserCreator : public CreatorBase
      {
         using Vector4 = AppFrame::Math::Vector4;
      public:
         LaserCreator(Game::GameMain& gameMain);
      private:
         /**
         * \brief レーザーsの位置、アニメーション、状態の登録を行う
         * \param game ゲーム本体クラスの参照
         * \return レーザーのインスタンス
         */
         virtual std::unique_ptr<Object::ObjectBase> Create() override;

      };
   }
}
