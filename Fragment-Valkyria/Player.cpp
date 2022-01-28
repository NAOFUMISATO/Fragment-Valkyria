
/*****************************************************************//**
 * \file   Player.cpp
 * \brief  プレイヤーの処理を回すクラス
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "Player.h"
#include <cmath>
#include "ObjectServer.h"
#include "ObjectFactory.h"
#include "Bullet.h"
#include "CameraComponent.h"
#include "CollisionComponent.h"
#include "ModelAnimeComponent.h"
#include "GameMain.h"
using namespace FragmentValkyria::Player;

namespace {
    auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("player",
        { "movespeed", "capsulePos1", "capsulePos2", "capsuleRadian"});

    const double MoveSpeed = paramMap["movespeed"];
    const double CapsulePos1 = paramMap["capsulePos1"];
    const double CapsulePos2 = paramMap["capsulePos2"];
    const double CapsuleRadian = paramMap["capsuleRadian"];
}

Player::Player(Game::GameMain& gameMain) : ObjectBase{ gameMain } {
    Init();
}

void Player::Player::Init(){
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
   GetObjServer().RegistVector("PlayerPos", _position);
   GetObjServer().RegistVector("PlayerRot", _rotation);
   GetObjServer().RegistVector("PlayerFor",GetForward());

   auto handle = _modelAnimeComponent.get()->modelHandle();
   auto headFrame = _modelAnimeComponent.get()->FindFrameChild("SDChar_Root", "SDChar_Head");
   auto headPos = MV1GetFramePosition(handle, headFrame);
   GetObjServer().RegistVector("PlayerHeadPos", AppFrame::Math::ToMath(headPos));
   GetObjServer().RegistVector("CamTarget", _cameraComponent->GetTarget());
   GetObjServer().RegistVector("CamPos", _cameraComponent->GetPos());
}

void Player::Draw() {
   _stateServer->Draw();
}

void Player::ComputeWorldTransform() {
   auto [sx, sy, sz] = _scale.GetVec3();
   auto [rx, ry, rz] = _rotation.GetVec3();
   auto [px, py, pz] = _position.GetVec3();
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

    auto [x, y, z] = camForward.GetVec3();
    auto direction = Vector4(x, 0.0, z);
    auto radius = std::atan2(x, z);
    _rotation.SetY(AppFrame::Math::Utility::RadianToDegree(radius));

}

void Player::HitCheckFromFallObjectRange() {
    auto report = _collisionComponent->report();
    if (report.id() == Collision::CollisionComponent::ReportId::HitFromObjectRange) {
        _stateServer->PushBack("ShootReady");
        _cameraComponent->SetZoom(true);
    }
}

void Player::HitCheckFromIdleFallObject() {
    auto report = _collisionComponent->report();
    if (report.id() == Collision::CollisionComponent::ReportId::HitFromIdleFallObject) {
        auto normal = _collisionComponent->hitPos();
        auto moveVec = _moved * MoveSpeed;
        auto slideVec = moveVec.Cross(normal);
        slideVec = normal.Cross(slideVec);

        _position = _position + normal * MoveSpeed /*+ slideVec*/;

        _collisionComponent->report().id(Collision::CollisionComponent::ReportId::None);
    }
}

void Player::HitCheckFromGatling() {
    auto report = _collisionComponent->report();
    if (report.id() == Collision::CollisionComponent::ReportId::HitFromGatling) {
        auto hitPos = _collisionComponent->hitPos();
        auto knockBackVec = _position - hitPos;
        auto [x, y, z] = knockBackVec.GetVec3();
        auto knockBackDelta = Vector4(x, 0.0, z);
        knockBackDelta.Normalized();
        _knockBack = knockBackDelta * 10.0;

        _hp -= _collisionComponent->damage();

        _collisionComponent->report().id(Collision::CollisionComponent::ReportId::None);

        _stateServer->PushBack("KnockBack");
    }
}

void Player::HitCheckFromFallObject() {
    auto report = _collisionComponent->report();
    if (report.id() == Collision::CollisionComponent::ReportId::HitFromFallObject) {
        auto hitPos = _collisionComponent->hitPos();
        
        auto [hitX, hitY, hitZ] = hitPos.GetVec3();
        auto [posX, posY, posZ] = _position.GetVec3();
        if (hitX == posX && posZ == hitZ) {
            auto rotateY = _rotation.GetY();
            Matrix44 mat = Matrix44();
            mat.RotateY(rotateY, true);
            Vector4 vec = Vector4(0.0, 0.0, 1.0);
            vec.Normalized();
            auto knockBackDelta = vec * mat * -1.0;
            knockBackDelta.Normalized();
            _knockBack = knockBackDelta * 10.0;
        }
        else {
            auto knockBackVec = _position - hitPos;
            auto [x, y, z] = knockBackVec.GetVec3();
            auto knockBackDelta = Vector4(x, 0.0, z);
            knockBackDelta.Normalized();
            _knockBack = knockBackDelta * 10.0;
        }

        _hp -= _collisionComponent->damage();

        _collisionComponent->report().id(Collision::CollisionComponent::ReportId::None);

        _stateServer->PushBack("KnockBack");
    }
}

