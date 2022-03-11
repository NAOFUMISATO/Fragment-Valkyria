#include "PoorEnemyAlmightyCreator.h"
#include "PoorEnemyAlmighty.h"
#include "ModelAnimeComponent.h"

using namespace FragmentValkyria;
using namespace FragmentValkyria::Create;

PoorEnemyAlmightyCreator::PoorEnemyAlmightyCreator(Game::GameMain& gameMain) : CreatorBase{ gameMain } {

}

std::unique_ptr<Object::ObjectBase> PoorEnemyAlmightyCreator::Create() {

   auto poorEnemyAlmighty = std::make_unique<Enemy::PoorEnemyAlmighty>(_gameMain);

   auto model = std::make_unique<Model::ModelAnimeComponent>(*poorEnemyAlmighty);
   model->Init();
   model->SetModel("PoorEnemyAlmighty", 1000);
   poorEnemyAlmighty->modelAnimeComponent(std::move(model));

   auto state = std::make_unique<AppFrame::State::StateServer>("Fall", std::make_shared <Enemy::PoorEnemyAlmighty::StateFall>(*poorEnemyAlmighty));
   state->Register("Idle", std::make_shared<Enemy::PoorEnemyAlmighty::StateIdle>(*poorEnemyAlmighty));
   state->Register("Rush", std::make_shared<Enemy::PoorEnemyAlmighty::StateRush>(*poorEnemyAlmighty));
   state->Register("Gatling", std::make_shared<Enemy::PoorEnemyAlmighty::StateGatling>(*poorEnemyAlmighty));
   state->Register("SideStep", std::make_shared<Enemy::PoorEnemyAlmighty::StateSideStep>(*poorEnemyAlmighty));
   state->Register("Die", std::make_shared<Enemy::PoorEnemyAlmighty::StateDie>(*poorEnemyAlmighty));
   poorEnemyAlmighty->stateServer(std::move(state));

   return std::move(poorEnemyAlmighty);
}
