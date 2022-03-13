#include "PoorEnemyBase.h"
#include "CollisionComponent.h"
#include "ModelAnimeComponent.h"
#include "ObjectFactory.h"
#include "GameMain.h"
#include "ObjectServer.h"
#include "EffectPoorCrash.h"
#include "EffectServer.h"

using namespace FragmentValkyria::Enemy;

namespace {
   auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("poorenemy",
      { "gravity", "rotate_speed" ,"step_distance","step_speed","idle_animespeed",
      "walk_animespeed","fall_animespeed","die_animespeed","white_frame" });

   const double Gravity = paramMap["gravity"];
   const double RotateSpeed = paramMap["rotate_speed"];
   const double StepDistance = paramMap["step_distance"];
   const double StepSpeed = paramMap["step_speed"];
   const double IdleAnimeSpeed = paramMap["idle_animespeed"];
   const double WalkAnimeSpeed = paramMap["walk_animespeed"];
   const double FallAnimeSpeed = paramMap["fall_animespeed"];
   const double DieAnimeSpeed = paramMap["die_animespeed"];
   const int WhiteFrame = paramMap["white_frame"];

   constexpr auto OneFrame = 60;
   constexpr auto DegreeQuarter = 90;
   constexpr auto StepDistanceLimit = 100.0;
   constexpr auto MaxOpacityRate = 1.0f;
   constexpr auto FadeOutFrame = 60;
}

PoorEnemyBase::PoorEnemyBase(Game::GameMain& gameMain) : Object::ObjectBase{ gameMain } {
}

void PoorEnemyBase::Init() {
   auto modelHandle = _modelAnimeComponent->modelHandle();
   _collNum = _modelAnimeComponent->FindFrame("mob");
   // フレーム1をナビメッシュとして使用
   MV1SetupCollInfo(modelHandle, _collNum, 1, 1, 1);
   _actionList.emplace_back("SideStep");
   _hp = 100.0;
}

void PoorEnemyBase::Update() {
   //コリジョン情報の更新
   MV1RefreshCollInfo(_modelAnimeComponent->modelHandle(), _collNum);
   _collisionComponent->BulletFromPoorEnemy();
   _collisionComponent->PoorEnemyFromPlayer();
   HitCheckFromBullet();
   HitCheckFromFallObject();
   DamageExpression();
   // 状態の更新
   _stateServer->Update();
   // ワールド行列の更新
   ComputeWorldTransform();
   // モデルの更新
   _modelAnimeComponent->Update();
}

void PoorEnemyBase::Draw() {
   _stateServer->Draw();
}

void PoorEnemyBase::Rotate() {
   auto [x, y, z] = _position.GetVec3();
   auto toPlayer = _gameMain.objServer().GetVecData("PlayerPos") - Vector4(x, 0.0, z);
   toPlayer.Normalized();
   AppFrame::Math::Matrix44 rotate;
   rotate.RotateY(_rotation.GetY(), true);

   auto forward = Vector4(0.0, 0.0, -1.0) * rotate;
   auto addRotate = 0.5 * forward.Cross(toPlayer).GetY();
   _rotation.Add(Vector4(0.0, addRotate * RotateSpeed, 0.0));
}

void PoorEnemyBase::HitCheckFromBullet() {
   auto report = _collisionComponent->report();
   if (report.id() == Collision::CollisionComponent::ReportId::HitFromBullet) {

      _hp -= _collisionComponent->damage();

      _collisionComponent->report().id(Collision::CollisionComponent::ReportId::None);

      if (_hp <= 0) {
         _stateServer->GoToState("Die");
      }
      else {
         _damageCnt = _gameMain.modeServer().frameCount() - _damageCnt;
      }
   }
}

void PoorEnemyBase::HitCheckFromFallObject() {
   auto report = _collisionComponent->report();
   if (report.id() == Collision::CollisionComponent::ReportId::HitFromFallObject) {

      _hp -= _collisionComponent->damage();

      _collisionComponent->report().id(Collision::CollisionComponent::ReportId::None);

      if (_hp <= 0) {
         _stateServer->GoToState("Die");
      }
   }
}

void PoorEnemyBase::DamageExpression() {
   auto frame = static_cast<int>(_gameMain.modeServer().frameCount() - _damageCnt);
   if (frame < WhiteFrame) {
      _modelAnimeComponent->SetEmiColor(0, 1.0f, 1.0f, 1.0f);
      _modelAnimeComponent->SetEmiColor(1, 1.0f, 1.0f, 1.0f);
   }
   else {
      _modelAnimeComponent->SetEmiColor(0, 0.0f, 0.0f, 0.0f);
      _modelAnimeComponent->SetEmiColor(1, 0.0f, 0.0f, 0.0f);
   }
}

