
/*****************************************************************//**
 * \file   GatlingCreator.cpp
 * \brief  
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "GatlingCreator.h"
#include "Gatling.h"

using namespace FragmentValkyria::Create;
using namespace FragmentValkyria;

GatlingCreator::GatlingCreator(Game::GameMain& gameMain) : CreatorBase{ gameMain } {

}

std::unique_ptr<Object::ObjectBase> GatlingCreator::Create() {
	auto gatling = std::make_shared<Enemy::Gatling>(_gameMain);

	return std::unique_ptr<Object::ObjectBase>();
}
