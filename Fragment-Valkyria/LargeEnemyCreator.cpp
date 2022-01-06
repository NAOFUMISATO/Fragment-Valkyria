
/*****************************************************************//**
 * \file   LargeEnemyCreator.cpp
 * \brief  
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "LargeEnemyCreator.h"
#include "GameMain.h"
#include "LargeEnemy.h"
#include "ModelAnimeComponent.h"
#include "ObjectBase.h"

using namespace FragmentValkyria;
using namespace FragmentValkyria::Create;


std::unique_ptr<Object::ObjectBase> LargeEnemyCreator::Create(Game::GameMain& game) {
	using Vector4 = AppFrame::Math::Vector4;

	auto largeEnemy = std::make_unique<LargeEnemy::LargeEnemy>(game);
	largeEnemy->position(Vector4(0.0, 0.0, 1000.0));
	largeEnemy->scale(Vector4(3.0, 3.0, 3.0));
	largeEnemy->rotation(Vector4(0.0, 180.0, 0.0));

	auto model = std::make_unique<Model::ModelAnimeComponent>(*largeEnemy);
	model->SetModel("LargeEnemy", 0);
	largeEnemy->modelAnimeComponent(std::move(model));

	auto state = std::make_unique<AppFrame::State::StateServer>("Idle", std::make_shared<LargeEnemy::LargeEnemy::StateIdle>(*largeEnemy));
	state->Register("FallObject", std::make_shared<LargeEnemy::LargeEnemy::StateFallObject>(*largeEnemy));
	largeEnemy->stateServer(std::move(state));

	return std::move(largeEnemy);
}
