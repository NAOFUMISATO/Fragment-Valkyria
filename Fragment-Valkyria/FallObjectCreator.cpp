
/*****************************************************************//**
 * \file   FallObjectCreator.cpp
 * \brief  
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "FallObjectCreator.h"
#include "FallObject.h"
#include "GameMain.h"
#include "ModelAnimeComponent.h"

using namespace FragmentValkyria;
using namespace FragmentValkyria::Create;

std::unique_ptr<Object::ObjectBase> FragmentValkyria::Create::FallObjectCreator::Create(Game::GameMain& game) {
	using Vector4 = AppFrame::Math::Vector4;

	auto fallObject = std::make_unique<FallObject::FallObject>(game);
	auto startPos = game.objServer().GetPosition("Player") + Vector4(0.0, 500.0, 0.0);
	fallObject->position(startPos);

	auto model = std::make_unique<Model::ModelAnimeComponent>(*fallObject);
	model->SetModel("LargeEnemy", 1000);
	fallObject->modelAnimeComponent(std::move(model));

	auto state = std::make_unique<AppFrame::State::StateServer>("Fall", std::make_shared<FallObject::FallObject::StateFall>(*fallObject));
	state->Register("Idle", std::make_shared<FallObject::FallObject::StateIdle>(*fallObject));
	fallObject->stateServer(std::move(state));
	
	return std::move(fallObject);
}
