
/*****************************************************************//**
 * \file   FallObject.cpp
 * \brief  落下オブジェクトの処理を回すクラス
 * 
 * \author YUSUKEAHAMEDO
 * \date   January 2022
 *********************************************************************/
#include "FallObject.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "Player.h"
#include "ModelAnimeComponent.h"
#include "EffectObjectFall.h"
#include "ObjectServer.h"
#include "EffectServer.h"
#include "EffectObjectHit.h"
#include "ParamFallObject.h"

using namespace FragmentValkyria::Enemy;

namespace {
   constexpr auto DefaultPointAngle = 0.0;  //!< 落ちる場所のビルボードの回転角度
   constexpr auto UpEffectDiffY = 20.0;     //!< エフェクトの高さ
}

FallObject::FallObject() {
   _param = std::make_unique<Param::ParamFallObject>("fallobject");
   _collParam = std::make_unique<Param::ParamCollision>("collision");
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
   auto& resServer = AppFrame::Resource::ResourceServer::GetInstance();
   _fallPointHandles = resServer.GetTextures("FallPoint");
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
      auto efcHit = std::make_unique<Effect::EffectObjectHit>("ObjectHit");
      efcHit->position(_position);
      auto& efcServer = Game::Game::GetEfcServer();
      efcServer.Add(std::move(efcHit));
      auto& soundComponent = Game::Game::GetSoundComponent();
      soundComponent.Play("ObjectHit", _position);
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

void FallObject::HitCheckFromPoorEnemy() {
   // 当たり判定結果クラスの参照の取得
   auto report = _collisionComponent->report();
   // 当たり判定結果の確認
   if (report.id() == Collision::CollisionComponent::ReportId::HitFromPoorEnemy) {
      auto efcHit = std::make_unique<Effect::EffectObjectHit>("ObjectHit");
      efcHit->position(_position);
      auto& efcServer = Game::Game::GetEfcServer();
      efcServer.Add(std::move(efcHit));
      auto& soundComponent = Game::Game::GetSoundComponent();
      soundComponent.Play("ObjectHit",_position);
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
   /**
    * \brief double型の値を文字列で指定し、値管理クラスから取得する
    * \param paramName 値を指定する文字列
    * \return 文字列により指定された値
    */
   const auto _DoubleParam = [&](std::string paramName) {
      return _param->GetDoubleParam(paramName);
   };
   // 不規則な回転をさせるために使うサインの値を取得するために使う角度の更新
   _rotateAngle += 1.0;
   // 角度をラジアンにする
   auto radian = AppFrame::Math::Utility::DegreeToRadian(_rotateAngle);
   // サインの値を取得
   auto sinValue = std::sin(radian);
   // 既定の角度の-1倍から1倍の範囲で角度を決める
   auto xyz = _DoubleParam("rotate_angle") * sinValue;
   // 回転のベクトルを設定
   _rotation = Vector4(xyz, xyz, xyz);
   // オブジェクト一括管理クラスのオブジェクトの取得
   auto& runObjects = Game::Game::GetObjServer().runObjects();
   for (auto&& object : runObjects) {
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
      _position = _position + moveVec * _DoubleParam("up_speed");
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
   auto y = _DoubleParam("updown_range") * upDownSinValue;
   // 高さの更新
   _position.Add(0.0, y, 0.0);
}

void FallObject::Up() {
   /**
    * \brief double型の値を文字列で指定し、値管理クラスから取得する
    * \param paramName 値を指定する文字列
    * \return 文字列により指定された値
    */
   const auto _DoubleParam = [&](std::string paramName) {
      return _param->GetDoubleParam(paramName);
   };
   // オブジェクト一括管理クラスのオブジェクトの取得
   auto& runObjects = Game::Game::GetObjServer().runObjects();
   for (auto&& object : runObjects) {
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
   const auto UpSpeed = _DoubleParam("up_speed");
   // 位置の高さが既定の値より大きい場合
   if (_position.GetY() >= _DoubleParam("up_value")) {
      // プレイヤーの右手の座標のY成分に既定の値足す
      _plyRightHandVec.Add(0.0, _DoubleParam("right_hand_up_value"), 0.0);
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
   auto move = _shootVec * _param->GetDoubleParam("shoot_speed");
   // 位置の更新
   _position = _position + move;
}

void FallObject::SetCapsulePos() {
   /**
    * \brief double型の値を文字列で指定し、値管理クラスから取得する
    * \param paramName 値を指定する文字列
    * \return 文字列により指定された値
    */
   const auto _DoubleParam = [&](std::string paramName) {
      return _collParam->GetDoubleParam(paramName);
   };
   // モデルの回転のマトリクスを作成
   auto [rx, ry, rz] = _rotation.GetVec3();
   auto world = AppFrame::Math::Matrix44();
   world.RotateZ(rz, false);
   world.RotateX(rx, false);
   world.RotateY(ry, false);
   // カプセルの位置の設定
   _capsulePos1 = _position + Vector4(0.0, _DoubleParam("fallobject_drum_capsule_pos1"), 0.0) * world;
   _capsulePos2 = _position + Vector4(0.0, _DoubleParam("fallobject_drum_capsule_pos2"), 0.0) * world;
}

void FallObject::StateBase::Draw() {
   // モデルの描画処理を回す
   _owner._modelAnimeComponent->Draw();
#ifdef _DEBUG
   /**
    * \brief double型の値を文字列で指定し、値管理クラスから取得する
    * \param paramName 値を指定する文字列
    * \return 文字列により指定された値
    */
   const auto _DoubleParam = [&](std::string paramName) {
      return _owner._collParam->GetDoubleParam(paramName);
   };
   // 自作のベクトルクラスの位置をDxLibのVECTOR型に変換
   auto pos = AppFrame::Math::ToDX(_owner._position);
   // float型にキャスト
   auto radian = static_cast<float>(_DoubleParam("fallobject_range"));
   // 球の描画
   DrawSphere3D(pos, radian, 10, GetColor(0, 0, 0), GetColor(0, 0, 0), FALSE);
   // float型にキャスト
   radian = static_cast<float>(_DoubleParam("fallobject_drum_radius"));
   // カプセルの描画
   DrawCapsule3D(AppFrame::Math::ToDX(_owner._capsulePos1), AppFrame::Math::ToDX(_owner._capsulePos2), radian, 20, GetColor(0, 255, 0), GetColor(0, 0, 0), FALSE);
#endif
}

void FallObject::StateIdle::Enter() {
   // 落下状態でないと設定
   _owner._isFall = false;
}

void FallObject::StateIdle::Input(InputManager& input) {
   // 左トリガーが入力されていたら
   if (input.GetXJoypad().LeftTrigger() >= 20) {
      // プレイヤーが浮く範囲内にいるか確認
      _owner.HitCheckFromPlayerPoint();
   }
}

void FallObject::StateIdle::Update() {
   // カプセルの位置の設定
   _owner.SetCapsulePos();
   // 当たり判定処理を行うクラスでオブジェクトを持ち上げられる範囲にプレイヤーがいるか確認
   _owner._collisionComponent->PlayerFromObjectRange();
   // 当たり判定処理を行うクラスでプレイヤーが落下オブジェクトのモデルと当たっているか確認
   _owner._collisionComponent->PlayerFromFallObjectModel(_owner._isFall);
   // 当たり判定処理を行うクラスでオブジェクトのモデルにガトリングが当たっているか確認
   _owner._collisionComponent->GatlingFromObjectModel();
   // 当たり判定処理を行うクラスでレーザーが落下するオブジェクトに当たっているか確認
   _owner._collisionComponent->FallObjectFromLaser();
   // レーザーと当たっているか確認
   _owner.HitCheckFromLaser();
}

void FallObject::StateFall::Enter() {
   // 落下状態の進捗の初期値の設定
   _owner._fallTimer = 0.0;
}

void FallObject::StateFall::Update() {
   // Y座標の位置の変化量の取得
   auto posY = (0.5 * _owner._param->GetDoubleParam("gravity") * _owner._fallTimer * _owner._fallTimer);
   // 位置の更新
   _owner._position.Add(0.0, -posY, 0.0);
   // Y座標が地面より小さくなったら
   if (_owner._position.GetY() <= 0.0) {
      // 位置の分解
      auto [oldPosX, oldPosY, oldPosZ] = _owner._position.GetVec3();
      // 位置の高さを無くして設定
      _owner._position = AppFrame::Math::Vector4(oldPosX, 0.0, oldPosZ);
      auto efcFall = std::make_unique<Effect::EffectObjectFall>("ObjectFall");
      efcFall->position(_owner._position);
      efcFall->speed(2.0);
      auto& efcServer = Game::Game::GetEfcServer();
      efcServer.Add(std::move(efcFall));
      // 残留オブジェクトでない場合死亡状態へ
      if (!_owner._residual) {
         _owner._stateServer->GoToState("Die");
      }
      // 残留オブジェクトの場合待機状態へ
      else {
         _owner._stateServer->GoToState("Idle");
         auto& soundComponent = Game::Game::GetSoundComponent();
         soundComponent.Play("ObjectFall",_owner._position);
      }
   }
   // カプセルの位置の設定
   _owner.SetCapsulePos();
   // 当たり判定処理を行うクラスでプレイヤーが落下オブジェクトのモデルと当たっているか確認
   _owner._collisionComponent->PlayerFromFallObjectModel(_owner._isFall);
   // 落下状態の進捗の更新
   ++_owner._fallTimer;
}

void FallObject::StateFall::Draw() {
   /**
    * \brief double型の値を文字列で指定し、値管理クラスから取得する
    * \param paramName 値を指定する文字列
    * \return 文字列により指定された値
    */
   const auto _DoubleParam = [&](std::string paramName) {
      return _owner._param->GetDoubleParam(paramName);
   };
   // 状態の基底クラスの描画処理
   FallObject::StateBase::Draw();
   auto pointPosition = _owner._position;
   pointPosition.SetY(_DoubleParam("fallpoint_pos_y"));
   auto& texComponent = Game::Game::GetTexComponent();
   texComponent.DrawBillBoard(pointPosition, _DoubleParam("fallpoint_scale"),
      DefaultPointAngle, _owner._fallPointHandles, _owner._param->GetIntParam("fallpoint_animespeed"));
}

void FallObject::StateUp::Enter() {
   // 残留オブジェクトでないと設定
   _owner.residual(false);
   _owner._efcUp = std::make_unique<Effect::EffectObjectUp>("ObjectUp");
   auto efcPos = _owner._position;
   efcPos.SetY(_owner._position.GetY() + UpEffectDiffY);
   _owner._efcUp->position(efcPos);
   _owner._efcUp->Init();
   _owner._efcBorn = true;
   // 更新処理
   Update();
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
   auto efcPos = _owner._position;
   efcPos.SetY(_owner._position.GetY() + UpEffectDiffY);
   _owner._efcUp->position(efcPos);
   _owner._efcUp->Update();
}

void FallObject::StateSave::Enter() {
   // オブジェクト一括管理クラスの各オブジェクトの取得
   auto& runObjects = Game::Game::GetObjServer().runObjects();
   for (auto&& object : runObjects) {
      // プレイヤーでなければ処理をスキップしてfor文を回す
      if (object->GetObjType() != Object::ObjectBase::ObjectType::Player) {
         continue;
      }
      // プレイヤーの参照型にキャスト
      auto& player = dynamic_cast<Player::Player&>(*object);
      // プレイヤーがオブジェクトを打てると設定
      player.objectShoot(true);
      break;
   }
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

void FallObject::StateShoot::Enter() {
   // カメラの注視点へのベクトルを設定
   auto& objServer = Game::Game::GetObjServer();
   _owner._shootVec = objServer.GetVecData("CamTarget") - _owner._position;
   // 単位化する
   _owner._shootVec.Normalized();
}

void FallObject::StateShoot::Update() {
   // カメラの注視点へ進む処理
   _owner.Shoot();
   // カプセルの位置の設定
   _owner.SetCapsulePos();
   // 当たり判定処理を行うクラスでラージエネミーのモデルに落下オブジェクトが当たっているか確認
   _owner._collisionComponent->LargeEnemyFromObjectModel();
   // 当たり判定処理を行うクラスで雑魚敵のモデルに落下オブジェクトが当たっているか確認
   _owner._collisionComponent->PoorEnemyGatlingFromObjectModel();
   // 当たり判定処理を行うクラスでレーザーが落下オブジェクトに当たっているか確認
   _owner._collisionComponent->FallObjectFromLaser();
   // 当たり判定処理を行うクラスでステージ外にいるか確認
   _owner._collisionComponent->OutStage();
   // ラージエネミーのモデルと当たっているか確認
   _owner.HitCheckFromLargeEnemy();
   // レーザーと当たっているか確認
   _owner.HitCheckFromLaser();
   // 雑魚敵と当たっているか確認
   _owner.HitCheckFromPoorEnemy();
   // ステージ外にいるか確認
   _owner.OutStageCheck();
   auto efcPos = _owner._position;
   efcPos.SetY(_owner._position.GetY() + UpEffectDiffY);
   _owner._efcUp->position(efcPos);
   _owner._efcUp->Update();
}

void FallObject::StateDie::Update() {
   if (_owner._efcBorn) {
      _owner._efcUp->StopEffect();
   }
   // 死亡状態に設定
   _owner.SetDead();
}
