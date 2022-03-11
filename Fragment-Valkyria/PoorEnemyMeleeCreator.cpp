#include "PoorEnemyMeleeCreator.h"
#include "PoorEnemyMelee.h"
#include "ModelAnimeComponent.h"

using namespace FragmentValkyria;
using namespace FragmentValkyria::Create;

PoorEnemyMeleeCreator::PoorEnemyMeleeCreator(Game::GameMain& gameMain) : CreatorBase{ gameMain } {

}

std::unique_ptr<Object::ObjectBase> PoorEnemyMeleeCreator::Create() {

   auto poorEnemyMelee = std::make_unique<Enemy::PoorEnemyMelee>(_gameMain);

   auto model = std::make_unique<Model::ModelAnimeComponent>(*poorEnemyMelee);
   model->Init();
   model->SetModel("PoorEnemyMelee", 1000);
   poorEnemyMelee->modelAnimeComponent(std::move(model));

   auto state = std::make_unique<AppFrame::State::StateServer>("Fall", std::make_shared <Enemy::PoorEnemyMelee::StateFall>(*poorEnemyMelee));
   state->Register("Idle", std::make_shared<Enemy::PoorEnemyMelee::StateIdle>(*poorEnemyMelee));
   state->Register("Rush", std::make_shared<Enemy::PoorEnemyMelee::StateRush>(*poorEnemyMelee));
   state->Register("SideStep", std::make_shared<Enemy::PoorEnemyMelee::StateSideStep>(*poorEnemyMelee));
   state->Register("Die", std::make_shared<Enemy::PoorEnemyMelee::StateDie>(*poorEnemyMelee));
   poorEnemyMelee->stateServer(std::move(state));

   return std::move(poorEnemyMelee);
}
