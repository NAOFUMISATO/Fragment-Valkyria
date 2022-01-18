
/*****************************************************************//**
 * \file   Player.cpp
 * \brief  プレイヤー
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "Player.h"
#include <cmath>
#include "ObjectServer.h"
#include "CameraComponent.h"
#include "CollisionComponent.h"
#include "ModelAnimeComponent.h"
#include "GameMain.h"
using namespace FragmentValkyria::Player;

namespace {
    auto paramMap = AppFrame::Resource::LoadJson::GetParamMap("player",
        { "movespeed" });

    const double MoveSpeed = paramMap["movespeed"];
}

Player::Player(Game::GameMain& gameMain) : ObjectBase{ gameMain } {
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

   _stateServer->Input(input);
}

void Player::Update() {
   // 状態の更新
   _stateServer->Update();
   // ワールド行列の更新
   ComputeWorldTransform();
   // モデルの更新
   _modelAnimeComponent->Update();
   // カメラの更新
   _cameraComponent->SetTarget(_position);
   _cameraComponent->SetPlyPos(_position);
   _cameraComponent->Update();
   // オブジェクトサーバーに位置を通知
   GetObjServer().Register("PlayerPos", _position);
   GetObjServer().Register("PlayerRot", _rotation);
   GetObjServer().Register("CamTarget", _cameraComponent->GetTarget());
   GetObjServer().Register("CamPos", _cameraComponent->GetPos());
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

void Player::Move(Vector4 forward) {
    _position = _position + forward;
}

void Player::ShootRotate() {
    // カメラから向く方向の単位ベクトルをもとめる
    auto camForward = _cameraComponent->GetForward();

    auto [x, y, z] = camForward.GetXYZ();
    auto direction = Vector4(x, 0.0, z);
    auto radian = std::atan2(x, z);
    _rotation.SetY(AppFrame::Math::Utility::RadianToDegree(radian));

}

void Player::HitCheckFromFallObjectRange() {
    auto report = _collisionComponent->report();
    if (report.id() == Collision::CollisionComponent::ReportId::HitFromObjectRange) {
        _stateServer->PushBack("ShootReady");
    }
}

void Player::HitCheckFromIdleFallObject() {
    auto report = _collisionComponent->report();
    if (report.id() == Collision::CollisionComponent::ReportId::HitFromIdleFallObject) {
        auto hitPos = _collisionComponent->hitPos();
        _position = _position + (_moved * -MoveSpeed);

        _collisionComponent->report().id(Collision::CollisionComponent::ReportId::None);
    }
}

void Player::HitCheckFromGatling() {
    auto report = _collisionComponent->report();
    if (report.id() == Collision::CollisionComponent::ReportId::HitFromGatling) {
        auto hitPos = _collisionComponent->hitPos();
        auto knockBackVec = _position - hitPos;
        auto [x, y, z] = knockBackVec.GetXYZ();
        auto knockBackDelta = Vector4(x, 0.0, z);
        knockBackDelta.Normalized();
        _knockBack = knockBackDelta * 10.0;

        _collisionComponent->report().id(Collision::CollisionComponent::ReportId::None);

        _stateServer->PushBack("KnockBack");
    }
}

void Player::StateBase::Draw() {
   _owner._modelAnimeComponent->Draw();
#ifdef _DEBUG
   auto pos1 = _owner._position + Vector4(0.0, 30.0, 0.0);
   auto pos2 = _owner._position + Vector4(0.0, 60.0, 0.0);
   auto radian = static_cast<float>(30.0);
  
   DrawCapsule3D(AppFrame::Math::ToDX(pos1), AppFrame::Math::ToDX(pos2), radian, 20, GetColor(0, 255, 0), GetColor(0, 0, 0), FALSE);
#endif

}
/// 待機
void Player::StateIdle::Enter() {
   /*_owner._forwardSpeed = 0.0;*/
   _owner._modelAnimeComponent->ChangeAnime("MO_SDChar_idle", true);
}
void Player::StateIdle::Input(InputManager& input) {

    _owner.HitCheckFromIdleFallObject();

   if (input.GetKeyboard().SpaceClick()) {
      _owner._stateServer->PushBack("Attack");
   }
   //左スティックが動いていたら走り状態へ
   if (input.GetXJoypad().LeftStickX() >= 3000) {
       _owner._stateServer->PushBack("Run");
   }
   if (input.GetXJoypad().LeftStickX() <= -3000) {
       _owner._stateServer->PushBack("Run");
   }
   if (input.GetXJoypad().LeftStickY() >= 3000) {
       _owner._stateServer->PushBack("Run");
   }
   if (input.GetXJoypad().LeftStickY() <= -3000) {
       _owner._stateServer->PushBack("Run");
   }

   if (input.GetXJoypad().LeftTrigger() >= 20) {
       _owner.HitCheckFromFallObjectRange();
   }
}
void Player::StateIdle::Update() {
    _owner._collisionComponent->ObjectRangeFromPlayer();
    _owner._collisionComponent->GatlingFromPlayer();
    _owner.HitCheckFromGatling();
}

/// 走り
void Player::StateRun::Enter() {
   /*_owner._forwardSpeed = 10.0;*/
   _owner._modelAnimeComponent->ChangeAnime("MO_SDChar_run", true);
}
void Player::StateRun::Input(InputManager& input) {
    _owner.HitCheckFromIdleFallObject();

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
   if (input.GetXJoypad().LeftTrigger() >= 20) {
       _owner.HitCheckFromFallObjectRange();
   }
   if (!moved) {
       _owner._stateServer->PopBack();
   }
   else {
       _owner._moved.Normalized();
       _owner._moved = _owner._moved * MoveSpeed;
       auto [x, y, z] = _owner._moved.GetXYZ();
       auto radian = std::atan2(x, z);
       _owner._rotation.SetY(AppFrame::Math::Utility::RadianToDegree(radian));
   }
}
void Player::StateRun::Update() {
   _owner.Move(_owner._moved);
   _owner._collisionComponent->ObjectRangeFromPlayer();
   _owner._collisionComponent->GatlingFromPlayer();
   _owner.HitCheckFromGatling();
}

/// 攻撃
void Player::StateAttack::Enter() {
   /*_owner._forwardSpeed = 0.f;*/
   _owner._modelAnimeComponent->ChangeAnime("MO_SDChar_attack");
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

void Player::StateShootReady::Enter() {
    _owner._modelAnimeComponent->ChangeAnime("MO_SDChar_idle", true);
}

void Player::StateShootReady::Input(InputManager& input) {
    if (input.GetXJoypad().RBClick()) {
        _owner._stateServer->PopBack();
    }
}

void Player::StateShootReady::Update() {
    _owner.ShootRotate();
}

void Player::StateShootReady::Draw() {
    _owner._modelAnimeComponent->Draw();
}

void Player::StateKnockBack::Enter() {
    _owner._freezeTime = 20;
}

void Player::StateKnockBack::Input(InputManager& input) {

}

void Player::StateKnockBack::Update() {
    if (_owner._freezeTime > 0) {
        _owner.Move(_owner._knockBack);

        --_owner._freezeTime;
        return;
    }
    else {
        _owner._stateServer->PopBack();
    }
}

void Player::StateKnockBack::Draw() {
    _owner._modelAnimeComponent->Draw();
}

void Player::StateDie::Enter() {

}

void Player::StateDie::Input(InputManager& input) {

}

void Player::StateDie::Update() {
    
}

void Player::StateDie::Draw() {

}

