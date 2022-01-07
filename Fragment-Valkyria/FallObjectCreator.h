#pragma once
/*****************************************************************//**
 * \file   FallObjectCreator.h
 * \brief  
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "CreatorBase.h"

namespace FragmentValkyria {

	namespace Create {

		class FallObjectCreator : public CreatorBase {
		public:
			/**
			 * \brief ゲームメインクラスの参照を得る
			 * \param gameMain ゲームメインクラスの参照
			 */
			FallObjectCreator(Game::GameMain& gameMain);

		private:
			/**
			* \brief プレイヤーの位置、アニメーション、状態の登録を行う
			* \param game ゲーム本体クラスの参照
			* \return プレイヤーのインスタンス
			*/
			virtual std::unique_ptr<Object::ObjectBase> Create() override;

			std::unique_ptr<Object::ObjectBase> _fallObject;
		};
	}
}
