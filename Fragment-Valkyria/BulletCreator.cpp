
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
#include "GameMain.h"
#include "ObjectServer.h"
#include "ModelAnimeComponent.h"

using namespace FragmentValkyria;
using namespace FragmentValkyria::Create;
BulletCreator::BulletCreator(Game::GameMain& gameMain) : CreatorBase{ gameMain } {

}

std::unique_ptr<Object::ObjectBase> BulletCreator::Create() {
   auto bullet = std::make_unique<Player::Bullet>(_gameMain);
   for (auto&& object : _gameMain.objServer().runObjects()) {

      if (object->GetObjType() != Object::ObjectBase::ObjectType::Player) {
         continue;
      }
      /*auto& ply = dynamic_cast<Player::Player&>(*object);*/
      auto handle = object->modelAnimeComponent().modelHandle();
      auto rightHandFrame = object->modelAnimeComponent().FindFrameChild("Kamilla_kari_Reference", "Kamilla_kari_RightFingerBase");
      auto rightHandPos = MV1GetFramePosition(handle, rightHandFrame);
      bullet->position(AppFrame::Math::ToMath(rightHandPos));
   }
   bullet->Init();

   auto state = std::make_unique<AppFrame::State::StateServer>("Shoot", std::make_shared<Player::Bullet::StateShoot>(*bullet));
   state->Register("Die", std::make_shared<Player::Bullet::StateDie>(*bullet));
   bullet->stateServer(std::move(state));

   return std::move(bullet);
}
