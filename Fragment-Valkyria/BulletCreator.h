#pragma once
/*****************************************************************//**
 * \file   BulletCreator.h
 * \brief  遠隔弱攻撃の生成管理クラス
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "CreatorBase.h"

namespace FragmentValkyria {

	namespace Create {

		class BulletCreator : public CreatorBase {
		public:
			BulletCreator(Game::GameMain& gameMain);

		private:
			/**
		    * \brief 遠隔弱攻撃の位置、アニメーション、状態の登録を行う
		    * \param game ゲーム本体クラスの参照
		    * \return 遠隔弱攻撃のインスタンス
		    */
			virtual std::unique_ptr<Object::ObjectBase> Create() override;
		};
	}
}
