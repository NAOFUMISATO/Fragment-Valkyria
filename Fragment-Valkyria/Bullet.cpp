
/*****************************************************************//**
 * \file   Bullet.cpp
 * \brief  プレイヤーの遠隔弱攻撃の弾の処理を回すクラス
 * 
 * \author YUSUKEAHAMEDO
 * \date   January 2022
 *********************************************************************/
#include "Bullet.h"
#include "CollisionComponent.h"
#include "ObjectServer.h"
#include "ParamBullet.h"

using namespace FragmentValkyria::Player;

Bullet::Bullet(Game::GameMain& gameMain) : ObjectBase{ gameMain } {
   _param = std::make_unique<Param::ParamBullet>(_gameMain,"weakbullet");
   _collParam = std::make_unique < Param::ParamCollision>(_gameMain, "collision");
}

void Bullet::Init() {
   // カメラの注視点へのベクトルを取得
   _moved = GetObjServer().GetVecData("CamTarget") - _position;
   // 単位化する
   _moved.Normalized();
}

void Bullet::Update() {
   // 状態の更新
   _stateServer->Update();
   // ワールド行列の更新
   ComputeWorldTransform();
}

void Bullet::Draw() {
   // 各状態の描画処理を回す
   _stateServer->Draw();
}

void Bullet::Move() {
   // 位置の更新
   _position = _position + _moved * _param->GetDoubleParam("speed");
}

void Bullet::HitCheckFromLargeEnemy() {
   // 当たり判定結果クラスの参照の取得
   auto report = _collisionComponent->report();
   // 当たり判定結果がラージエネミーと当たっていたら死亡状態へ
   if (report.id() == Collision::CollisionComponent::ReportId::HitFromLargeEnemy) {
      // ここで死亡状態に設定することで弾が複数当たってしまうのを直せた
      SetDead();
      _efcBullet->StopEffect();
   }
}

void Bullet::HitCheckFromPoorEnemyGatling() {
   // 当たり判定結果クラスの参照の取得
   auto report = _collisionComponent->report();
   // 当たり判定結果がガトリング攻撃をしてくる雑魚敵と当たっていたら死亡状態へ
   if (report.id() == Collision::CollisionComponent::ReportId::HitFromPoorEnemy) {
      // ここで死亡状態に設定することで弾が複数当たってしまうのを直せた
      SetDead();
      _efcBullet->StopEffect();
   }
}

void Bullet::OutCheckFromStage() {
   // 当たり判定結果クラスの参照の取得
   auto report = _collisionComponent->report();
   // 当たり判定結果がステージ外の場合死亡状態へ
   if (report.id() == Collision::CollisionComponent::ReportId::OutStage) {
      // ここで死亡状態に設定することで弾が複数当たってしまうのを直せた
      SetDead();
      _efcBullet->StopEffect();
   }
}

void Bullet::StateBase::Draw() {
#ifdef _DEBUG
   // 位置を自作のVector4クラスからDxLib::VECTOR構造体へ変換
   auto pos = AppFrame::Math::ToDX(_owner._position);
   // 半径をfloat型にキャスト
   auto radius = static_cast<float>(_owner._collParam->GetDoubleParam("bullet_radius"));
   // DxLibによる球の描画
   DrawSphere3D(pos, radius, 20, GetColor(0, 0, 255), GetColor(0, 0, 0), FALSE);
#endif
}

void Bullet::StateShoot::Enter() {
   _owner._efcBullet = std::make_unique<Effect::EffectWeakBullet>(_owner._gameMain,"WeakBullet");
   _owner._efcBullet->Init();
   Update();
}

void Bullet::StateShoot::Update() {
   // 移動処理
   _owner.Move();
   // 当たり判定処理を行うクラスでラージエネミーと当たっているか確認
   _owner._collisionComponent->LargeEnemyFromBullet();
   // 当たり判定処理を行うクラスでステージ外にいるか確認
   _owner._collisionComponent->OutStage();
   // ラージエネミーと当たっているか確認
   _owner.HitCheckFromLargeEnemy();
   // ガトリング攻撃をしてくる雑魚敵と当たっているか確認
   _owner.HitCheckFromPoorEnemyGatling();
   // ステージ外にいるか確認
   _owner.OutCheckFromStage();
   _owner._efcBullet->position(_owner._position);
   _owner._efcBullet->Update();
}



