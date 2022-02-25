
#include "PoorEnemyMelee.h"
#include "CollisionComponent.h"
#include "ModelAnimeComponent.h"
#include "ObjectFactory.h"
#include "GameMain.h"
#include "ObjectServer.h"

namespace {
	constexpr auto RushSpeed = 15.0;
}

using namespace FragmentValkyria::Enemy;

PoorEnemyMelee::PoorEnemyMelee(Game::GameMain& gameMain) : PoorEnemyBase{ gameMain } {

}

void PoorEnemyMelee::Init() {
	PoorEnemyBase::Init();
	_actionList.emplace_back("Rush");
}

void PoorEnemyMelee::Rush(const Vector4& moved) {
	auto [x, y, z] = moved.GetVec3();
	auto position = _position;
	position = _collisionComponent->LargeEnemyCheckStage(position, Vector4(x, y, 0.0));
	position = _collisionComponent->LargeEnemyCheckStage(position, Vector4(0.0, y, z));
	_position = position;
}

void PoorEnemyMelee::StateRush::Enter() {
	_owner._modelAnimeComponent->ChangeAnime("Spider_Armature|Jump", true);
	_owner._stateCnt = 0;
	_moved = _owner.GetObjServer().GetVecData("PlayerPos") - _owner._position;
	_moved.Normalized();
	_moved = _moved * RushSpeed;
}

void PoorEnemyMelee::StateRush::Update() {
	if (_owner._stateCnt <= 60 * 4) {
		_owner.Rush(_moved);
	}
	else {
		_owner._stateServer->GoToState("Idle");
	}
	_owner._stateCnt++;
}
