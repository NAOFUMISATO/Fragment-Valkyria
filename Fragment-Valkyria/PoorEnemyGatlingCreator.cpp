
/*****************************************************************//**
 * \file   PoorEnemyGatlingCreator.cpp
 * \brief  ƒKƒgƒŠƒ“ƒOUŒ‚‚ğ‚µ‚Ä‚­‚éG‹›“G‚Ì¶¬
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "PoorEnemyGatlingCreator.h"
#include "PoorEnemyGatling.h"
#include "ModelAnimeComponent.h"

using namespace FragmentValkyria;
using namespace FragmentValkyria::Create;

PoorEnemyGatlingCreator::PoorEnemyGatlingCreator(Game::GameMain& gameMain) : CreatorBase{ gameMain } {

}

std::unique_ptr<Object::ObjectBase> PoorEnemyGatlingCreator::Create() {

   auto poorEnemyGatling = std::make_unique<Enemy::PoorEnemyGatling>(_gameMain);

   auto model = std::make_unique<Model::ModelAnimeComponent>(*poorEnemyGatling);
   model->Init();
   model->SetModel("PoorEnemyGatling", 1000);
   poorEnemyGatling->modelAnimeComponent(std::move(model));

   auto state = std::make_unique<AppFrame::State::StateServer>("Fall", std::make_shared <Enemy::PoorEnemyGatling::StateFall>(*poorEnemyGatling));
   state->Register("Idle", std::make_shared<Enemy::PoorEnemyGatling::StateIdle>(*poorEnemyGatling));
   state->Register("Gatling", std::make_shared<Enemy::PoorEnemyGatling::StateGatling>(*poorEnemyGatling));
   state->Register("SideStep", std::make_shared<Enemy::PoorEnemyGatling::StateSideStep>(*poorEnemyGatling));
   state->Register("Die", std::make_shared<Enemy::PoorEnemyGatling::StateDie>(*poorEnemyGatling));
   poorEnemyGatling->stateServer(std::move(state));

   return std::move(poorEnemyGatling);
}
