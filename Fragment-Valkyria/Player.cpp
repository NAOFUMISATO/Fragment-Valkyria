
/*****************************************************************//**
 * \file   Player.cpp
 * \brief  プレイヤー
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

Player::Player(Game::GameMain& appMain) : ObjectBase{ appMain } {
    Init();
}

void Player::Player::Init()
{
    _rightRotation.RotateY(90.0, true);
    _leftRotation.RotateY(-90.0, true);
    _backRotation.RotateY(180.0, true);
}

void Player::Input(InputManager& input) {
   namespace AppMath = AppFrame::Math;
   _cameraComponent->Input(input);
   _rotateSpeed = 0;
   if (input.GetKeyboard().APress()) {
      _rotateSpeed -= GetLoadJson().GetParam("player","rotspeed");
   }
   else if (input.GetKeyboard().DPress()) {
      _rotateSpeed += GetLoadJson().GetParam("player", "rotspeed");
   }

   _stateServer->Input(input);
}

void Player::Update() {
   auto angle = rotation();
   auto ry = angle.GetY();
   angle.SetY(ry += _rotateSpeed);
   rotation(angle);
   // 状態の更新
   _stateServer->Update();
   // ワールド行列の更新
   ComputeWorldTransform();
   // モデルの更新
   _modelAnimeComponent->Update();
   // カメラの更新
   _cameraComponent->SetTarget(_position, GetForward());
   _cameraComponent->Update();
   // アクターサーバーに位置を通知
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
    _position = _position + (_moved * _moveSpeed);
}

void Player::StateBase::Draw() {
   _owner._modelAnimeComponent->Draw();
}
/// 待機
void Player::StateIdle::Enter() {
   /*_owner._forwardSpeed = 0.0;*/
   _owner._modelAnimeComponent->ChangeAnime("Idle", true);
}
void Player::StateIdle::Input(InputManager& input) {

   if (input.GetKeyboard().SpaceClick()) {
      _owner._stateServer->PushBack("Attack");
   }
   /*if (input.GetKeyboard().WPress()) {
      _owner._stateServer->PushBack("Run");
   }*/
   if (input.GetXJoypad().LeftStickX() <= -3000) {
       _owner._stateServer->PushBack("Run");
   }
   if (input.GetXJoypad().LeftStickX() >= 3000) {
       _owner._stateServer->PushBack("Run");
   }
   if (input.GetXJoypad().LeftStickY() >= 3000) {
       _owner._stateServer->PushBack("Run");
   }
   if (input.GetXJoypad().LeftStickY() <= -3000) {
       _owner._stateServer->PushBack("Run");
   }
}
void Player::StateIdle::Update() {
   _owner._modelAnimeComponent->Update();
}

/// 走り
void Player::StateRun::Enter() {
   /*_owner._forwardSpeed = 10.0;*/
   _owner._modelAnimeComponent->ChangeAnime("Run", true);
}
void Player::StateRun::Input(InputManager& input) {
    auto moved = false;
    // カメラから前進方向単位ベクトルをもとめる
    auto camForward = _owner._cameraComponent->GetForward();
    auto [x, y, z] = camForward.GetXYZ();
    _owner._direction = Vector4(x, 0.0, z);
    _owner._moved = Vector4();
   if (input.GetKeyboard().SpaceClick()) {
      _owner._stateServer->PushBack("Attack");
      return;
   }
   /*if (input.GetKeyboard().WPress()) {
      return;
   }*/
   if (input.GetXJoypad().LeftStickX() >= 3000) {
       _owner._moved = _owner._moved + (_owner._direction * _owner._rightRotation);
       moved = true;
   }
   if (input.GetXJoypad().LeftStickX() <= -3000) {
       _owner._moved = _owner._moved + (_owner._direction * _owner._leftRotation);
       moved = true;
   }
   if (input.GetXJoypad().LeftStickY() >= 3000) {
       _owner._moved = _owner._moved + _owner._direction;
       moved = true;
   }
   if (input.GetXJoypad().LeftStickY() <= -3000) {
       _owner._moved = _owner._moved + (_owner._direction * _owner._backRotation);
       moved = true;
   }
   if (!moved) {
       _owner._stateServer->PopBack();
   }
   else {
       _owner._moved.Normalized();
   }
}
void Player::StateRun::Update() {
   _owner.Move();
}

/// 攻撃
void Player::StateAttack::Enter() {
   /*_owner._forwardSpeed = 0.f;*/
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
