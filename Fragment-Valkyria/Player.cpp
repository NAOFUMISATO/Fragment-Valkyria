
/*****************************************************************//**
 * \file   Player.cpp
 * \brief  �v���C���[
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "Player.h"
#include "ObjectServer.h"
#include "CameraComponent.h"
#include "ModelAnimeComponent.h"
#include "GameMain.h"
using namespace FragmentValkyria::Player;

namespace {
   constexpr auto RotateSpeed = 2.0;
}

Player::Player(Game::GameMain& appMain) : ObjectBase{ appMain } {
}

void Player::Input(InputManager& input) {
   namespace AppMath = AppFrame::Math;
   _cameraComponent->Input(input);
   _rotateSpeed = 0;
   if (input.GetKeyboard().APress()) {
      _rotateSpeed -= RotateSpeed;
   }
   else if (input.GetKeyboard().DPress()) {
      _rotateSpeed += RotateSpeed;
   }
   _stateServer->Input(input);
}

void Player::Update() {
   auto angle = rotation();
   auto ry = angle.GetY();
   angle.SetY(ry += _rotateSpeed);
   rotation(angle);
   // ��Ԃ̍X�V
   _stateServer->Update();
   // ���[���h�s��̍X�V
   ComputeWorldTransform();
   // ���f���̍X�V
   _modelAnimeComponent->Update();
   // �J�����̍X�V
   _cameraComponent->SetTarget(_position, GetForward());
   _cameraComponent->Update();
   // �A�N�^�[�T�[�o�[�Ɉʒu��ʒm
   GetObjServer().Register("Player", _position);
}

void Player::Draw() {
   _stateServer->Draw();
}

void Player::ComputeWorldTransform() {
   auto [sx, sy, sz] = _scale.GetXYZ();
   auto [rx, ry, rz] = _rotation.GetXYZ();
   auto [px, py, pz] = _position.GetXYZ();
   auto world = Matrix44();
   world.Scale(sx, sy, sz, true);
   world.RotateZ(rz, false);
   world.RotateX(rx, false);
   world.RotateY(ry + AppFrame::Math::DEGREES_180, false);
   world.Transfer(px, py, pz, false);
   _worldTransform = world;
}

void Player::Move() {
   // �O�i�����P�ʃx�N�g���ɃX�s�[�h������
   auto delta = GetForward() * _forwardSpeed;
   _position = _position + delta;
}

void Player::StateBase::Draw() {
   _owner._modelAnimeComponent->Draw();
}
/// �ҋ@
void Player::StateIdle::Enter() {
   _owner._forwardSpeed = 0.0;
   _owner._modelAnimeComponent->ChangeAnime("Idle", true);
}
void Player::StateIdle::Input(InputManager& input) {

   if (input.GetKeyboard().SpaceClick()) {
      _owner._stateServer->PushBack("Attack");
   }
   if (input.GetKeyboard().WPress()) {
      _owner._stateServer->PushBack("Run");
   }
}
void Player::StateIdle::Update() {
   _owner._modelAnimeComponent->Update();
}

/// ����
void Player::StateRun::Enter() {
   _owner._forwardSpeed = 10.0;
   _owner._modelAnimeComponent->ChangeAnime("Run", true);
}
void Player::StateRun::Input(InputManager& input) {
   if (input.GetKeyboard().SpaceClick()) {
      _owner._stateServer->PushBack("Attack");
      return;
   }
   if (input.GetKeyboard().WPress()) {
      return;
   }
   _owner._stateServer->PopBack();
}
void Player::StateRun::Update() {
   _owner.Move();
}

/// �U��
void Player::StateAttack::Enter() {
   _owner._forwardSpeed = 0.f;
   _owner._modelAnimeComponent->ChangeAnime("Attack");
}
void Player::StateAttack::Update() {
   auto cnt = _owner._modelAnimeComponent->repeatedCount();
   if (cnt > 0) {
      _owner._stateServer->PopBack();
      return;
   }
   auto playTime = _owner._modelAnimeComponent->playTime();
   if (playTime < 3.5f || playTime > 20.f) {
      return;
   }
}
void Player::StateAttack::Draw() {
   _owner._modelAnimeComponent->Draw();
}
