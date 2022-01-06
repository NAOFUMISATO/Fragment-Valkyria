#pragma once
/*****************************************************************//**
 * \file   LargeEnemyCreator.h
 * \brief  
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "CreatorBase.h"

namespace FragmentValkyria {

	namespace Create {
		class LargeEnemyCreator : public CreatorBase{

			virtual std::unique_ptr<Object::ObjectBase> Create(Game::GameMain& game) override;

		};

		}
}
