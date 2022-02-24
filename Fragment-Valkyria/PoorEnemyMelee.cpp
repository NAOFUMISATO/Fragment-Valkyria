
#include "PoorEnemyMelee.h"
#include "CollisionComponent.h"
#include "ModelAnimeComponent.h"
#include "ObjectFactory.h"
#include "GameMain.h"
#include "ObjectServer.h"

using namespace FragmentValkyria::Enemy;

PoorEnemyMelee::PoorEnemyMelee(Game::GameMain& gameMain) : PoorEnemyBase{ gameMain } {

}

void PoorEnemyMelee::Init() {
	PoorEnemyBase::Init();
	_actionList.emplace_back("FallObject");
	_actionList.emplace_back("Gatling");
	_actionList.emplace_back("Move");
	_actionList.emplace_back("Laser");
}


void PoorEnemyMelee::StateIdle::Enter() {
   PoorEnemyBase::StateIdle::Enter();
}

void PoorEnemyMelee::StateIdle::Update() {
	if (_owner._stateCnt >= 60 * 5) {
		_owner._stateServer->GoToState("SideStep");
	}
   PoorEnemyBase::StateIdle::Update();
}


void PoorEnemyMelee::StateRush::Enter() {
	_owner._modelAnimeComponent->ChangeAnime("Spider_Armature|Jump", true);
	_owner._stateCnt = 0;
	_moved = _owner.GetObjServer().GetVecData("PlayerPos") - _owner._position;
	_moved.Normalized();
}

void PoorEnemyMelee::StateRush::Update() {
	if (_owner._stateCnt <= 60*3) {
		_owner._position = _owner._position + _moved * 10.0;
	}
	else {
		_owner._stateServer->GoToState("Idle");
	}
	_owner._stateCnt++;
}
