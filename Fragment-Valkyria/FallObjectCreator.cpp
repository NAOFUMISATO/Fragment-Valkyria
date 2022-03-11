
/*****************************************************************//**
 * \file   FallObjectCreator.cpp
 * \brief  
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "FallObjectCreator.h"
#include "CollisionComponent.h"
#include "FallObject.h"
#include "GameMain.h"
#include "ModelAnimeComponent.h"
#include "ObjectServer.h"

using namespace FragmentValkyria;
using namespace FragmentValkyria::Create;

namespace {
   constexpr int MaxNum = 6;
}

FallObjectCreator::FallObjectCreator(Game::GameMain& gameMain) : CreatorBase{ gameMain } {

}

std::unique_ptr<Object::ObjectBase> FallObjectCreator::Create() {
   auto random = AppFrame::Math::Utility::GetRandom(0, _fallObjectModels.size() - 1);
   // �����I�u�W�F�N�g�̐���
   auto fallObject = std::make_unique<Enemy::FallObject>(_gameMain);

   auto model = std::make_unique<Model::ModelAnimeComponent>(*fallObject);
   model->SetModel(_fallObjectModels[random], 1000);
   fallObject->modelAnimeComponent(std::move(model));

   fallObject->collisionName(_fallObjectCollisions[random]);
   fallObject->Init();

   auto state = std::make_unique<AppFrame::State::StateServer>("Fall", std::make_shared<Enemy::FallObject::StateFall>(*fallObject));
   state->Register("Idle", std::make_shared<Enemy::FallObject::StateIdle>(*fallObject));
   state->Register("Save", std::make_shared<Enemy::FallObject::StateSave>(*fallObject));
   state->Register("Shoot", std::make_shared<Enemy::FallObject::StateShoot>(*fallObject));
   state->Register("Die", std::make_shared<Enemy::FallObject::StateDie>(*fallObject));
   state->Register("Up", std::make_shared<Enemy::FallObject::StateUp>(*fallObject));
   fallObject->stateServer(std::move(state));
   
   return std::move(fallObject);
}