void Player::WeakAttack() {
    auto bullet = gameMain().objFactory().Create("Bullet");
    gameMain().objServer().Add(std::move(bullet));
}

void Player::StateBase::Draw() {
   _owner._modelAnimeComponent->Draw();
#ifdef _DEBUG
   auto pos1 = _owner._position + Vector4(0.0, CapsulePos1, 0.0);
   auto pos2 = _owner._position + Vector4(0.0, CapsulePos2, 0.0);
   auto radian = static_cast<float>(CapsuleRadian);
  
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
   else if (input.GetXJoypad().LBClick()) {
       _owner._stateServer->PushBack("WeakShootReady");
       _owner._cameraComponent->SetZoom(true);
   }
   if (input.GetXJoypad().XClick()) {
       _owner._stateServer->GoToState("Reload");
   }
}
void Player::StateIdle::Update() {
    _owner._collisionComponent->ObjectRangeFromPlayer();
    _owner._collisionComponent->GatlingFromPlayer();
    _owner.HitCheckFromFallObject();
    _owner.HitCheckFromGatling();
}

/// 走り
void Player::StateRun::Enter() {
   /*_owner._forwardSpeed = 10.0;*/
   _owner._modelAnimeComponent->ChangeAnime("MO_SDChar_run", true);
}
void Player::StateRun::Input(InputManager& input) {
    _owner.HitCheckFromFallObject();
    _owner.HitCheckFromIdleFallObject();

    auto moved = false;
    // カメラから前進方向単位ベクトルをもとめる
    auto camForward = _owner._cameraComponent->GetForward();
    auto [x, y, z] = camForward.GetVec3();
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
   else if (input.GetXJoypad().LBClick()) {
       _owner._stateServer->PushBack("WeakShootReady");
       _owner._cameraComponent->SetZoom(true);
   }
   if (input.GetXJoypad().XClick()) {
       _owner._stateServer->GoToState("Reload");
   }
   if (!moved) {
       _owner._stateServer->PopBack();
   } 
   else {
       _owner._moved.Normalized();
       _owner._moved = _owner._moved * MoveSpeed;
       auto [x, y, z] = _owner._moved.GetVec3();
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
   _owner._modelAnimeComponent->ChangeAnime("MO_SDChar_attack",false);
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
        _owner._cameraComponent->SetZoom(false);
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
        auto [x, y, z] = _owner._knockBack.GetVec3();
        auto radian = std::atan2(-x, -z);
        _owner._rotation.SetY(AppFrame::Math::Utility::RadianToDegree(radian));

        --_owner._freezeTime;
        return;
    }
    if (_owner._hp <= 0) {
        _owner._stateServer->GoToState("Die");
    }
    else {
        _owner._stateServer->PopBack();
    }
}

void Player::StateKnockBack::Draw() {
    _owner._modelAnimeComponent->Draw();
}

void Player::StateDie::Enter() {
    _owner.modelAnimeComponent().ChangeAnime("MO_SDChar_jumpStart", true);
    _timeOver = 60 * 2;
}

void Player::StateDie::Input(InputManager& input) {

}

void Player::StateDie::Update() {
    if (_timeOver > 0) {
        --_timeOver;
    }
    else {
        _owner.gameMain().modeServer().GoToMode("Title");
    }
}

void Player::StateDie::Draw() {
    _owner._modelAnimeComponent->Draw();
}

void Player::StateWeakShootReady::Enter() {
    _owner._modelAnimeComponent->ChangeAnime("MO_SDChar_idle", true);
    _coolTime = 0;
}

void Player::StateWeakShootReady::Input(InputManager& input) {
    if (input.GetXJoypad().RBClick() && _coolTime <= 0 && _owner._bulletStock > 0) {
        _owner.WeakAttack();
        --_owner._bulletStock;
        _coolTime = 60 * 3;
    }
    if (input.GetXJoypad().LBClick()) {
        _owner._stateServer->PopBack();
        _owner._cameraComponent->SetZoom(false);
    }
}

void Player::StateWeakShootReady::Update() {
    _owner.ShootRotate();

    --_coolTime;
}

void Player::StateReload::Enter() {
    _owner._modelAnimeComponent->ChangeAnime("MO_SDChar_idle", true);
    _reloadCnt = 0;
}

void Player::StateReload::Input(InputManager& input) {

}

void Player::StateReload::Update() {
    if (_reloadCnt > 60) {
        _reloadCnt = 0;
        ++_owner._bulletStock;
        if (_owner._bulletStock > 5) {
            _owner._stateServer->GoToState("Idle");
        }
    }

    ++_reloadCnt;
}
