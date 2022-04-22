
/*****************************************************************//**
 * \file   GatlingCreator.cpp
 * \brief  ƒKƒgƒŠƒ“ƒO¶¬
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "GatlingCreator.h"
#include "Gatling.h"
#include "GameMain.h"
#include "ObjectServer.h"

using namespace FragmentValkyria::Create;
using namespace FragmentValkyria;

std::unique_ptr<Object::ObjectBase> GatlingCreator::Create() {
   auto gatling = std::make_unique<Enemy::Gatling>();
   auto gameInstance = Game::GameMain::GetInstance();
   auto startPos = gameInstance->objServer().GetVecData("GatlingPos");
   gatling->position(startPos);

   auto state = std::make_unique<AppFrame::State::StateServer>("Chase",
      std::make_shared<Enemy::Gatling::StateChase>(*gatling));
   state->Register("Die", std::make_shared<Enemy::Gatling::StateDie>(*gatling));
   gatling->stateServer(std::move(state));

   return std::move(gatling);
}
