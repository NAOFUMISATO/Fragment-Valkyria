
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
    auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("player",{
       "idle_animespeed","walk_animespeed","run_animespeed","shootready_animespeed","shoot_animespeed",
       "move_speed", "capsule_pos1", "capsule_pos2", "capsule_radius"});
    const double IdleAnimeSpeed = paramMap["idle_animespeed"];
    const double WalkAnimeSpeed = paramMap["walk_animespeed"];
    const double RunAnimeSpeed = paramMap["run_animespeed"];
    const double ShootReadyAnimeSpeed = paramMap["shootready_animespeed"];
    const double ShootAnimeSpeed = paramMap["shoot_animespeed"];
    const double MoveSpeed = paramMap["move_speed"];
    const double CapsulePos1 = paramMap["capsule_pos1"];
    const double CapsulePos2 = paramMap["capsule_pos2"];
    const double CapsuleRadius = paramMap["capsule_radius"];

    constexpr auto FootStepHeight = 3.0;   //!< 走り状態時の足音発生高さ(足の甲からの位置)
    constexpr auto FootStepStart = 10;     //!< 走り状態遷移時からの足音未発生フレーム
}

Player::Player(Game::GameMain& gameMain) : ObjectBase{ gameMain } {
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

   auto handle = modelAnimeComponent().modelHandle();
   auto headFrame = modelAnimeComponent().FindFrameChild("Kamilla_kari_Reference", "Kamilla_kari_Head");
   auto headPos = MV1GetFramePosition(handle, headFrame);
   GetObjServer().RegistVector("PlayerHeadPos", AppFrame::Math::ToMath(headPos));
   GetObjServer().RegistVector("CamTarget", _cameraComponent->GetTarget());
   GetObjServer().RegistVector("CamPos", _cameraComponent->GetPos());
   GetObjServer().RegistDouble("PlayerHP",_hp);
   GetObjServer().RegistDouble("PlayerBulletStock",static_cast<double>(_bulletStock));
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

        _collisionComponent->knockBack(true);

        _collisionComponent->report().id(Collision::CollisionComponent::ReportId::None);

        _cameraComponent->SetZoom(false);

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

        _collisionComponent->knockBack(true);

        _collisionComponent->report().id(Collision::CollisionComponent::ReportId::None);

        _cameraComponent->SetZoom(false);

        _stateServer->PushBack("KnockBack");
    }
}

void Player::HitCheckFromLaser() {
    auto report = _collisionComponent->report();

    if (report.id() == Collision::CollisionComponent::ReportId::HitFromLaser) {
        auto hitPos = _collisionComponent->hitPos();
        auto knockBackVec = _position - hitPos;
        auto [x, y, z] = knockBackVec.GetVec3();
        auto knockBackDelta = Vector4(x, 0.0, z);
        knockBackDelta.Normalized();
        _knockBack = knockBackDelta * 20.0;

        _hp -= _collisionComponent->damage();

        _collisionComponent->knockBack(true);

        _collisionComponent->report().id(Collision::CollisionComponent::ReportId::None);

        _cameraComponent->SetZoom(false);

        _stateServer->PushBack("KnockBack");
    }
}