void PoorEnemyBase::StateBase::Draw() {
   _owner._modelAnimeComponent->Draw();
}

void PoorEnemyBase::StateIdle::Enter() {
   _owner._modelAnimeComponent->ChangeAnime("idle", true, IdleAnimeSpeed);
   _stateCnt = _owner._gameMain.modeServer().frameCount();
}

void PoorEnemyBase::StateIdle::Update() {
   auto frame = _owner._gameMain.modeServer().frameCount() - _stateCnt;
   _owner.Rotate();
   // 一定フレーム数たったら残っている行動のなかからランダムに行動を選びその行動をする
   if (frame >= 60 * 5) {
      if (_owner._action.empty()) {
         _owner._action = _owner._actionList;
      }
      auto random = AppFrame::Math::Utility::GetRandom(0, static_cast<int>(_owner._action.size()) - 1);
      _owner._stateServer->GoToState(_owner._action[random]);
      _owner._action.erase(_owner._action.begin() + random);
   }
}

void PoorEnemyBase::StateSideStep::Enter() {
   _owner._modelAnimeComponent->ChangeAnime("walk", true, WalkAnimeSpeed);
   SideStepDecide();
}

void PoorEnemyBase::StateSideStep::Update() {
   auto length = (_moveOnPos - _owner._position).Lenght();
   if (length <= StepDistanceLimit){
      _owner._stateServer->GoToState("Idle");
   }
   _owner._position = _owner._position + _moveOnDir * StepSpeed;
}

void PoorEnemyBase::StateFall::Enter() {
   _owner._modelAnimeComponent->ChangeAnime("idle", true, FallAnimeSpeed);
   _stateCnt = 0;
}

void PoorEnemyBase::StateFall::Update() {
   auto y = 0.5 * Gravity * _stateCnt * _stateCnt;
   _owner._position.Add(Vector4(0.0, y, 0.0));
   _owner.Rotate();

   if (_owner.position().GetY() <= 0.0) {
      _owner.position().SetY(0.0);
      _owner._stateServer->GoToState("Idle");
   }
   _stateCnt++;
 }

void PoorEnemyBase::StateDie::Enter() {
   _owner._modelAnimeComponent->ChangeAnime("idle", true, DieAnimeSpeed);
   _stateCnt = _owner._gameMain.modeServer().frameCount();
   _opacityRate = MaxOpacityRate;
   auto efcCrash = std::make_unique<Effect::EffectPoorCrash>(_owner._gameMain, "PoorCrash");
   efcCrash->position(_owner._position);
   _owner.GetEfcServer().Add(std::move(efcCrash));
}

void PoorEnemyBase::StateDie::Update() {
   auto frame = _owner._gameMain.modeServer().frameCount() - _stateCnt;
   
   auto deltaRate = MaxOpacityRate / static_cast<float>(FadeOutFrame);

   _opacityRate -= deltaRate;
   if (frame > FadeOutFrame) {
      _owner.SetDead();
   }
}

void PoorEnemyBase::StateDie::Draw() {
   auto handle = _owner._modelAnimeComponent->modelHandle();
   MV1SetOpacityRate(handle, _opacityRate);
   StateBase::Draw();
}

void PoorEnemyBase::StateSideStep::SideStepDecide() {
   auto moved = _owner.GetObjServer().GetVecData("PlayerPos") - _owner._position;
   moved.Normalized();
   auto quarterRight = AppFrame::Math::Matrix44();
   auto quarterLeft = AppFrame::Math::Matrix44();
   quarterRight.RotateY(DegreeQuarter, false);
   quarterLeft.RotateY(-DegreeQuarter, false);
   auto rightDir = moved * quarterRight;
   auto leftDir = moved * quarterLeft;
   auto rightPos = _owner._position + rightDir * StepDistance;
   auto leftPos = _owner._position + leftDir * StepDistance;
   auto isRightSafeZone = _owner._collisionComponent->IsLineFromStage(rightPos);
   auto isLeftSafeZone = _owner._collisionComponent->IsLineFromStage(leftPos);
   auto random = AppFrame::Math::Utility::GetRandom(0, 1);
   if (random == 0) {
      if (isRightSafeZone) {
         _moveOnDir = rightDir;
         _moveOnPos = rightPos;
      }
      else if (isLeftSafeZone) {
         _moveOnDir = leftDir;
         _moveOnPos = leftPos;
      }
      else {
         _owner._stateServer->GoToState("Idle");
      }
   }
   if (random == 1) {
      if (isLeftSafeZone) {
         _moveOnDir = leftDir;
         _moveOnPos = leftPos;
      }
      else if (isRightSafeZone) {
         _moveOnDir = rightDir;
         _moveOnPos = rightPos;
      }
      else {
         _owner._stateServer->GoToState("Idle");
      }
   }
}