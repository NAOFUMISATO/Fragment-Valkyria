
/*****************************************************************//**
 * \file   PlayerCreator.cpp
 * \brief  プレイヤーの生成
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "Player.h"
#include "PlayerCreator.h"
#include "ObjectBase.h"
#include "GameMain.h"
#include "CameraComponent.h"
#include "ModelAnimeComponent.h"
using namespace FragmentValkyria;
using namespace FragmentValkyria::Create;

std::unique_ptr<Object::ObjectBase> PlayerCreator::Create(Game::GameMain& gameMain) {
   auto camera = std::make_shared<Camera::CameraComponent>();
   camera->Init();
   camera->SetPosition({ 0, 50, -200 });
   camera->SetTarget({ 0, 50, 0 });

   // プレイヤーの生成
   auto player = std::make_unique<Player::Player>(gameMain);
   player->cameraComponent(camera);

   auto model = std::make_unique<Model::ModelAnimeComponent>(*player);
   model->SetModel("Player");
   model->Register("Idle", 0);
   model->Register("Run", 1);
   model->Register("Attack", 2);
   model->Register("JumpStart", 3);
   model->Register("JumpLoop", 4);
   model->Register("JumpEnd", 5);
   player->modelAnimeComponent(std::move(model));

   auto state = std::make_unique<AppFrame::State::StateServer>("Idle", std::make_shared <Player::Player::StateIdle>(*player));
   state->Register("Run", std::make_shared<Player::Player::StateRun>(*player));
   state->Register("Attack", std::make_shared<Player::Player::StateAttack>(*player));
   player->stateServer(std::move(state));

   return std::move(player);
}