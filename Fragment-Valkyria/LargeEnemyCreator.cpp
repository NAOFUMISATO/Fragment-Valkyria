
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
#include "LargeEnemyHP.h"
#include "ModelAnimeComponent.h"
#include "ObjectBase.h"

using namespace FragmentValkyria;
using namespace FragmentValkyria::Create;


LargeEnemyCreator::LargeEnemyCreator(Game::GameMain& gameMain) : CreatorBase{ gameMain } {

}

std::unique_ptr<Object::ObjectBase> LargeEnemyCreator::Create() {
	using Vector4 = AppFrame::Math::Vector4;

	auto largeEnemy = std::make_unique<Enemy::LargeEnemy>(_gameMain);
	largeEnemy->position(Vector4(0.0, 0.0, 1000.0));
	largeEnemy->scale(Vector4(1.0, 1.0, 1.0));
	largeEnemy->rotation(Vector4(0.0, 0.0, 0.0));

	auto model = std::make_unique<Model::ModelAnimeComponent>(*largeEnemy);
	model->SetModel("LargeEnemy");
	largeEnemy->modelAnimeComponent(std::move(model));

	auto state = std::make_unique<AppFrame::State::StateServer>("Idle", std::make_shared<Enemy::LargeEnemy::StateIdle>(*largeEnemy));
	state->Register("FallObject", std::make_shared<Enemy::LargeEnemy::StateFallObject>(*largeEnemy));
	state->Register("Gatling", std::make_shared<Enemy::LargeEnemy::StateGatling>(*largeEnemy));
	state->Register("Move", std::make_shared<Enemy::LargeEnemy::StateMove>(*largeEnemy));
	state->Register("Die", std::make_shared<Enemy::LargeEnemy::StateDie>(*largeEnemy));
	state->Register("Laser", std::make_shared<Enemy::LargeEnemy::StateLaser>(*largeEnemy));
	largeEnemy->stateServer(std::move(state));

	_gameMain.sprServer().Add(std::make_unique<Enemy::LargeEnemyHP>(_gameMain));

	return std::move(largeEnemy);
}
