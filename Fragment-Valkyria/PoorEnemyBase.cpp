
/*****************************************************************//**
 * \file   PoorEnemyBase.cpp
 * \brief  雑魚敵の基底クラス
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "PoorEnemyBase.h"
#include "CollisionComponent.h"
#include "ModelAnimeComponent.h"
#include "ObjectFactory.h"
#include "Game.h"
#include "ObjectServer.h"
#include "EffectPoorCrash.h"
#include "EffectServer.h"
#include "ParamPoorEnemy.h"

using namespace FragmentValkyria::Enemy;

namespace {
   constexpr auto OneFrame = 60;
   constexpr auto DegreeQuarter = 90;
   constexpr auto StepDistanceLimit = 100.0;
   constexpr auto MaxOpacityRate = 1.0f;
   constexpr auto FadeOutFrame = 60;
   constexpr auto RandomSec = 5;
}

PoorEnemyBase::PoorEnemyBase() {
   _param = std::make_unique<Param::ParamPoorEnemy>("poorenemy");
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
   auto& objServer = Game::Game::GetInstance().objServer();
   auto toPlayer = objServer.GetVecData("PlayerPos") - Vector4(x, 0.0, z);
   toPlayer.Normalized();
   AppFrame::Math::Matrix44 rotate;
   rotate.RotateY(_rotation.GetY(), true);

   auto forward = Vector4(0.0, 0.0, -1.0) * rotate;
   auto addRotate = 0.5 * forward.Cross(toPlayer).GetY();
   _rotation.Add(Vector4(0.0, addRotate * _param->GetDoubleParam("rotate_speed"), 0.0));
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
         auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
         _damageCnt = modeServer.frameCount();
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
   auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
   auto frame = static_cast<int>(modeServer.frameCount() - _damageCnt);
   if (frame < _param->GetIntParam("white_frame")) {
      _modelAnimeComponent->SetBlendModeAdd(0);
      _modelAnimeComponent->SetBlendModeAdd(1);
   }
   else {
      _modelAnimeComponent->SetBlendModeAlpha(0);
      _modelAnimeComponent->SetBlendModeAlpha(1);
   }
}

void PoorEnemyBase::StateBase::Update() {
   _owner.HitCheckFromBullet();
   _owner.HitCheckFromFallObject();
}

void PoorEnemyBase::StateBase::Draw() {
   _owner._modelAnimeComponent->Draw();
}

void PoorEnemyBase::StateIdle::Enter() {
   _owner._modelAnimeComponent->ChangeAnime("idle", true,
      _owner._param->GetDoubleParam("idle_animespeed"));
   auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
   _stateCnt = modeServer.frameCount();
}

void PoorEnemyBase::StateIdle::Update() {
   StateBase::Update();
   auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
   auto frame = modeServer.frameCount() - _stateCnt;
   _owner.Rotate();
   // 一定フレーム数たったら残っている行動のなかからランダムに行動を選びその行動をする
   if (frame >= OneFrame * RandomSec) {
      if (_owner._action.empty()) {
         _owner._action = _owner._actionList;
      }
      auto random = AppFrame::Math::Utility::GetRandom(0, static_cast<int>(_owner._action.size()) - 1);
      _owner._stateServer->GoToState(_owner._action[random]);
      _owner._action.erase(_owner._action.begin() + random);
   }
}

void PoorEnemyBase::StateSideStep::Enter() {
   _owner._modelAnimeComponent->ChangeAnime("walk", true, 
      _owner._param->GetDoubleParam("walk_animespeed"));
   SideStepDecide();
}

void PoorEnemyBase::StateSideStep::Update() {
   StateBase::Update();
   auto length = (_moveOnPos - _owner._position).Lenght();
   if (length <= StepDistanceLimit){
      _owner._stateServer->GoToState("Idle");
   }
   _owner._position = 
      _owner._position + _moveOnDir * _owner._param->GetDoubleParam("step_speed");
}

void PoorEnemyBase::StateFall::Enter() {
   _owner._modelAnimeComponent->ChangeAnime("idle", true, 
      _owner._param->GetDoubleParam("fall_animespeed"));
   _stateCnt = 0;
}

void PoorEnemyBase::StateFall::Update() {
   StateBase::Update();
   auto y = 0.5 * _owner._param->GetDoubleParam("gravity") * _stateCnt * _stateCnt;
   _owner._position.Add(Vector4(0.0, y, 0.0));
   _owner.Rotate();

   if (_owner.position().GetY() <= 0.0) {
      _owner.position().SetY(0.0);
      _owner._stateServer->GoToState("Idle");
   }
   _stateCnt++;
 }

void PoorEnemyBase::StateDie::Enter() {
   _owner._modelAnimeComponent->ChangeAnime("idle", true,
      _owner._param->GetDoubleParam("die_animespeed"));
   auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
   _stateCnt = modeServer.frameCount();
   _opacityRate = MaxOpacityRate;
   auto efcCrash = std::make_unique<Effect::EffectPoorCrash>("PoorCrash");
   efcCrash->position(_owner._position);
   auto& gameInstance = Game::Game::GetInstance();
   gameInstance.efcServer().Add(std::move(efcCrash));
   gameInstance.soundComponent().Play("PoorCrash",_owner._position);
}

void PoorEnemyBase::StateDie::Update() {
   auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
   auto frame = modeServer.frameCount() - _stateCnt;
   
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
   auto& objServer = Game::Game::GetInstance().objServer();
   auto moved = objServer.GetVecData("PlayerPos") - _owner._position;
   auto [x, y, z] = moved.GetVec3();
   moved = Vector4(x, 0.0, z);
   moved.Normalized();
   auto quarterRight = AppFrame::Math::Matrix44();
   auto quarterLeft = AppFrame::Math::Matrix44();
   quarterRight.RotateY(DegreeQuarter, false);
   quarterLeft.RotateY(-DegreeQuarter, false);
   auto rightDir = moved * quarterRight;
   auto leftDir = moved * quarterLeft;
   const auto StepDistance = _owner._param->GetDoubleParam("step_distance");
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