
/*****************************************************************//**
 * \file   BulletCreator.cpp
 * \brief  âìäué„çUåÇÇÃê∂ê¨
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "BulletCreator.h"
#include "Bullet.h"
#include "GameMain.h"
#include "ObjectServer.h"

using namespace FragmentValkyria;
using namespace FragmentValkyria::Create;
BulletCreator::BulletCreator(Game::GameMain& gameMain) : CreatorBase{ gameMain } {

}

std::unique_ptr<Object::ObjectBase> BulletCreator::Create() {
	auto bullet = std::make_unique<Player::Bullet>(_gameMain);
	auto startPos = _gameMain.objServer().GetVecData("PlayerPos") + AppFrame::Math::Vector4(0.0, 100.0, 0.0);
	bullet->position(startPos);
	bullet->Init();

	auto state = std::make_unique<AppFrame::State::StateServer>("Shoot", std::make_shared<Player::Bullet::StateShoot>(*bullet));
	state->Register("Die", std::make_shared<Player::Bullet::StateDie>(*bullet));
	bullet->stateServer(std::move(state));

	return std::move(bullet);
}
