
/*****************************************************************//**
 * \file   PlayerCreator.cpp
 * \brief  プレイヤーの生成
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "PlayerCreator.h"
#include "Player.h"
#include "ObjectBase.h"
#include "GameMain.h"
#include "CameraComponent.h"
#include "ModelAnimeComponent.h"
#include "PlayerHP.h"
#include "RemainingBullet.h"
using namespace FragmentValkyria;
using namespace FragmentValkyria::Create;

PlayerCreator::PlayerCreator(Game::GameMain& gameMain) : CreatorBase{ gameMain } {

}

std::unique_ptr<Object::ObjectBase> PlayerCreator::Create() {
   auto camera = std::make_shared<Camera::CameraComponent>(_gameMain);
   camera->SetPosition({ 0, 120, -500 });
   camera->SetTarget({ 150, 300, 2000 });
   camera->Init();

   auto camerastate = std::make_unique<AppFrame::State::StateServer>("Normal", std::make_shared <Camera::CameraComponent::StateNormal>(*camera));
   camerastate->Register("ZoomIn", std::make_shared<Camera::CameraComponent::StateZoomIn>(*camera));
   camerastate->Register("ShootReady", std::make_shared<Camera::CameraComponent::StateShootReady>(*camera));
   camerastate->Register("ZoomOut", std::make_shared<Camera::CameraComponent::StateZoomOut>(*camera));
   camera->stateServer(std::move(camerastate));

   // プレイヤーの生成
   auto player = std::make_unique<Player::Player>(_gameMain);
   /*player->position(gameMain.loadJson().GetVecParam())*/
   player->cameraComponent(camera);

   auto model = std::make_unique<Model::ModelAnimeComponent>(*player);
   model->SetModel("Player");
   model->PixelLightingON();
   player->modelAnimeComponent(std::move(model));

   auto state = std::make_unique<AppFrame::State::StateServer>("Idle", std::make_shared <Player::Player::StateIdle>(*player));
   state->Register("Run", std::make_shared<Player::Player::StateRun>(*player));
   state->Register("Attack", std::make_shared<Player::Player::StateAttack>(*player));
   state->Register("ShootReady", std::make_shared<Player::Player::StateShootReady>(*player));
   state->Register("KnockBack", std::make_shared<Player::Player::StateKnockBack>(*player));
   state->Register("Die", std::make_shared<Player::Player::StateDie>(*player));
   state->Register("WeakShootReady", std::make_shared<Player::Player::StateWeakShootReady>(*player));
   state->Register("Reload", std::make_shared<Player::Player::StateReload>(*player));
   player->stateServer(std::move(state));

   _gameMain.sprServer().Add(std::make_unique<Player::PlayerHP>(_gameMain));
   _gameMain.sprServer().Add(std::make_unique<Player::RemainingBullet>(_gameMain));
   return std::move(player);
}