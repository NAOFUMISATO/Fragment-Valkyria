
/*****************************************************************//**
 * \file   PoorEnemyGatringCreator.cpp
 * \brief  ƒKƒgƒŠƒ“ƒOUŒ‚‚ğ‚µ‚Ä‚­‚éG‹›“G‚Ì¶¬
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "PoorEnemyGatringCreator.h"
#include "PoorEnemyGatling.h"
#include "ModelAnimeComponent.h"

using namespace FragmentValkyria;
using namespace FragmentValkyria::Create;

PoorEnemyGatringCreator::PoorEnemyGatringCreator(Game::GameMain& gameMain) : CreatorBase{ gameMain } {

}

std::unique_ptr<Object::ObjectBase> PoorEnemyGatringCreator::Create() {

	auto poorEnemyGatling = std::make_unique<Enemy::PoorEnemyGatling>(_gameMain);

	auto model = std::make_unique<Model::ModelAnimeComponent>(*poorEnemyGatling);

	auto state = std::make_unique<AppFrame::State::StateServer>("Idle", std::make_shared <Enemy::PoorEnemyGatling::StateIdle>(*poorEnemyGatling));
	state->Register("Move", std::make_shared<Enemy::PoorEnemyGatling::StateMove>(*poorEnemyGatling));
	poorEnemyGatling->stateServer(std::move(state));


	return std::move(poorEnemyGatling);
}
