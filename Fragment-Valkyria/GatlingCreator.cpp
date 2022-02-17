
/*****************************************************************//**
 * \file   GatlingCreator.cpp
 * \brief  
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "GatlingCreator.h"
#include "Gatling.h"
#include "GameMain.h"

using namespace FragmentValkyria::Create;
using namespace FragmentValkyria;

GatlingCreator::GatlingCreator(Game::GameMain& gameMain) : CreatorBase{ gameMain } {

}

std::unique_ptr<Object::ObjectBase> GatlingCreator::Create() {
	auto gatling = std::make_unique<Enemy::Gatling>(_gameMain);
	auto startPos = _gameMain.objServer().GetVecData("GatlingPos");
	gatling->position(startPos);
	gatling->Init();

	auto state = std::make_unique<AppFrame::State::StateServer>("Chase", std::make_shared<Enemy::Gatling::StateChase>(*gatling));
	state->Register("Die", std::make_shared<Enemy::Gatling::StateDie>(*gatling));
	gatling->stateServer(std::move(state));

	return std::move(gatling);
}
