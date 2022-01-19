
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

using namespace FragmentValkyria;
using namespace FragmentValkyria::Create;

FallObjectCreator::FallObjectCreator(Game::GameMain& gameMain) : CreatorBase{ gameMain } {

}

std::unique_ptr<Object::ObjectBase> FallObjectCreator::Create() {
	using Vector4 = AppFrame::Math::Vector4;
	using Matrix44 = AppFrame::Math::Matrix44;

	auto rightTransMatrix = Matrix44();
	auto leftTransMatrix = Matrix44();
	/*auto da =AppFrame::Math::Utility::GetRandom(45.0,70.0);*/
	rightTransMatrix.RotateY(45.0, true);
	leftTransMatrix.RotateY(-45.0, true);

	auto MoveVec = _gameMain.objServer().GetPosition("CamTarget") - _gameMain.objServer().GetPosition("CamPos");
	MoveVec.Normalized();

	auto rightMoveVec = MoveVec * rightTransMatrix;
	auto leftMoveVec = MoveVec * leftTransMatrix;

	constexpr double distance = 1000.0;

	std::array<Vector4, 3> startPosition = {
		_gameMain.objServer().GetPosition("PlayerPos") + Vector4(0.0, 500.0, 0.0),
		_gameMain.objServer().GetPosition("PlayerPos") + Vector4(0.0, 500.0, 0.0) + (rightMoveVec * distance),
		_gameMain.objServer().GetPosition("PlayerPos") + Vector4(0.0, 500.0, 0.0) + (leftMoveVec * distance),
	};

	for (auto i = 0; i < 3; ++i) {
		auto fallObject = std::make_unique<Enemy::FallObject>(_gameMain);

		fallObject->position(startPosition[i]);

		auto model = std::make_unique<Model::ModelAnimeComponent>(*fallObject);
		model->SetModel("DrumGreen", 1000);
		fallObject->modelAnimeComponent(std::move(model));

		fallObject->Init();

		auto state = std::make_unique<AppFrame::State::StateServer>("Fall", std::make_shared<Enemy::FallObject::StateFall>(*fallObject));
		state->Register("Idle", std::make_shared<Enemy::FallObject::StateIdle>(*fallObject));
		state->Register("Save", std::make_shared<Enemy::FallObject::StateSave>(*fallObject));
		state->Register("Shoot", std::make_shared<Enemy::FallObject::StateShoot>(*fallObject));
		fallObject->stateServer(std::move(state));

		if (i < 2) {
			_gameMain.objServer().Add(std::move(fallObject));
		}
		else {
			_fallObject = std::move(fallObject);
		}
	}
	
	return std::move(_fallObject);
}
