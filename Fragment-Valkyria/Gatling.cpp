
/*****************************************************************//**
 * \file   Gatling.cpp
 * \brief  ガトリング
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "Gatling.h"
#include "CollisionComponent.h"
#include "ModelAnimeComponent.h"
#include "ObjectServer.h"
#include "EffectGatlingMuzzleFlash.h"
#include "EffectGatlingBullet.h"
#include "EffectServer.h"

namespace {
   auto gatParamMap = AppFrame::Resource::LoadParamJson::GetParamMap("gatling",{ "speed" });
   const double Speed = gatParamMap["speed"];

   auto collParamMap = AppFrame::Resource::LoadParamJson::GetParamMap("collision", { "gatling_radius" });
   const double Radius = collParamMap["gatling_radius"];
}

using namespace FragmentValkyria::Enemy;

Gatling::Gatling(Game::GameMain& gameMain) : ObjectBase{ gameMain } {

}

void Gatling::Init() {
   // 移動する方向のベクトルの設定
   _moveDirection = GetObjServer().GetVecData("GatlingMoveDirection");
   // 移動する方向のベクトルの各成分を分解
   auto [x, y, z] = _moveDirection.GetVec3();
   // 移動する方向のベクトルの高さをなくす
   _moveDirection = Vector4(x, 0.0, z);
   // 単位化する
   _moveDirection.Normalized();
}

void Gatling::Update() {
   // 状態の更新
   _stateServer->Update();
   // ワールド行列の更新
   ComputeWorldTransform();
   
}

void Gatling::Draw() {
   // 各状態の描画処理を回す
   _stateServer->Draw();
}

void Gatling::Move() {
   // 移動量のベクトルの設定
   _moved = _moveDirection * Speed;
   // 位置の設定
   _position = _position + _moved;
}

void Gatling::HitCheckFromObjectModel() {
   // 当たり判定結果クラスの参照の取得
   auto report = _collisionComponent->report();
   // 当たり判定結果が落下オブジェクトと当たっていたら死亡状態へ
   if (report.id() == Collision::CollisionComponent::ReportId::HitFromIdleFallObject) {
      _stateServer->PushBack("Die");
   }
}

void Gatling::HitCheckFromPlayer() {
   // 当たり判定結果クラスの参照の取得
   auto report = _collisionComponent->report();
   // 当たり判定結果がプレイヤーと当たっていたら死亡状態へ
   if (report.id() == Collision::CollisionComponent::ReportId::HitFromPlayer) {
      _stateServer->PushBack("Die");
   }
}

void Gatling::OutStageCheck() {
   // 当たり判定結果クラスの参照の取得
   auto report = _collisionComponent->report();
   // 当たり判定結果がステージ外の場合は死亡状態へ
   if (report.id() == Collision::CollisionComponent::ReportId::OutStage) {
      _stateServer->PushBack("Die");
   }
}

void Gatling::StateBase::Draw() {
   _owner._efcGatling->Draw();
#ifdef _DEBUG
   // 位置を自作のVector4クラスからDxLib::VECTOR構造体への変換
   auto position = AppFrame::Math::ToDX(_owner._position);
   // 半径をfloat型にキャスト
   auto radian = static_cast<float>(Radius);
   // DxLibによる球の描画
   DrawSphere3D(position, radian, 10, GetColor(255, 0, 0), GetColor(0, 0, 0), FALSE);
#endif
}

void Gatling::StateChase::Enter() {
   auto efcMuzzleFlash = std::make_unique<Effect::EffectGatlingMuzzleFlash>(_owner._gameMain,"GatlingMuzzleFlash");
   efcMuzzleFlash->position(_owner._position);
   _owner.GetEfcServer().Add(std::move(efcMuzzleFlash));
   _owner._efcGatling = std::make_unique<Effect::EffectGatlingBullet>(_owner._gameMain, "GatlingBullet");
   _owner._efcGatling->Init();
   Update();
}

void Gatling::StateChase::Update() {
   // 移動処理
   _owner.Move();
   // 当たり判定処理を行うクラスでステージ外にいるか確認
   _owner._collisionComponent->OutStage();
   // プレイヤーと当たっているか確認
   _owner.HitCheckFromPlayer();
   // 落下オブジェクトのモデルと当たっているか確認
   _owner.HitCheckFromObjectModel();
   // ステージ外にいるか確認
   _owner.OutStageCheck();
   _owner._efcGatling->position(_owner._position);
   _owner._efcGatling->Update();
}

void Gatling::StateDie::Update() {
   // 死亡状態に設定
   _owner.SetDead();
   _owner._efcGatling->StopEffect();
}

void Gatling::StateDie::Draw() {
#ifdef _DEBUG
   // 位置を自作のVector4クラスからDxLib::VECTOR構造体への変換
   auto position = AppFrame::Math::ToDX(_owner._position);
   // 半径をfloat型にキャスト
   auto radian = static_cast<float>(Radius);
   // ブレンドモードをαブレンドに設定
   SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
   // DxLibによる球の描画
   DrawSphere3D(position, radian, 10, GetColor(255, 0, 0), GetColor(0, 0, 0), FALSE);
   // ブレンドモードをノーブレンドに設定
   SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
#endif
}

