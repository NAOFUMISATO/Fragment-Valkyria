
/*****************************************************************//**
 * \file   CollisionServer.cpp
 * \brief  当たり判定処理を行うクラス
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "CollisionComponent.h"
#include "ModelAnimeComponent.h"
#include "Gatling.h"
#include "GameMain.h"
#include "Player.h"
#include "Stage.h"
#include "StageModelComponent.h"
#include "LoadStageFromJson.h"
#include "Laser.h"
#include "ModeBoss.h"
#include "ModePoor.h"
#include "ObjectBase.h"
#include "ObjectServer.h"
#include "Player.h"
#ifdef _DEBUG
#include <stdexcept>
#include <windows.h>
#endif

using namespace FragmentValkyria::Collision;

namespace {
   auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("collision",
      { "fallobject_range", "ply_radius", "ply_capsule_pos1",
       "ply_capsule_pos2", "gatling_radius", "fallobject_capsule_pos1",
      "fallobject_capsule_pos2", "fallobject_radius", "laser_radius"});

   const double FallObjectRange = paramMap["fallobject_range"];                      //!< 落下するオブジェクトを持ち上げられる範囲の球の半径
   const double PlayerRadius = paramMap["ply_radius"];                               //!< プレイヤーのカプセルの半径
   const double PlayerCapsulePos1 = paramMap["ply_capsule_pos1"];                    //!< プレイヤーのカプセルを形成する2点中の一点の座標までのプレイヤーの位置からの距離
   const double PlayerCapsulePos2 = paramMap["ply_capsule_pos2"];                    //!< プレイヤーのカプセルを形成する2点中の一点の座標までのプレイヤーの位置からの距離
   const double GatlingRadius = paramMap["gatling_radius"];                          //!< ガトリングの半径
   const double FallObjectCapsulePos1 = paramMap["fallobject_capsule_pos1"];         //!< フォールオブジェクトのカプセルを形成する2点中の一点の座標までのフォールオブジェクトの位置からの距離
   const double FallObjectCapsulePos2 = paramMap["fallobject_capsule_pos2"];         //!< フォールオブジェクトのカプセルを形成する2点中の一点の座標までのフォールオブジェクトの位置からの距離
   const double FallObjectRadius = paramMap["fallobject_radius"];                    //!< フォールオブジェクトのカプセルの半径
   const double LaserRadius = paramMap["laser_radius"];                              //!< レーザーのカプセルの半径
}

CollisionComponent::CollisionComponent(Object::ObjectBase& owner) : _owner{ owner } {
   //当たり判定の結果を管理するクラスのポインタ作成
   _report = std::make_unique<Report>();
}

void CollisionComponent::PlayerFromObjectRange() {
   // オブジェクトサーバーの各オブジェクトを取得
   for (auto&& object : _owner.GetObjServer().runObjects()) {
      // プレイヤーじゃなければ何もしない
      if (object->GetObjType() != Object::ObjectBase::ObjectType::Player) {
         continue;
      }
      // 他のオブジェクトに当たっていた場合は終了する
      if (object->collisionComponent().report().id() == ReportId::HitFromIdleFallObject) {
         break;
      }
      if (object->collisionComponent().report().id() == ReportId::HitFromFallObject) {
         break;
      }
      if (object->collisionComponent().report().id() == ReportId::HitFromGatling) {
         break;
      }
      if (object->collisionComponent().report().id() == ReportId::HitFromLargeEnemy) {
         break;
      }
      if (object->collisionComponent().report().id() == ReportId::HitFromPoorEnemy) {
         break;
      }
      // プレイヤーの参照型にキャストする
      auto& player = dynamic_cast<Player::Player&>(*object);
      // オブジェクトサーバーの各オブジェクトを取得
      for (auto&& object : _owner.GetObjServer().runObjects()) {
         // 当たり判定の結果がプレイヤーと当たっているか確認
         if (object->collisionComponent().report().id() == ReportId::HitFromPlayer) {
            // プレイヤーと当たっているオブジェクトがあった場合
            // プレイヤーがオブジェクトを持ち上げられない場合落下するオブジェクトの当たり判定の結果に当たっていないと設定
            if (!player.isLift()) {
               object->collisionComponent().report().id(ReportId::None);
               // 処理をスキップして戻る
               return;
            }
            // 落下するオブジェクトの位置を取得
            auto objectPos = object->position();
            // 落下するオブジェクトを持ち上げられる範囲の球の半径
            auto objectRadian = FallObjectRange;
            // 自前の球を定義
            AppFrame::Math::Sphere objectRange = std::make_tuple(objectPos, objectRadian);
            // プレイヤーの位置取得
            auto plyPoint = player.position();
            // 球と点の当たり判定をとる
            if (AppFrame::Math::Utility::CollisionSpherePoint(plyPoint, objectRange)) {
               // 当たっていたらプレイヤーの当たり判定の結果にオブジェクトを持ち上げられる範囲にいると設定
               player.collisionComponent().report().id(ReportId::HitFromObjectRange);
               // 落下するオブジェクトの当たり判定の結果にプレイヤーに当たっていると設定
               object->collisionComponent().report().id(ReportId::HitFromPlayer);
            }
            else {
               // 当たっていなかったらプレイヤーの当たり判定の結果に当たっていないと設定
               player.collisionComponent().report().id(ReportId::None);
               // 落下するオブジェクトの当たり判定の結果に当たっていないと設定
               object->collisionComponent().report().id(ReportId::None);
            }
            // 処理をスキップして戻る
            return;
         }
      }
      // 落下するオブジェクトの位置を取得
      auto objectPos = _owner.position();
      // 落下するオブジェクトを持ち上げられる範囲の球の半径
      auto objectRadian = FallObjectRange;
      // 自前の球を定義
      AppFrame::Math::Sphere objectRange = std::make_tuple(objectPos, objectRadian);
      // プレイヤーの位置取得
      auto plyPoint = player.position();
      // プレイヤーがオブジェクトを持ち上げられない場合落下するオブジェクトの当たり判定の結果に当たっていないと設定
      if (!player.isLift()) {
         _owner.collisionComponent().report().id(ReportId::None);
         // 処理をスキップして戻る
         return;
      }
      // 球と点の当たり判定をとる
      if (AppFrame::Math::Utility::CollisionSpherePoint(plyPoint, objectRange)) {
         // 当たっていたらプレイヤーの当たり判定の結果にオブジェクトを持ち上げられる範囲にいると設定
         player.collisionComponent().report().id(ReportId::HitFromObjectRange);
         // 落下するオブジェクトの当たり判定の結果にプレイヤーに当たっていると設定
         _owner.collisionComponent().report().id(ReportId::HitFromPlayer);
      }
      else {
         // 当たっていなかったらプレイヤーの当たり判定の結果に当たっていないと設定
         player.collisionComponent().report().id(ReportId::None);
         // 落下するオブジェクトの当たり判定の結果に当たっていないと設定
         _owner.collisionComponent().report().id(ReportId::None);
      }
   }
}

void CollisionComponent::PlayerFromFallObjectModel(bool fall) {
   // 落下するオブジェクトのモデルのハンドル取得
   auto objectModel = _owner.modelAnimeComponent().modelHandle();
   // モデルのコリジョンのフレーム番号取得
   auto collision = MV1SearchFrame(objectModel, "drum_green_c");
   // オブジェクトサーバーの各オブジェクトを取得
   for (auto&& object : _owner.GetObjServer().runObjects()) {
      // プレイヤーじゃなかったら何もしない
      if (object->GetObjType() != Object::ObjectBase::ObjectType::Player) {
         continue;
      }
      // 当たり判定の結果が待機状態の落下するオブジェクトと当たっている場合何もしない
      if (object->collisionComponent().report().id() == ReportId::HitFromIdleFallObject) {
         continue;
      }
      // プレイヤーの参照型の取得
      auto& player = dynamic_cast<Player::Player&>(*object);
      // 無敵時間の取得
      auto invincibleCnt = player.invincibleCnt();
      // プレイヤー側のカプセルを設定
      auto plyPos = object->position();
      // カプセルの一つ目の位置
      auto pos1 = plyPos + AppFrame::Math::Vector4(0.0, PlayerCapsulePos1, 0.0);
      // カプセルの二つ目の位置
      plyPos.Add(0.0, PlayerCapsulePos2, 0.0);
      // カプセルの半径
      auto radian = static_cast<float>(PlayerRadius);
      // カプセルとモデルの当たり判定をとる
      auto result = MV1CollCheck_Capsule(objectModel, collision, AppFrame::Math::ToDX(plyPos), AppFrame::Math::ToDX(plyPos), radian);
      // 当たり判定の結果が当たっているか確認
      if (result.HitNum > 0) {
         // 当たっていた場合落下か確認
         if (fall) {
            // 落下中だった場合無敵時間だったら終了
            if (invincibleCnt > 0) {
               return;
            }
            // 無敵時間じゃない場合当たった位置に落下するオブジェクトの位置を設定
            object->collisionComponent().hitPos(_owner.position());
            // プレイヤーの当たり判定の結果に落下するオブジェクトのモデルと当たったと設定
            object->collisionComponent().report().id(ReportId::HitFromFallObject);
            // ダメージを20.0に設定
            object->collisionComponent().damage(20.0);
         }
         else {
            // 落下中じゃない場合当たった位置に当たったポリゴンの法線を設定
            object->collisionComponent().hitPos(AppFrame::Math::ToMath(result.Dim[0].Normal));
            // プレイヤーの当たり判定の結果に待機状態の落下するオブジェクトと当たったと設定
            object->collisionComponent().report().id(ReportId::HitFromIdleFallObject);
         }
      }
   }
}

void CollisionComponent::GatlingFromObjectModel() {
   // 落下するオブジェクトのモデルのハンドルの取得
   auto objectModel = _owner.modelAnimeComponent().modelHandle();
   // モデルのコリジョンのフレーム番号取得
   auto collision = MV1SearchFrame(objectModel, "drum_green_c");
   // オブジェクトサーバーの各オブジェクトを取得
   for (auto&& object : _owner.GetObjServer().runObjects()) {
      // ガトリングじゃなかったら何もしない
      if (object->GetObjType() != Object::ObjectBase::ObjectType::Gatling) {
         continue;
      }
      // ガトリング側の球を設定
      auto gatlingPos = object->position();
      auto radian = static_cast<float>(GatlingRadius);
      // 球とモデルの当たり判定の結果を取得
      auto result = MV1CollCheck_Sphere(objectModel, collision, AppFrame::Math::ToDX(gatlingPos), radian);
      // 当たり判定の結果が当たっているか確認
      if (result.HitNum > 0) {
         // ガトリングの当たり判定結果に待機状態の落下するオブジェクトと当たったと設定
         object->collisionComponent().report().id(ReportId::HitFromIdleFallObject);
      }
   }
}

void CollisionComponent::GatlingFromPlayer() {
   // プレイヤーの参照型にキャスト
   auto& player = dynamic_cast<Player::Player&>(_owner);
   // 無敵時間の取得
   auto invincibleCnt = player.invincibleCnt();
   // 無敵時間中だったら何もしない
   if (invincibleCnt > 0) {
      return;
   }
   // プレイヤー側のカプセルを設定
   auto playerPos = _owner.position();
   // カプセルの位一つ目の位置
   auto capsulePos1 = playerPos + Vector4(0.0, 30.0, 0.0);
   // カプセルの二つ目の位置
   auto capsulePos2 = playerPos + Vector4(0.0, 60.0, 0.0);
   // カプセルの半径
   auto casuleRadian = PlayerRadius;
   // 自前のカプセルを定義
   AppFrame::Math::Capsule playerCapsule = std::make_tuple(capsulePos1, capsulePos2, casuleRadian);
   // オブジェクトサーバーの各オブジェクトを取得
   for (auto&& object : _owner.GetObjServer().runObjects()) {
      // ガトリングじゃなかったら何もしない
      if (object->GetObjType() != Object::ObjectBase::ObjectType::Gatling) {
         continue;
      }
      // ガトリングの位置の取得
      auto gatling = object->position();
      // ガトリングの半径の設定
      auto gatlingRadian = GatlingRadius;
      // 自前の球を定義
      AppFrame::Math::Sphere gatlingSphere = std::make_tuple(gatling, gatlingRadian);
      // カプセルと球で当たり判定をとる
      if (AppFrame::Math::Utility::CollisionCapsuleSphere(playerCapsule, gatlingSphere)) {
         // 当たっていたらガトリング側の当たり判定の結果をプレイヤーと当たったと設定
         object->collisionComponent().report().id(ReportId::HitFromPlayer);
         // プレイヤー側の当たり判定の結果をガトリングと当たったと設定
         _owner.collisionComponent().report().id(ReportId::HitFromGatling);
         // ダメージの設定
         _owner.collisionComponent().damage(20.0);
         // プレイヤーの当たった位置にガトリングの位置を設定
         _owner.collisionComponent().hitPos(object->position());
      }
   }
}

void CollisionComponent::LargeEnemyFromObjectModel() {
   // 落下するオブジェクトのカプセルを作成
   auto fallObjectPos = _owner.position();
   // カプセルの一つ目の位置
   auto capsulePos1 = Vector4(0.0, FallObjectCapsulePos1, 0.0) + fallObjectPos;
   // カプセルの二つ目の位置
   auto capsulePos2 = Vector4(0.0, FallObjectCapsulePos2, 0.0) + fallObjectPos;
   // カプセルの半径
   auto capsuleRadian = static_cast<float>(FallObjectRadius);
   // オブジェクトサーバーの各オブジェクトを取得
   for (auto&& object : _owner.GetObjServer().runObjects()) {
      // ラージエネミーじゃない場合何もしない
      if (object->GetObjType() != Object::ObjectBase::ObjectType::LargeEnemy) {
         continue;
      }
      // ラージエネミーのモデルのハンドルの取得
      auto largeEnemyModel = object->modelAnimeComponent().modelHandle();
      // ラージエネミーのモデルの弱点のコリジョンフレーム番号の取得
      auto collision = object->modelAnimeComponent().FindFrame("pasted__weak_collision");
      // モデルの弱点とカプセルの当たり判定を取る
      auto result = MV1CollCheck_Capsule(largeEnemyModel, collision, AppFrame::Math::ToDX(capsulePos1), AppFrame::Math::ToDX(capsulePos2), capsuleRadian);
      // 当たり判定の結果が当たっているか確認
      if (result.HitNum > 0) {
         // 当たっていたらラージエネミーの当たり判定結果を落下するオブジェクトと当たっていると設定
         object->collisionComponent().report().id(ReportId::HitFromFallObject);
         // ダメージの設定
         object->collisionComponent().damage(1000.0);
         // 落下するオブジェクトの当たり判定結果をラージエネミーと当たったと設定
         _owner.collisionComponent().report().id(ReportId::HitFromLargeEnemy);
         // for文を抜ける
         break;
      }
      // モデルの弱点と当たっていない場合
      else {
         // ラージエネミーのモデルの顔のコリジョンフレーム番号の取得
         collision = object->modelAnimeComponent().FindFrame("pasted__face_collision");
         // モデルの顔とカプセルの当たり判定を取る
         result = MV1CollCheck_Capsule(largeEnemyModel, collision, AppFrame::Math::ToDX(capsulePos1), AppFrame::Math::ToDX(capsulePos2), capsuleRadian);
         // 当たり判定の結果が当たっているか確認
         if (result.HitNum > 0) {
            // 当たっていたらラージエネミーの当たり判定結果を落下するオブジェクトと当たっていると設定
            object->collisionComponent().report().id(ReportId::HitFromFallObject);
            // ダメージの設定
            object->collisionComponent().damage(100.0);
            // 落下するオブジェクトの当たり判定結果をラージエネミーと当たったと設定
            _owner.collisionComponent().report().id(ReportId::HitFromLargeEnemy);
            // for文を抜ける
            break;
         }
         // モデルの顔と当たっていない場合
         else {
            // ラージエネミーのモデルの胴体のコリジョンフレーム番号の取得
            collision = object->modelAnimeComponent().FindFrame("pasted__body_collision");
            // モデルの胴体とカプセルの当たり判定を取る
            result = MV1CollCheck_Capsule(largeEnemyModel, collision, AppFrame::Math::ToDX(capsulePos1), AppFrame::Math::ToDX(capsulePos2), capsuleRadian);
            // 当たり判定の結果が当たっているか確認
            if (result.HitNum > 0) {
               // 当たっていたらラージエネミーの当たり判定結果を落下するオブジェクトと当たっていると設定
               object->collisionComponent().report().id(ReportId::HitFromFallObject);
               // ダメージの設定
               object->collisionComponent().damage(0.0);
               // 落下するオブジェクトの当たり判定結果をラージエネミーと当たったと設定
               _owner.collisionComponent().report().id(ReportId::HitFromLargeEnemy);
               // for文を抜ける
               break;
            }
         }
      }
    }
}

void CollisionComponent::LargeEnemyFromBullet() {
   // 遠隔弱攻撃の弾の位置を取得
   auto bulletPos = _owner.position();
   // 遠隔弱攻撃の弾の半径を設定
   auto bulletRadius = static_cast<float>(20.0);
   // オブジェクトサーバーの各オブジェクトを取得
   for (auto&& object : _owner.GetObjServer().runObjects()){
      // ラージエネミーじゃなかったら何もしない
      if (object->GetObjType() != Object::ObjectBase::ObjectType::LargeEnemy) {
         continue;
      }
      // ラージエネミーのモデルのハンドルを取得
      auto largeEnemyModel = object->modelAnimeComponent().modelHandle();
      // ラージエネミーのモデルの弱点のコリジョンフレーム番号の取得
      auto collision = object->modelAnimeComponent().FindFrame("pasted__weak_collision");
      // モデルの弱点と球の当たり判定を取る
      auto result = MV1CollCheck_Sphere(largeEnemyModel, collision, AppFrame::Math::ToDX(bulletPos), bulletRadius);
      // 当たり判定の結果から当たっているか確認
      if (result.HitNum > 0) {
         // 当たっていたらラージエネミーの当たり判定結果を遠隔弱攻撃の弾と当たったと設定
         object->collisionComponent().report().id(ReportId::HitFromBullet);
         // ダメージ量の設定
         object->collisionComponent().damage(100.0);
         // 遠隔弱攻撃の弾の当たり判定結果をラージエネミーと当たったと設定
         _owner.collisionComponent().report().id(ReportId::HitFromLargeEnemy);
         // for文を抜ける
         break;
      }
      // モデルの弱点と当たっていない場合
      else {
         // ラージエネミーのモデルの顔のコリジョンフレーム番号の取得
         collision = object->modelAnimeComponent().FindFrame("pasted__face_collision");
         // モデルの顔と球の当たり判定を取る
         result = MV1CollCheck_Sphere(largeEnemyModel, collision, AppFrame::Math::ToDX(bulletPos), bulletRadius);
         // 当たり判定の結果から当たっているか確認
         if (result.HitNum > 0) {
            // 当たっていたらラージエネミーの当たり判定結果を遠隔弱攻撃の弾と当たったと設定
            object->collisionComponent().report().id(ReportId::HitFromBullet);
            // ダメージ量の設定
            object->collisionComponent().damage(50.0);
            // 遠隔弱攻撃の弾の当たり判定結果をラージエネミーと当たったと設定
            _owner.collisionComponent().report().id(ReportId::HitFromLargeEnemy);
            // for文を抜ける
            break;
         }
         // モデルの顔に当たっていなかった場合
         else {
            // ラージエネミーのモデルの胴体のコリジョンフレーム番号の取得
            collision = object->modelAnimeComponent().FindFrame("pasted__body_collision");
            // モデルの胴体と球の当たり判定を取る
            result = MV1CollCheck_Sphere(largeEnemyModel, collision, AppFrame::Math::ToDX(bulletPos), bulletRadius);
            // 当たり判定の結果から当たっているか確認
            if (result.HitNum > 0) {
               // 当たっていたらラージエネミーの当たり判定結果を遠隔弱攻撃の弾と当たったと設定
               object->collisionComponent().report().id(ReportId::HitFromBullet);
               // ダメージ量の設定
               object->collisionComponent().damage(0.0);
               // 遠隔弱攻撃の弾の当たり判定結果をラージエネミーと当たったと設定
               _owner.collisionComponent().report().id(ReportId::HitFromLargeEnemy);
               // for文を抜ける
               break;
            }
         }
      }
   }
}

void CollisionComponent::FallObjectFromLaser() {
   //落下するオブジェクトのカプセルの作成
   auto fallObjectPos = _owner.position();
   //カプセルの一つ目の位置
   auto fallObjectCapsulePos1 = Vector4(0.0, FallObjectCapsulePos1, 0.0) + fallObjectPos;
   //カプセルの二つ目の位置
   auto fallObjectCapsulePos2 = Vector4(0.0, FallObjectCapsulePos2, 0.0) + fallObjectPos;
   //カプセルの半径
   auto fallObjectRadius = FallObjectRadius;
   //自前のカプセルを定義
   auto fallObjectCapsule = std::make_tuple(fallObjectCapsulePos1, fallObjectCapsulePos2, fallObjectRadius);
   //オブジェクトサーバーの各オブジェクトを取得
   for (auto&& object : _owner.GetObjServer().runObjects()) {
      //レーザーじゃなかったら何もしない
      if (object->GetObjType() != Object::ObjectBase::ObjectType::Laser) {
         continue;
      }
      //レーザーの参照型にキャスト
      auto& laser = dynamic_cast<Enemy::Laser&>(*object);
      //レーザーのカプセルを作成
      //カプセルの一つ目の位置
      auto laserCapsulePos1 = laser.position();
      //カプセルの二つ目の位置
      auto laserCapsulePos2 = laser.end();
      //カプセルの半径
      auto laserRadius = LaserRadius;
      //自前のカプセルを定義
      auto laserCapsule = std::make_tuple(laserCapsulePos1, laserCapsulePos2, laserRadius);
      //カプセルとカプセルの当たり判定を取る
      if (AppFrame::Math::Utility::CollisionCapsuleCapsule(fallObjectCapsule, laserCapsule)) {
         //当たっていたら落下するオブジェクトの当たり判定結果をレーザーと当たったと設定
         _owner.collisionComponent().report().id(ReportId::HitFromLaser);
      }
      break;
   }
}

void CollisionComponent::PlayerFromLaser() {
   //プレイヤーの参照型の取得
   auto& player = dynamic_cast<Player::Player&>(_owner);
   //無敵時間の取得
   auto invincibleCnt = player.invincibleCnt();
   //無敵時間中だった場合何もしない
   if (invincibleCnt > 0) {
      return;
   }
   //プレイヤーのカプセルの作成
   auto playerPos = _owner.position();
   //カプセルの一つ目の位置
   auto plyCapsulePos1 = Vector4(0.0, PlayerCapsulePos1, 0.0) + playerPos;
   //カプセルの二つ目の位置
   auto plyCapsulePos2 = Vector4(0.0, PlayerCapsulePos2, 0.0) + playerPos;
   //カプセルの半径
   auto playerCapsuleRadius = PlayerRadius;
   //自前のカプセルを定義
   AppFrame::Math::Capsule playerCapsule = std::make_tuple(plyCapsulePos1, plyCapsulePos2, playerCapsuleRadius);
   //オブジェクトサーバーの各オブジェクトを取得
   for (auto&& object : _owner.GetObjServer().runObjects()) {
      //レーザーじゃなかったら何もしない
      if (object->GetObjType() != Object::ObjectBase::ObjectType::Laser) {
         continue;
      }
      //レーザーの参照型にキャスト
      auto& laser = dynamic_cast<Enemy::Laser&>(*object);
      //レーザーのカプセルを作成
      //カプセルの一つ目の位置
      auto laserCapsulePos1 = laser.position();
      //カプセルの二つ目の位置
      auto laserCapsulePos2 = laser.end();
      //カプセルの半径
      auto laserRadius = LaserRadius;
      //自前のカプセルを定義
      auto laserCapsule = std::make_tuple(laserCapsulePos1, laserCapsulePos2, laserRadius);
      //カプセルとカプセルの当たり判定を取る
      if (AppFrame::Math::Utility::CollisionCapsuleCapsule(playerCapsule, laserCapsule)) {
         //当たっていた場合プレイヤーの当たり判定の結果をレーザーと当たったと設定
         _owner.collisionComponent().report().id(ReportId::HitFromLaser);
         //ダメージの設定
         _owner.collisionComponent().damage(20.0);
         //当たった位置にレーザーの位置を設定
         _owner.collisionComponent().hitPos(laser.position());
      }
      break;
   }
}

void CollisionComponent::LargeEnemyFromPlayer() {
   //ラージエネミーのモデルのハンドルの取得
   auto largeEnemyModel = _owner.modelAnimeComponent().modelHandle();
   //ラージエネミーのモデルのコリジョンフレーム番号の取得
   auto collision = _owner.modelAnimeComponent().FindFrame("S301_typeCO");
   //オブジェクトサーバーの各オブジェクトを取得
   for (auto&& object : _owner.GetObjServer().runObjects()) {
      //プレイヤーじゃなかったら何もしない
      if (object->GetObjType() != Object::ObjectBase::ObjectType::Player) {
         continue;
      }
      //プレイヤー型の参照の取得
      auto& player = dynamic_cast<Player::Player&>(*object);
      //無敵時間の取得
      auto invincibleCnt = player.invincibleCnt();
      //無敵時間中だったら何もしない
      if (invincibleCnt > 0) {
         return;
      }
      //プレイヤーのカプセルの作成
      auto playerPos = player.position();
      //カプセルの一つ目の位置
      auto plyCapsulePos1 = Vector4(0.0, PlayerCapsulePos1, 0.0) + playerPos;
      //カプセルの二つ目の位置
      auto plyCapsulePos2 = Vector4(0.0, PlayerCapsulePos2, 0.0) + playerPos;
      //カプセルの半径
      auto playerCapsuleRadius = static_cast<float>(PlayerRadius);
      //モデルとカプセルの当たり判定を取る
      auto result = MV1CollCheck_Capsule(largeEnemyModel, collision, AppFrame::Math::ToDX(plyCapsulePos1), AppFrame::Math::ToDX(plyCapsulePos2), playerCapsuleRadius);
      //当たり判定の結果を確認
      if (result.HitNum > 0) {
         //当たっていたらプレイヤーの当たり判定結果をボスと当たったと設定
         player.collisionComponent().report().id(ReportId::HitFromLargeEnemy);
         //当たった位置にラージエネミーの位置を設定
         player.collisionComponent().hitPos(_owner.position());
         //ダメージの設定
         player.collisionComponent().damage(20.0);
      }
   }
}

void CollisionComponent::PoorEnemyFromPlayer() {
   //ラージエネミーのモデルのハンドルの取得
   auto poorEnemyModel = _owner.modelAnimeComponent().modelHandle();
   //ラージエネミーのモデルのコリジョンフレーム番号の取得
   auto collision = _owner.modelAnimeComponent().FindFrame("mob");
   //オブジェクトサーバーの各オブジェクトを取得
   for (auto&& object : _owner.GetObjServer().runObjects()) {
      //プレイヤーじゃなかったら何もしない
      if (object->GetObjType() != Object::ObjectBase::ObjectType::Player) {
         continue;
      }
      //プレイヤー型の参照の取得
      auto& player = dynamic_cast<Player::Player&>(*object);
      //無敵時間の取得
      auto invincibleCnt = player.invincibleCnt();
      //無敵時間中だったら何もしない
      if (invincibleCnt > 0) {
         return;
      }
      //プレイヤーのカプセルの作成
      auto playerPos = player.position();
      //カプセルの一つ目の位置
      auto plyCapsulePos1 = Vector4(0.0, PlayerCapsulePos1, 0.0) + playerPos;
      //カプセルの二つ目の位置
      auto plyCapsulePos2 = Vector4(0.0, PlayerCapsulePos2, 0.0) + playerPos;
      //カプセルの半径
      auto playerCapsuleRadius = static_cast<float>(PlayerRadius);
      //モデルとカプセルの当たり判定を取る
      auto result = MV1CollCheck_Capsule(poorEnemyModel, collision, AppFrame::Math::ToDX(plyCapsulePos1), AppFrame::Math::ToDX(plyCapsulePos2), playerCapsuleRadius);
      //当たり判定の結果を確認
      if (result.HitNum > 0) {
         //当たっていたらプレイヤーの当たり判定結果を雑魚敵と当たったと設定
         player.collisionComponent().report().id(ReportId::HitFromPoorEnemy);
         //当たった位置にラージエネミーの位置を設定
         player.collisionComponent().hitPos(_owner.position());
         //ダメージの設定
         player.collisionComponent().damage(20.0);
      }
   }
}

void CollisionComponent::BulletFromPoorEnemy() {
   //ガトリング攻撃をしてくる雑魚敵のモデルハンドルの取得
   auto poorEnemyGatlingModel = _owner.modelAnimeComponent().modelHandle();
   //モデルのコリジョンフレームの取得
   auto collision = _owner.modelAnimeComponent().FindFrame("mob");
   //オブジェクトサーバーの各オブジェクトを取得
   for (auto&& object : _owner.GetObjServer().runObjects()) {
      //遠隔弱攻撃の弾じゃなかったらなにもしない
      if (object->GetObjType() != Object::ObjectBase::ObjectType::Bullet) {
         continue;
      }
      //遠隔弱攻撃の弾の球の作成
      //球の位置
      auto bullet = object->position();
      //球の半径
      auto bulletRadius = static_cast<float>(20.0);
      //モデルと球の当たり判定を取る
      auto result = MV1CollCheck_Sphere(poorEnemyGatlingModel, collision, AppFrame::Math::ToDX(bullet), bulletRadius);
      //当たり判定の結果を確認
      if (result.HitNum > 0) {
         //当たっていたら遠隔弱攻撃の弾の当たり判定結果をガトリング攻撃をしてくる雑魚敵と当たったと設定
         object->collisionComponent().report().id(ReportId::HitFromPoorEnemyGatling);
         //ダメージの設定
         _owner.collisionComponent().damage(10.0);
         //ガトリング攻撃をしてくる雑魚敵の当たり判定結果を遠隔弱攻撃の弾と当たったと設定
         _owner.collisionComponent().report().id(ReportId::HitFromBullet);
      }
   }
}

void CollisionComponent::PoorEnemyGatlingFromObjectModel() {
   //落下するオブジェクトのカプセルの作成
   auto fallObjectPos = _owner.position();
   //カプセルの一つ目の位置
   auto capsulePos1 = Vector4(0.0, FallObjectCapsulePos1, 0.0) + fallObjectPos;
   //カプセルの二つ目の位置
   auto capsulePos2 = Vector4(0.0, FallObjectCapsulePos2, 0.0) + fallObjectPos;
   //カプセルの半径
   auto capsuleRadian = static_cast<float>(FallObjectRadius);
   //オブジェクトサーバーの各オブジェクトを取得
   for (auto&& object : _owner.GetObjServer().runObjects()) {
      //ガトリング攻撃をしてくる雑魚敵じゃなければ何もしない
      if (object->GetObjType() != Object::ObjectBase::ObjectType::PoorEnemy) {
         continue;
      }
      //ガトリング攻撃をしてくる雑魚敵のモデルハンドルを取得
      auto poorEnemyGatlingModel = object->modelAnimeComponent().modelHandle();
      //モデルのコリジョンフレーム番号を取得
      auto collision = object->modelAnimeComponent().FindFrame("mob");
      //モデルとカプセルの当たり判定を取る
      auto result = MV1CollCheck_Capsule(poorEnemyGatlingModel, collision, AppFrame::Math::ToDX(capsulePos1), AppFrame::Math::ToDX(capsulePos2), capsuleRadian);
      //当たり判定の結果を確認
      if (result.HitNum > 0) {
         //ガトリング攻撃をしてくる雑魚敵の当たり判定結果を落下するオブジェクトと当たったと設定
         object->collisionComponent().report().id(ReportId::HitFromFallObject);
         //ダメージの設定
         object->collisionComponent().damage(20.0);
         //落下するオブジェクトの当たり判定結果をガトリング攻撃をしてくる雑魚敵と当たったと設定
         _owner.collisionComponent().report().id(ReportId::HitFromPoorEnemyGatling);
      }
   }
}

void CollisionComponent::PlayerKnockBack() {
   // オブジェクトサーバーの各オブジェクトを取得
   for (auto&& object : _owner.GetObjServer().runObjects()) {
      // プレイヤーじゃなかったら何もしない
      if (object->GetObjType() != Object::ObjectBase::ObjectType::Player) {
         continue;
      }
      // プレイヤーがノックバックしているか
      if (object->collisionComponent().knockBack()) {
         // ノックバックしていたらプレイヤーがノックバックしていると設定
         _owner.collisionComponent().knockBack(true);
      }
   }
}

AppFrame::Math::Vector4 CollisionComponent::PlayerCheckStage(const Vector4& pos, const Vector4& moved) {
   for (auto&& object : _owner.gameMain().objServer().runObjects()) {
      if (object->GetObjType() == Object::ObjectBase::ObjectType::Player) {
         auto& player = dynamic_cast<Player::Player&>(*object);
         // プレイヤーが死亡モーションならば返す
         if (player.isDeadMotion()) {
            return pos;
         };
      }
   }
   auto modeBase = _owner.gameMain().modeServer().GetNowMode();
   auto modeIngame = std::dynamic_pointer_cast<Mode::ModeInGameBase>(modeBase);
   auto stageComponent = modeIngame->GetStage().stageComponent();
    
   auto [handle, collision] = stageComponent.GetHandleAndFrameNum("stage_character_c");

   auto newPos = pos + moved;
   auto start = newPos + Vector4(0.0, 50.0, 0.0);
   auto end = newPos + Vector4(0.0, -10000.0, 0.0);
   auto result = MV1CollCheck_Line(handle, collision, AppFrame::Math::ToDX(start), AppFrame::Math::ToDX(end));
   if (result.HitFlag != 0) {
      newPos = AppFrame::Math::ToMath(result.HitPosition);
      return newPos;
   }
   else {
      return pos;
   }
}

AppFrame::Math::Vector4 CollisionComponent::LargeEnemyCheckStage(const Vector4& pos, const Vector4& moved) {
   for (auto&& object : _owner.gameMain().objServer().runObjects()) {
      if (object->GetObjType() == Object::ObjectBase::ObjectType::Player) {
         // プレイヤーが死亡モーションならば返す
         auto& player = dynamic_cast<Player::Player&>(*object);
         if (player.isDeadMotion()) {
            return pos;
         };
      }
   }
   auto modeBase = _owner.gameMain().modeServer().GetNowMode();
   auto modeIngame = std::dynamic_pointer_cast<Mode::ModeInGameBase>(modeBase);
   auto stageComponent = modeIngame->GetStage().stageComponent();

   auto [handle, collision] = stageComponent.GetHandleAndFrameNum("stage_boss_c");

   auto newPos = pos + moved;
   auto start = newPos + Vector4(0.0, 50.0, 0.0);
   auto end = newPos + Vector4(0.0, -10000.0, 0.0);
   auto result = MV1CollCheck_Line(handle, collision, AppFrame::Math::ToDX(start), AppFrame::Math::ToDX(end));
   if (result.HitFlag != 0) {
      newPos = AppFrame::Math::ToMath(result.HitPosition);
      return newPos;
   }
   else {
      return pos;
   }
}

AppFrame::Math::Vector4 CollisionComponent::PoorEnemyCheckStage(const Vector4& pos, const Vector4& moved) {
   for (auto&& object : _owner.gameMain().objServer().runObjects()) {
      if (object->GetObjType() == Object::ObjectBase::ObjectType::Player) {
         // プレイヤーが死亡モーションならば返す
         auto& player = dynamic_cast<Player::Player&>(*object);
         if (player.isDeadMotion()) {
            return pos;
         };
      }
   }
   auto modeBase = _owner.gameMain().modeServer().GetNowMode();
   auto modeIngame = std::dynamic_pointer_cast<Mode::ModeInGameBase>(modeBase);
   auto stageComponent = modeIngame->GetStage().stageComponent();

   auto [handle, collision] = stageComponent.GetHandleAndFrameNum("stage_character_c");

   auto newPos = pos + moved;
   auto start = newPos + Vector4(0.0, 50.0, 0.0);
   auto end = newPos + Vector4(0.0, -10000.0, 0.0);
   auto result = MV1CollCheck_Line(handle, collision, AppFrame::Math::ToDX(start), AppFrame::Math::ToDX(end));
   if (result.HitFlag) {
      newPos = AppFrame::Math::ToMath(result.HitPosition);
      return newPos;
   }
   else {
      return pos;
   }
}

void CollisionComponent::OutStage() {
   for (auto&& object : _owner.gameMain().objServer().runObjects()) {
      if (object->GetObjType() == Object::ObjectBase::ObjectType::Player) {
         auto& player = dynamic_cast<Player::Player&>(*object);
         // プレイヤーが死亡モーションならば返す
         if (player.isDeadMotion()) {
            return;
         };
      }
   }
   auto modeBase = _owner.gameMain().modeServer().GetNowMode();
   auto modeInGameBase = std::dynamic_pointer_cast<Mode::ModeInGameBase>(modeBase);
   auto stageComponent = modeInGameBase->GetStage().stageComponent();

   auto [handle, collision] = stageComponent.GetHandleAndFrameNum("stage_object_c");
   // 位置の取得
   auto position = _owner.position();
   // 線分の作成
   // 始点
   auto start = position + Vector4(0.0, 50.0, 0.0);
   // 終点
   auto end = position + Vector4(0.0, -10000.0, 0.0);

   auto result = MV1CollCheck_Line(handle, collision, AppFrame::Math::ToDX(start), AppFrame::Math::ToDX(end));

   if (!result.HitFlag) {
      _owner.collisionComponent().report().id(ReportId::OutStage);
      return;
   }
   return;
}

bool CollisionComponent::IsLineFromStage(const Vector4& pos) {
   for (auto&& object : _owner.gameMain().objServer().runObjects()) {
      if (object->GetObjType() == Object::ObjectBase::ObjectType::Player) {
         auto& player = dynamic_cast<Player::Player&>(*object);
         // プレイヤーが死亡モーションならば返す
         if (player.isDeadMotion()) {
            return false;
         };
      }
   }
   auto modeBase = _owner.gameMain().modeServer().GetNowMode();
   auto modeInGameBase = std::dynamic_pointer_cast<Mode::ModeInGameBase>(modeBase);
   auto stageComponent = modeInGameBase->GetStage().stageComponent();

   auto [handle, collision] = stageComponent.GetHandleAndFrameNum("stage_character_c");
   // 位置の取得
   auto [x, y, z] = pos.GetVec3();
   // 線分の作成
   auto start = Vector4(x, 50.0, z);   // 始点
   auto end = Vector4(x, -10000.0, z); // 終点

   auto result = MV1CollCheck_Line(handle, collision, AppFrame::Math::ToDX(start), AppFrame::Math::ToDX(end));

   return result.HitFlag;
}
