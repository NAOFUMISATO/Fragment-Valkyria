
/*****************************************************************//**
 * \file   PlayerCreator.cpp
 * \brief  プレイヤーの生成
 *
 * \author NAOFUMISATO, AHMD2000
 * \date   December 2021
 *********************************************************************/
#include "PlayerCreator.h"
#include "Player.h"
#include "ObjectBase.h"
#include "Game.h"
#include "CameraComponent.h"
#include "ModelAnimeComponent.h"
#include "PlayerHP.h"
#include "RemainingBullet.h"
#include "RemainingPortion.h"
#include "Reticle.h"
#include "SpriteServer.h"

using namespace FragmentValkyria;
using namespace FragmentValkyria::Create;

std::unique_ptr<Object::ObjectBase> PlayerCreator::Create() {
   // カメラの生成
   auto camera = std::make_shared<Camera::CameraComponent>();
   // カメラの位置の設定
   camera->SetPosition({ 0, 120, -500 });
   // カメラの注視点の設定
   camera->SetTarget({ 500, 150, 1500 });
   // カメラの初期化処理
   camera->Init();

   // カメラの状態一括管理クラスの生成
   auto camerastate = std::make_unique<AppFrame::State::StateServer>("Normal",
      std::make_shared <Camera::CameraComponent::StateNormal>(*camera));
   // カメラの状態の追加登録
   camerastate->Register("ZoomIn", std::make_shared<Camera::CameraComponent::StateZoomIn>(*camera));
   camerastate->Register("ShootReady", std::make_shared<Camera::CameraComponent::StateShootReady>(*camera));
   camerastate->Register("ZoomOut", std::make_shared<Camera::CameraComponent::StateZoomOut>(*camera));
   // カメラの状態一括管理クラスの設定
   camera->stateServer(std::move(camerastate));

   // プレイヤーの生成
   auto player = std::make_unique<Player::Player>();
   // プレイヤーのカメラ管理クラスの設定
   player->cameraComponent(camera);
   player->position({0,0,-2000.0});

   // プレイヤーのアニメーション一括管理クラスの生成
   auto model = std::make_unique<Model::ModelAnimeComponent>(*player);
   model->Init();
   // モデルの設定
   model->SetModel("Player");
   model->PixelLightingON();
   // プレイヤーのアニメーション一括管理クラスの設定
   player->modelAnimeComponent(std::move(model));
   
   // プレイヤーの状態一括管理クラスの生成
   auto state = std::make_unique<AppFrame::State::StateServer>("Idle", 
      std::make_shared <Player::Player::StateIdle>(*player));
   // プレイヤーの状態の追加登録
   state->Register("Run", std::make_shared<Player::Player::StateRun>(*player));
   state->Register("ShootReady", std::make_shared<Player::Player::StateShootReady>(*player));
   state->Register("KnockBack", std::make_shared<Player::Player::StateKnockBack>(*player));
   state->Register("Die", std::make_shared<Player::Player::StateDie>(*player));
   state->Register("WeakShootReady", std::make_shared<Player::Player::StateWeakShootReady>(*player));
   state->Register("Reload", std::make_shared<Player::Player::StateReload>(*player));
   state->Register("Recovery", std::make_shared<Player::Player::StateRecovery>(*player));
   state->Register("Walk", std::make_shared<Player::Player::StateWalk>(*player));
   // プレイヤーの状態一括管理クラスの設定
   player->stateServer(std::move(state));

   auto& gameInstance = Game::Game::GetInstance();
   auto& sprServer = gameInstance.sprServer();
   sprServer.Add(std::make_unique<Player::PlayerHP>());
   sprServer.Add(std::make_unique<Player::RemainingBullet>());
   sprServer.Add(std::make_unique<Player::RemainingPortion>());
   sprServer.Add(std::make_unique<Player::Reticle>());

   // プレイヤーのインスタンスを返す
   return std::move(player);
}