void Player::HitCheckFromLargeEnemy() {
    auto report = _collisionComponent->report();

    if (report.id() == Collision::CollisionComponent::ReportId::HitFromLargeEnemy) {
        auto hitPos = _collisionComponent->hitPos();
        auto knockBackVec = _position - hitPos;
        knockBackVec.Normalized();
        _knockBack = knockBackVec * 10.0;

        _hp -= _collisionComponent->damage();

        _collisionComponent->knockBack(true);

        _cameraComponent->SetZoom(false);

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
   auto radian = static_cast<float>(CapsuleRadius);
  
   DrawCapsule3D(AppFrame::Math::ToDX(pos1), AppFrame::Math::ToDX(pos2), radian, 20, GetColor(0, 255, 0), GetColor(0, 0, 0), FALSE);
#endif

}
/// 待機
void Player::StateIdle::Enter() {
   /*_owner._forwardSpeed = 0.0;*/
   _owner._modelAnimeComponent->ChangeAnime("taiki_MO", true, IdleAnimeSpeed);
}
void Player::StateIdle::Input(InputManager& input) {

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
   if (input.GetXJoypad().XClick() && _owner._bulletStock < 5) {
       _owner._stateServer->GoToState("Reload");
   }
}
void Player::StateIdle::Update() {
    _owner._collisionComponent->ObjectRangeFromPlayer();
    _owner.HitCheckFromIdleFallObject();
    
    _owner._collisionComponent->GatlingFromPlayer();
    _owner._collisionComponent->PlayerFromLaser();
    _owner.HitCheckFromLargeEnemy();
    _owner.HitCheckFromFallObject();
    _owner.HitCheckFromGatling();
    _owner.HitCheckFromLaser();
    
    //無敵時間更新
    --_owner._invincibleCnt;
}

/// 走り
void Player::StateRun::Enter() {
   _owner._modelAnimeComponent->ChangeAnime("run_motion", true, RunAnimeSpeed);
   auto count = _owner.gameMain().modeServer().frameCount();
   _footCnt = count;
}
void Player::StateRun::Input(InputManager& input) {
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
   if (input.GetXJoypad().XClick() && _owner._bulletStock < 5) {
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
   FootStepSound();
   _owner.Move(_owner._moved);
   _owner._collisionComponent->ObjectRangeFromPlayer();
   _owner._collisionComponent->GatlingFromPlayer();
   _owner._collisionComponent->PlayerFromLaser();
   _owner.HitCheckFromFallObject();
   _owner.HitCheckFromLargeEnemy();
   _owner.HitCheckFromGatling();
   _owner.HitCheckFromLaser();

   //無敵時間更新
   --_owner._invincibleCnt;
}

/// 攻撃
void Player::StateAttack::Enter() {
   /*_owner._forwardSpeed = 0.f;*/
   _owner._modelAnimeComponent->ChangeAnime("hassya_MO",false, ShootAnimeSpeed);
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
    _owner._modelAnimeComponent->ChangeAnime("kamae_MO", true, ShootReadyAnimeSpeed);
    _owner.GetSoundComponent().Play("PlayerShootReady");
}

void Player::StateShootReady::Input(InputManager& input) {
    if (input.GetXJoypad().RBClick()) {
       _owner.GetSoundComponent().Play("PlayerShoot");
       _owner._modelAnimeComponent->ChangeAnime("hassya_MO", false, ShootAnimeSpeed);
        _owner._stateServer->PopBack();
        _owner._cameraComponent->SetZoom(false);
    }
}

void Player::StateShootReady::Update() {
    _owner.ShootRotate();
   
    _owner._collisionComponent->GatlingFromPlayer();
    _owner._collisionComponent->PlayerFromLaser();
    _owner.HitCheckFromLargeEnemy();
    _owner.HitCheckFromFallObject();
    _owner.HitCheckFromGatling();
    _owner.HitCheckFromLaser();

    //無敵時間更新
    --_owner._invincibleCnt;
   
}

void Player::StateShootReady::Draw() {
    _owner._modelAnimeComponent->Draw();
}

void Player::StateKnockBack::Enter() {
    _owner._freezeTime = 60 * 0.5;
}

void Player::StateKnockBack::Input(InputManager& input) {

}

void Player::StateKnockBack::Update() {
    if (_owner._freezeTime > 0) {
        _owner.Move(_owner._knockBack);
        auto [x, y, z] = _owner._knockBack.GetVec3();
        auto radius = std::atan2(-x, -z);
        _owner._rotation.SetY(AppFrame::Math::Utility::RadianToDegree(radius));

        --_owner._freezeTime;
        return;
    }
    if (_owner._hp <= 0) {
        _owner._stateServer->GoToState("Die");
    }
    else {
        _owner._invincibleCnt = 60 * 2;
        _owner.collisionComponent().knockBack(false);
        _owner.collisionComponent().report().id(Collision::CollisionComponent::ReportId::None);
        _owner._stateServer->GoToState("Idle");
    }
}

void Player::StateKnockBack::Draw() {
    _owner._modelAnimeComponent->Draw();
}

void Player::StateDie::Enter() {
    _owner.modelAnimeComponent().ChangeAnime("run_motion", true,10.0);
    _timeOver = 60 * 2;
}

void Player::StateDie::Input(InputManager& input) {

}

void Player::StateDie::Update() {
    if (_timeOver > 0) {
        --_timeOver;
    }
    else {
        _owner.gameMain().modeServer().PushBack("MissionFailed");
    }
}

void Player::StateDie::Draw() {
    _owner._modelAnimeComponent->Draw();
}

void Player::StateWeakShootReady::Enter() {
    _owner._modelAnimeComponent->ChangeAnime("kamae_MO", true);
    _owner.GetSoundComponent().Play("PlayerShootReady");
    _coolTime = 0;
}

void Player::StateWeakShootReady::Input(InputManager& input) {
    if (input.GetXJoypad().RBClick() && _coolTime <= 0 && _owner._bulletStock > 0) {
        _owner.WeakAttack();
        _owner._modelAnimeComponent->ChangeAnime("hassya_MO", false, ShootAnimeSpeed);
        _owner.GetSoundComponent().Play("PlayerShoot");
        --_owner._bulletStock;
        _coolTime = 60 * 1;

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
    _owner._modelAnimeComponent->ChangeAnime("walk_MO", true);
    _reloadCnt = 0;
}

void Player::StateReload::Input(InputManager& input) {

}

void Player::StateReload::Update() {
    if (_reloadCnt > 60 * 2) {
        _owner._bulletStock = 5;
        _owner._stateServer->GoToState("Idle");
    }
    ++_reloadCnt;
}

void Player::StateRun::FootStepSound() {
   auto count = _owner.gameMain().modeServer().frameCount();
   if (count - _footCnt >= FootStepStart) {
      auto handle = _owner.modelAnimeComponent().modelHandle();
      auto rightFootFrame = _owner.modelAnimeComponent().FindFrameChild("Kamilla_kari_Reference", "Kamilla_kari_RightToeBase");
      auto leftFootFrame = _owner.modelAnimeComponent().FindFrameChild("Kamilla_kari_Reference", "Kamilla_kari_LeftToeBase");
      auto rightFootPos = MV1GetFramePosition(handle, rightFootFrame);
      auto leftFootPos = MV1GetFramePosition(handle, leftFootFrame);
      auto rightFootY = AppFrame::Math::ToMath(rightFootPos).GetY();
      auto leftFootY = AppFrame::Math::ToMath(leftFootPos).GetY();
      if (rightFootY >= FootStepHeight) {
         _footRightStep = true;
      }
      else {
         if (_footRightStep) {
            auto& soundServer = _owner.GetSoundComponent();
            soundServer.Play("PlayerRightFootStep");
            _footRightStep = false;
         }
      }
      if (leftFootY >= FootStepHeight) {
         _footLeftStep = true;
      }
      else {
         if (_footLeftStep) {
            auto& soundServer = _owner.GetSoundComponent();
            soundServer.Play("PlayerLeftFootStep");
            _footLeftStep = false;
         }
      }
   }
}
