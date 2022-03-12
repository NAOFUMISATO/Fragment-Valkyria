
/*****************************************************************//**
 * \file   FallObject.cpp
 * \brief  落下オブジェクトの処理を回すクラス
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "FallObject.h"
#include "CollisionComponent.h"
#include "GameMain.h"
#include "Player.h"
#include "ModelAnimeComponent.h"
#include "EffectObjectFall.h"
#include "ObjectServer.h"
#include "EffectServer.h"

using namespace FragmentValkyria::Enemy;

namespace {
   auto fallParamMap = AppFrame::Resource::LoadParamJson::GetParamMap("fallobject",
      { "gravity", "shoot_speed", "up_speed", "rotate_angle","updown_range", "right_hand_up_value", 
      "up_value","fallpoint_pos_y","fallpoint_scale","fallpoint_animespeed" });
   const double Gravity = fallParamMap["gravity"];
   const double ShootSpeed = fallParamMap["shoot_speed"];
   const double UpSpeed = fallParamMap["up_speed"];
   const double RotateAngle = fallParamMap["rotate_angle"];
   const double UpDownRange = fallParamMap["updown_range"];
   const double RightHandUpValue = fallParamMap["right_hand_up_value"];
   const double UpValue = fallParamMap["up_value"];
   const double FallPointPosY = fallParamMap["fallpoint_pos_y"];
   const double FallPointScale = fallParamMap["fallpoint_scale"];
   const int FallPointAnimeSpeed = fallParamMap["fallpoint_animespeed"];

   auto collParamMap = AppFrame::Resource::LoadParamJson::GetParamMap("collision", { "fallobject_range" ,"fallobject_drum_capsule_pos1",
      "fallobject_drum_capsule_pos2", "fallobject_drum_radius" });
   const double Range = collParamMap["fallobject_range"];
   const double DrumCapsulePos1 = collParamMap["fallobject_drum_capsule_pos1"];
   const double DrumCapsulePos2 = collParamMap["fallobject_drum_capsule_pos2"];
   const double DrumCapsuleRadius = collParamMap["fallobject_drum_radius"];

   constexpr auto DefaultPointScale = 1.0;
   constexpr auto DefaultPointAngle = 0.0;
   constexpr auto UpEffectDiffY = 20.0;
}

FallObject::FallObject(Game::GameMain& gameMain) : ObjectBase{ gameMain } {

}

void FallObject::Init() {
   auto modelHandle = _modelAnimeComponent->modelHandle();
   _collision = MV1SearchFrame(modelHandle, _collisionName.data());
   // ナビメッシュの表示設定
#ifndef _DEBUG
   MV1SetFrameVisible(modelHandle, _collision, false);
#else
   MV1SetFrameOpacityRate(modelHandle, _collision, 0.5f);
#endif

   // フレーム1をナビメッシュとして使用
   MV1SetupCollInfo(modelHandle, _collision);

   _fallPointHandles = _gameMain.resServer().GetTextures("FallPoint");
}

void FallObject::Input(InputManager& input) {
   _stateServer->Input(input);
}

void FallObject::Update() {
   //コリジョン情報の更新
   MV1RefreshCollInfo(_modelAnimeComponent->modelHandle(), _collision);
   // 状態の更新
   _stateServer->Update();
   // ワールド行列の更新
   ComputeWorldTransform();
   // モデルの更新
   _modelAnimeComponent->Update();
}

void FallObject::Draw() {
   _stateServer->Draw();
}

void FallObject::HitCheckFromPlayerPoint() {
   // 当たり判定結果クラスの参照の取得
   auto report = _collisionComponent->report();
   // 当たり判定結果の確認
   if (report.id() == Collision::CollisionComponent::ReportId::HitFromPlayer) {
      // プレイヤーと当たっていたら上昇状態へ
      _stateServer->GoToState("Up");
   }
}

void FallObject::HitCheckFromLargeEnemy() {
   // 当たり判定結果クラスの参照の取得
   auto report = _collisionComponent->report();
   // 当たり判定結果の確認
   if (report.id() == Collision::CollisionComponent::ReportId::HitFromLargeEnemy) {
      // ラージエネミーと当たっていたら死亡状態へ
      _stateServer->GoToState("Die");
   }
}

void FallObject::HitCheckFromLaser() {
   // 当たり判定結果クラスの参照の取得
   auto report = _collisionComponent->report();
   // 当たり判定結果の確認
   if (report.id() == Collision::CollisionComponent::ReportId::HitFromLaser) {
      // レーザーと当たっていたら死亡状態へ
      _stateServer->GoToState("Die");
   }
}

void FallObject::HitCheckFromPoorEnemyGatling() {
   // 当たり判定結果クラスの参照の取得
   auto report = _collisionComponent->report();
   // 当たり判定結果の確認
   if (report.id() == Collision::CollisionComponent::ReportId::HitFromPoorEnemyGatling) {
      // 雑魚敵と当たったら死亡状態へ
      _stateServer->GoToState("Die");
   }
}

void FallObject::OutStageCheck() {
   // 当たり判定結果クラスの参照の取得
   auto report = _collisionComponent->report();
   // 当たり判定結果の確認
   if (report.id() == Collision::CollisionComponent::ReportId::OutStage) {
      // ステージの外にいたら死亡状態へ
      _stateServer->GoToState("Die");
   }
}

void FallObject::CheckPlayerKnockBack() {
   // 当たり判定結果クラスからプレイヤーがノックバックしているか取得
   auto result = _collisionComponent->knockBack();
   // ノックバックしていたら死亡状態へ
   if (result) {
      _stateServer->GoToState("Die");
   }
}

void FallObject::Save() {
   // 不規則な回転をさせるために使うサインの値を取得するために使う角度の更新
   _rotateAngle += 1.0;
   // 角度をラジアンにする
   auto radian = AppFrame::Math::Utility::DegreeToRadian(_rotateAngle);
   // サインの値を取得
   auto sinValue = std::sin(radian);
   // 既定の角度の-1倍から1倍の範囲で角度を決める
   auto xyz = RotateAngle * sinValue;
   // 回転のベクトルを設定
   _rotation = Vector4(xyz, xyz, xyz);
   // オブジェクト一括管理クラスのオブジェクトの取得
   for (auto&& object : GetObjServer().runObjects()) {
      // プレイヤーじゃない場合処理をスキップしてfor文を回す
      if (object->GetObjType() != Object::ObjectBase::ObjectType::Player) {
         continue;
      }
      // プレイヤーの参照型にキャスト
      auto& player = dynamic_cast<Player::Player&>(*object);
      // プレイヤーの右手の座標の取得
      _plyRightHandVec = player.modelAnimeComponent().GetFrameChildPosion("Kamilla_kari_Reference", "Kamilla_kari_RightFingerBase");
      // プレイヤーの右手の座標へのベクトルの取得
      auto toRightHand = _plyRightHandVec - _position;
      // 単位化する
      toRightHand.Normalized();
      // 高さを無くす
      auto [x, y, z] = toRightHand.GetVec3();
      auto moveVec = Vector4(x, 0.0, z);
      // 位置を高さを無くしたプレイヤーの右手の座標へ近づける
      _position = _position + moveVec * UpSpeed;
      // for文を抜ける
      break;
   }
   // 上下にふわふわさせるために使用するサインの値を取得するための角度の更新
   _upDownAngle += 2.0;
   // 角度をラジアンにする
   auto upDownRadian = AppFrame::Math::Utility::DegreeToRadian(_upDownAngle);
   // サインの値を取得
   auto upDownSinValue = std::sin(upDownRadian);
   // 既定の値の-1倍から1倍の範囲で足していく高さを決める
   auto y = UpDownRange * upDownSinValue;
   // 高さの更新
   _position.Add(0.0, y, 0.0);
}

void FallObject::Up() {
   // オブジェクト一括管理クラスのオブジェクトの取得
   for (auto&& object : GetObjServer().runObjects()) {
      // プレイヤーじゃない場合処理をスキップしてfor文を回す
      if (object->GetObjType() != Object::ObjectBase::ObjectType::Player) {
         continue;
      }
      // プレイヤーの参照型にキャスト
      auto& player = dynamic_cast<Player::Player&>(*object);
      // プレイヤーの右手の座標の取得
      _plyRightHandVec = player.modelAnimeComponent().GetFrameChildPosion("Kamilla_kari_Reference", "Kamilla_kari_RightFingerBase");
      // for文を抜ける
      break;
   }
   // 位置の高さが既定の値より大きい場合
   if (_position.GetY() >= UpValue) {
      // プレイヤーの右手の座標のY成分に既定の値足す
      _plyRightHandVec.Add(0.0, RightHandUpValue, 0.0);
      // プレイヤーの右手の座標に既定の値高さを足した位置へのベクトルの取得
      auto moveVec = _plyRightHandVec - _position;
      // 単位化する
      moveVec.Normalized();
      // 位置をプレイヤーの右手の座標に既定の値高さを足した位置へ近づける
      _position = _position + moveVec * UpSpeed;
      // 高さがプレイヤーの右手の座標に既定の値高さを足した位置の高さ以上の場合
      if (_position.GetY() >= _plyRightHandVec.GetY()) {
         // 浮く状態へ
         _stateServer->GoToState("Save");
      }
   }
   // 位置の高さが既定の値より小さい場合
   else {
      // 既定の値高さを増やす
      _position.Add(0.0, UpSpeed, 0.0);
   }
}

void FallObject::Shoot() {
   // 移動量の設定
   auto move = _shootVec * ShootSpeed;
   // 位置の更新
   _position = _position + move;
}

void FallObject::SetCapsulePos() {
   // モデルの回転のマトリクスを作成
   auto [rx, ry, rz] = _rotation.GetVec3();
   auto world = AppFrame::Math::Matrix44();
   world.RotateZ(rz, false);
   world.RotateX(rx, false);
   world.RotateY(ry, false);
   if (_collisionName == "steel_frame_large_c") {

   }
   // カプセルの位置の設定
   _capsulePos1 = _position + Vector4(0.0, DrumCapsulePos1, 0.0) * world;
   _capsulePos2 = _position + Vector4(0.0, DrumCapsulePos2, 0.0) * world;
}

void FallObject::StateBase::Draw() {
   _owner._modelAnimeComponent->Draw();
#ifdef _DEBUG
   auto pos = AppFrame::Math::ToDX(_owner._position);
   auto radian = static_cast<float>(Range);
   DrawSphere3D(pos, radian, 10, GetColor(0, 0, 0), GetColor(0, 0, 0), FALSE);

   radian = static_cast<float>(DrumCapsuleRadius);

   DrawCapsule3D(AppFrame::Math::ToDX(_owner._capsulePos1), AppFrame::Math::ToDX(_owner._capsulePos2), radian, 20, GetColor(0, 255, 0), GetColor(0, 0, 0), FALSE);
#endif
}

void FallObject::StateIdle::Enter() {
   _owner._isFall = false;
}

void FallObject::StateIdle::Input(InputManager& input) {
   if (input.GetXJoypad().LeftTrigger() >= 20) {
      _owner.HitCheckFromPlayerPoint();
   }
}

void FallObject::StateIdle::Update() {
   // カプセルの位置の設定
   _owner.SetCapsulePos();
   _owner._collisionComponent->PlayerFromFallObjectModel(_owner._isFall);
   _owner._collisionComponent->PlayerFromObjectRange();
   _owner._collisionComponent->GatlingFromObjectModel();
   _owner._collisionComponent->FallObjectFromLaser();
   _owner.HitCheckFromLaser();
}

void FallObject::StateFall::Enter() {
   _owner._fallTimer = 0.0;
}

void FallObject::StateFall::Input(InputManager& input) {
   
}

void FallObject::StateFall::Update() {
   auto posY = (0.5 * Gravity * _owner._fallTimer * _owner._fallTimer);

   _owner._position.Add(0.0, -posY, 0.0);

   if (_owner._position.GetY() <= 0.0) {
      auto [oldPosX, oldPosY, oldPosZ] = _owner._position.GetVec3();
      _owner._position = AppFrame::Math::Vector4(oldPosX, 0.0, oldPosZ);
      auto efcFall = std::make_unique<Effect::EffectObjectFall>(_owner._gameMain, "ObjectFall");
      efcFall->position(_owner._position);
      efcFall->speed(2.0);
      _owner.GetEfcServer().Add(std::move(efcFall));
      // 残留オブジェクトでない場合死亡状態へ
      if (!_owner._residual) {
         _owner._stateServer->GoToState("Die");
      }
      // 残留オブジェクトの場合待機状態へ
      else {
         _owner._stateServer->GoToState("Idle");
      }
   }

   // カプセルの位置の設定
   _owner.SetCapsulePos();

   _owner._collisionComponent->PlayerFromFallObjectModel(_owner._isFall);

   ++_owner._fallTimer;
}

void FallObject::StateFall::Draw() {
   FallObject::StateBase::Draw();
   auto pointPosition = _owner._position;
   pointPosition.SetY(FallPointPosY);
   _owner.GetTexComponent().DrawBillBoard(pointPosition, FallPointScale,DefaultPointAngle, _owner._fallPointHandles, FallPointAnimeSpeed);
}

void FallObject::StateUp::Enter() {
   _owner._vecBeforeSave = _owner._position;
   _owner.residual(false);
}

void FallObject::StateUp::Update() {
   // 上昇させる
   _owner.Up();
   // カプセルの位置の設定
   _owner.SetCapsulePos();
   // 当たり判定管理クラスでプレイヤーがノックバックしているか確認
   _owner._collisionComponent->PlayerKnockBack();
   // プレイヤーがノックバックしているか確認
   _owner.CheckPlayerKnockBack();
}

void FallObject::StateUp::Exit() {

}

void FallObject::StateSave::Enter() {
   for (auto&& object : _owner.GetObjServer().runObjects()) {
      if (object->GetObjType() != Object::ObjectBase::ObjectType::Player) {
         return;
      }
      auto& player = dynamic_cast<Player::Player&>(*object);
      player.objectShoot(true);
      break;
   }
   _owner._efcUp = std::make_unique<Effect::EffectObjectUp>(_owner._gameMain, "ObjectUp");
   auto efcPos = _owner._position;
   efcPos.SetY(_owner._position.GetY() + UpEffectDiffY);
   _owner._efcUp->position(efcPos);
   _owner._efcUp->Init();
   Update();
}

void FallObject::StateSave::Input(InputManager& input) {
   if (input.GetXJoypad().RBClick()) {
      _owner._stateServer->GoToState("Shoot");
   }
}

void FallObject::StateSave::Update() {
    // 浮かせながら回転させる
   _owner.Save();
   // カプセルの位置の設定
   _owner.SetCapsulePos();
   // 当たり判定管理クラスでプレイヤーがノックバックしているか確認
   _owner._collisionComponent->PlayerKnockBack();
   // プレイヤーがノックバックしているか確認
   _owner.CheckPlayerKnockBack();
   auto efcPos = _owner._position;
   efcPos.SetY(_owner._position.GetY() + UpEffectDiffY);
   _owner._efcUp->position(efcPos);
   _owner._efcUp->Update();
}

void FallObject::StateSave::Exit() {
   _owner._efcUp->StopEffect();
}

void FallObject::StateShoot::Enter() {
   _owner._shootVec = _owner.GetObjServer().GetVecData("CamTarget") - _owner._position;
   _owner._shootVec.Normalized();
}

void FallObject::StateShoot::Input(InputManager& input) {

}

void FallObject::StateShoot::Update() {
   _owner.Shoot();
   // カプセルの位置の設定
   _owner.SetCapsulePos();
   _owner._collisionComponent->LargeEnemyFromObjectModel();
   _owner._collisionComponent->PoorEnemyGatlingFromObjectModel();
   _owner._collisionComponent->FallObjectFromLaser();
   _owner._collisionComponent->OutStage();
   _owner.HitCheckFromLargeEnemy();
   _owner.HitCheckFromLaser();
   _owner.HitCheckFromPoorEnemyGatling();
   _owner.OutStageCheck();
}

void FallObject::StateDie::Enter() {

}

void FallObject::StateDie::Input(InputManager& input) {

}

void FallObject::StateDie::Update() {
   _owner.SetDead();
}
