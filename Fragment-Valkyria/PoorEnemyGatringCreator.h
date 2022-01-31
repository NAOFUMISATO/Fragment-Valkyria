#pragma once
/*****************************************************************//**
 * \file   PoorEnemyCreator.h
 * \brief  �K�g�����O�U�������Ă���G���G�̐����N���X
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/

#include "CreatorBase.h"

namespace FragmentValkyria {

	namespace Create {

		class PoorEnemyGatringCreator : public CreatorBase
		{
		public:
			PoorEnemyGatringCreator(Game::GameMain& gameMain);

		private:

			virtual std::unique_ptr<Object::ObjectBase> Create() override;

		};

	}
}
