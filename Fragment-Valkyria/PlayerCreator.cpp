
/*****************************************************************//**
 * \file   PlayerCreator.cpp
 * \brief  �v���C���[�̐���
 *
 * \author NAOFUMISATO, AHMD2000
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
#include "RemainingPortion.h"
#include "Reticle.h"
#include "SpriteServer.h"

using namespace FragmentValkyria;
using namespace FragmentValkyria::Create;

PlayerCreator::PlayerCreator(Game::GameMain& gameMain) : CreatorBase{ gameMain } {

}

std::unique_ptr<Object::ObjectBase> PlayerCreator::Create() {
   // �J�����̐���
   auto camera = std::make_shared<Camera::CameraComponent>(_gameMain);
   // �J�����̈ʒu�̐ݒ�
   camera->SetPosition({ 0, 120, -500 });
   // �J�����̒����_�̐ݒ�
   camera->SetTarget({ 500, 150, 1500 });
   // �J�����̏���������
   camera->Init();

   // �J�����̏�Ԉꊇ�Ǘ��N���X�̐���
   auto camerastate = std::make_unique<AppFrame::State::StateServer>("Normal", std::make_shared <Camera::CameraComponent::StateNormal>(*camera));
   // �J�����̏�Ԃ̒ǉ��o�^
   camerastate->Register("ZoomIn", std::make_shared<Camera::CameraComponent::StateZoomIn>(*camera));
   camerastate->Register("ShootReady", std::make_shared<Camera::CameraComponent::StateShootReady>(*camera));
   camerastate->Register("ZoomOut", std::make_shared<Camera::CameraComponent::StateZoomOut>(*camera));
   // �J�����̏�Ԉꊇ�Ǘ��N���X�̐ݒ�
   camera->stateServer(std::move(camerastate));

   // �v���C���[�̐���
   auto player = std::make_unique<Player::Player>(_gameMain);
   /*player->position(gameMain.loadJson().GetVecParam())*/
   // �v���C���[�̃J�����Ǘ��N���X�̐ݒ�
   player->cameraComponent(camera);
   player->position({0,0,-2000.0});

   // �v���C���[�̃A�j���[�V�����ꊇ�Ǘ��N���X�̐���
   auto model = std::make_unique<Model::ModelAnimeComponent>(*player);
   // ���f���̐ݒ�
   model->SetModel("Player");
   model->PixelLightingON();
   // �v���C���[�̃A�j���[�V�����ꊇ�Ǘ��N���X�̐ݒ�
   player->modelAnimeComponent(std::move(model));
   
   // �v���C���[�̏�Ԉꊇ�Ǘ��N���X�̐���
   auto state = std::make_unique<AppFrame::State::StateServer>("Idle", std::make_shared <Player::Player::StateIdle>(*player));
   // �v���C���[�̏�Ԃ̒ǉ��o�^
   state->Register("Run", std::make_shared<Player::Player::StateRun>(*player));
   state->Register("ShootReady", std::make_shared<Player::Player::StateShootReady>(*player));
   state->Register("KnockBack", std::make_shared<Player::Player::StateKnockBack>(*player));
   state->Register("Die", std::make_shared<Player::Player::StateDie>(*player));
   state->Register("WeakShootReady", std::make_shared<Player::Player::StateWeakShootReady>(*player));
   state->Register("Reload", std::make_shared<Player::Player::StateReload>(*player));
   state->Register("Recovery", std::make_shared<Player::Player::StateRecovery>(*player));
   // �v���C���[�̏�Ԉꊇ�Ǘ��N���X�̐ݒ�
   player->stateServer(std::move(state));

   auto& sprServer = _gameMain.sprServer();
   sprServer.Add(std::make_unique<Player::PlayerHP>(_gameMain));
   sprServer.Add(std::make_unique<Player::RemainingBullet>(_gameMain));
   sprServer.Add(std::make_unique<Player::RemainingPortion>(_gameMain));
   sprServer.Add(std::make_unique<Player::Reticle>(_gameMain));

   // �v���C���[�̃C���X�^���X��Ԃ�
   return std::move(player);
}