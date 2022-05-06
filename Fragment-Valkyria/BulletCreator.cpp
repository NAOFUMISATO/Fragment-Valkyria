
/*****************************************************************//**
 * \file   BulletCreator.cpp
 * \brief  âìäué„çUåÇÇÃê∂ê¨
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "BulletCreator.h"
#include "Bullet.h"
#include "Player.h"
#include "Game.h"
#include "ObjectServer.h"
#include "ModelAnimeComponent.h"

using namespace FragmentValkyria;
using namespace FragmentValkyria::Create;

std::unique_ptr<Object::ObjectBase> BulletCreator::Create() {
   auto bullet = std::make_unique<Player::Bullet>();
   auto& runObjects = Game::Game::GetObjServer().runObjects();
   for (auto&& object : runObjects) {
      if (object->GetObjType() != Object::ObjectBase::ObjectType::Player) {
         continue;
      }
      auto rightHandFramePos = object->modelAnimeComponent().GetFrameChildPosion("Kamilla_kari_Reference", "Kamilla_kari_RightFingerBase");
      bullet->position(rightHandFramePos);
   }

   auto state = std::make_unique<AppFrame::State::StateServer>("Shoot",
      std::make_shared<Player::Bullet::StateShoot>(*bullet));
   bullet->stateServer(std::move(state));

   return std::move(bullet);
}
