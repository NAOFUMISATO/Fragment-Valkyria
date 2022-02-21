
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
	_actionList.emplace_back("FallObject");
	_actionList.emplace_back("Gatling");
	_actionList.emplace_back("Move");
	_actionList.emplace_back("Laser");
}

void PoorEnemyMelee::Rush(){
}


void PoorEnemyMelee::StateIdle::Enter() {
   PoorEnemyBase::StateIdle::Enter();
}

void PoorEnemyMelee::StateIdle::Update() {
   PoorEnemyBase::StateIdle::Update();
}


void PoorEnemyMelee::StateRush::Enter() {

}

void PoorEnemyMelee::StateRush::Update() {

}

void PoorEnemyMelee::StateRush::Draw() {

}
