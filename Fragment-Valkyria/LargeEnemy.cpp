
/*****************************************************************//**
 * \file   LargeEnemy.cpp
 * \brief  ラージエネミーの処理を回すクラス
 * 
 * \author AHMD2000, NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "LargeEnemy.h"
#include "CameraComponent.h"
#include "GameMain.h"
#include "ObjectFactory.h"
#include "CollisionComponent.h"
#include "ModelAnimeComponent.h"
#include "PoorEnemyGatling.h"
#include "ObjectServer.h"
#include "ObjectFactory.h"
#include "FallObject.h"
#include "EffectServer.h"
#include "EffectBossCharge.h"
#include "EffectBossStan.h"
#include "EffectBossFall.h"
#include "EffectPreliminaryLight.h"
#include "EffectBossCrash.h"

namespace {
   auto largeEnemyParamMap = AppFrame::Resource::LoadParamJson::GetParamMap("largeenemy", {
      "gatling_frame", "max_stun", "stun_decrease", "object_stun_value",
      "bullet_stun_value", "hp", "gravity", "consecutive_fall_object_frame",
      "consecutive_num", "action_cooltime", "object_max_num","max_wave","max_poorenemy" });
   const int GatlingFrame = largeEnemyParamMap["gatling_frame"];                                                   //!< ガトリングを打つフレームの間隔
   const double MaxStun = largeEnemyParamMap["max_stun"];                                                          //!< スタン値の最大値
   const double StunDecrease = largeEnemyParamMap["stun_decrease"];                                                //!< 毎フレーム減らすスタン値の値
   const double ObjectStunValue = largeEnemyParamMap["object_stun_value"];                                         //!< オブジェクト攻撃を受けた時に増えるスタン値
   const double BulletStunValue = largeEnemyParamMap["bullet_stun_value"];                                         //!< 遠隔弱攻撃を受けた時に増えるスタン値
   const double MaxHitPoint = largeEnemyParamMap["hp"];                                                            //!< ヒットポイントの最大値
   const double Gravity = largeEnemyParamMap["gravity"];                                                           //!< 重力
   const int ConsecutiveFallObjectFrame = largeEnemyParamMap["consecutive_fall_object_frame"];                     //!< オブジェクト連続落下攻撃のフレーム数
   const int ConsecutiveNum = largeEnemyParamMap["consecutive_num"];                                               //!< オブジェクト連続落下攻撃の連続回数
   const int ActionCoolTime = largeEnemyParamMap["action_cooltime"];                                               //!< 行動を行うクールタイム
   const int MaxNum = largeEnemyParamMap["object_max_num"];                                                        //!< 落下オブジェクトの最大数
   const int MaxWave= largeEnemyParamMap["max_wave"];                                                              //!< 最大ウェイブ数
   const int MaxPoorEnemy = largeEnemyParamMap["max_poorenemy"];                                                   //!< 雑魚敵の最大数

   constexpr auto FootStepHeight = 40.0;                                                                           //!< 走り状態時の足音発生高さ
   constexpr auto LaserDiffPos = 150.0;                                                                            //!< レーザー生成位置に一番近い位置のフレームからのオフセット
   constexpr auto LaserIrradiationTime = 155.f;                                                                    //!< アニメーション始まってからのレーザーの照射時間
   constexpr auto MinWave = 2;
}

using namespace FragmentValkyria::Enemy;

LargeEnemy::LargeEnemy(Game::GameMain& gameMain) : ObjectBase{ gameMain } {

}

void LargeEnemy::Init() {
   // モデルのハンドルの取得
   auto modelHandle = _modelAnimeComponent->modelHandle();
   // モデルの全体のコリジョンフレームの番号の取得
   _wholeCollision = _modelAnimeComponent->FindFrame("S301_typeCO");
   // コリジョンフレームをナビメッシュとして使用
   MV1SetupCollInfo(modelHandle, _bodyCollision, 7, 6, 5);
   // モデルの胴体のコリジョンフレームの番号の取得
   _bodyCollision = _modelAnimeComponent->FindFrame("pasted__body_collision");
   // コリジョンフレームをナビメッシュとして使用
   MV1SetupCollInfo(modelHandle, _bodyCollision, 6, 6, 5);
   // モデルの弱点のコリジョンフレームの番号の取得
   _weakNessesCollision = _modelAnimeComponent->FindFrame("pasted__weak_collision");
   // コリジョンフレームをナビメッシュとして使用
   MV1SetupCollInfo(modelHandle, _weakNessesCollision, 1, 1, 1);
   // モデルの顔のコリジョンフレームの番号の取得
   _faceCollision = _modelAnimeComponent->FindFrame("pasted__face_collision");
   // コリジョンフレームをナビメッシュとして使用
   MV1SetupCollInfo(modelHandle, _faceCollision, 3, 2, 1);
   // ヒットポイントの設定
   _hp = MaxHitPoint;
   // 行動に追加する状態の文字列の動的配列を作成
   _actionList.emplace_back("FallObject");
   _actionList.emplace_back("Gatling");
   _actionList.emplace_back("Move");
   _actionList.emplace_back("Laser");
   _actionList.emplace_back("FanGatling");
}

void LargeEnemy::Input(InputManager& input) {
   // 各状態の入力処理を回す
   _stateServer->Input(input);
}

void LargeEnemy::Update() {
   // コリジョン情報の更新
   MV1RefreshCollInfo(_modelAnimeComponent->modelHandle(), _wholeCollision);
   MV1RefreshCollInfo(_modelAnimeComponent->modelHandle(), _bodyCollision);
   MV1RefreshCollInfo(_modelAnimeComponent->modelHandle(), _weakNessesCollision);
   MV1RefreshCollInfo(_modelAnimeComponent->modelHandle(), _faceCollision);
   // 状態の更新
   _stateServer->Update();
   // ワールド行列の更新
   ComputeWorldTransform();
   // モデルの更新
   _modelAnimeComponent->Update();
   // オブジェクトサーバーの参照を取得
   auto& objServer = GetObjServer();
   // オブジェクトサーバーに位置を通知
   objServer.RegistVector("LargeEnemyPos", _position);
   // オブジェクトサーバーにヒットポイントを通知
   objServer.RegistDouble("LargeEnemyHP", _hp);
}

void LargeEnemy::Draw() {
   // 各状態の描画処理を回す
   _stateServer->Draw();
}

void LargeEnemy::CreateGatling() {
   auto& objServer = GetObjServer();
   // ガトリングを生成する位置を取得
   auto gatlingFramePos = _modelAnimeComponent->GetFrameChildPosion("root", "gatling3");
   // ガトリングを生成する座標を設定
   objServer.RegistVector("GatlingPos", gatlingFramePos);
   // ガトリングを打つ向きの設定
   auto gatlingDirection = _position + _rotateDir * 100.0 - gatlingFramePos;
   // オブジェクトサーバーにガトリングを打つ向きを通知
   objServer.RegistVector("GatlingMoveDirection", gatlingDirection);
   // ガトリングを生成してオブジェクトサーバーへ追加
   auto gatling = gameMain().objFactory().Create("Gatling");
   objServer.Add(std::move(gatling));
}

void LargeEnemy::CreateLaser() {
   // レーザーを生成してオブジェクトサーバーへ追加
   auto laser = gameMain().objFactory().Create("Laser");
   GetObjServer().Add(std::move(laser));
}

void LargeEnemy::CreateFallObject() {
   using Vector4 = AppFrame::Math::Vector4;
   using Matrix44 = AppFrame::Math::Matrix44;
   // 生成する数を生成されている落下オブジェクトの数に設定
   _createNum = 3;
   // オブジェクトサーバーの各オブジェクトの取得
   for (auto&& object : _gameMain.objServer().runObjects()) {
      // 落下オブジェクトじゃなかったら処理をスキップして戻る
      if (object->GetObjType() != Object::ObjectBase::ObjectType::FallObject) {
         continue;
      }
      // 落下オブジェクトだったら生成されている落下オブジェクトの数を増やす
      ++_createNum;
   }
   // 生成されている落下オブジェクトの数が落下オブジェクトの最大数より大きいか確認
   if (_createNum > MaxNum) {
      // 生成されている落下オブジェクトの数が落下オブジェクトの最大数より大きかったらオブジェクトサーバーの各オブジェクトを取得
      for (auto&& object : _gameMain.objServer().runObjects()) {
         // 落下オブジェクトじゃなかったら処理をスキップして戻る
         if (object->GetObjType() != Object::ObjectBase::ObjectType::FallObject) {
            continue;
         }
         // 落下オブジェクトだったら落下オブジェクトの参照にキャスト
         auto& fallObject = dynamic_cast<Enemy::FallObject&>(*object);
         // 落下オブジェクトが残留オブジェクトか確認
         if (fallObject.residual()) {
            // 落下オブジェクトが残留オブジェクトだったら死亡状態に設定
            object->SetDead();
            // 生成されている落下オブジェクトの数を減らす
            --_createNum;
            // 生成されている落下オブジェクトの数が落下オブジェクトの最大数以下になったら処理を終了
            if (_createNum <= MaxNum) {
               break;
            }
         }
      }
   }
   // 右に既定の角度回転させるマトリクスの生成
   auto rightTransMatrix = Matrix44();
   rightTransMatrix.RotateY(30.0, true);
   // 左に既定の角度回転させるマトリクスの生成
   auto leftTransMatrix = Matrix44();
   leftTransMatrix.RotateY(-30.0, true);
   // ランダムに右か左に既定の角度回転させるマトリクスの作成
   auto randomdegree = AppFrame::Math::Utility::GetRandom(0, 1) ? 60.0 : -60.0;
   auto randomTransMatrix = Matrix44();
   randomTransMatrix.RotateY(randomdegree, true);
   // プレイヤーの位置の取得
   auto playerPos = GetObjServer().GetVecData("PlayerPos");
   // プレイヤーへのベクトルの取得
   auto MoveVec = playerPos - _position;
   // プレイヤーへの距離の取得
   auto LargeEnemyToPlyDistance = MoveVec.Lenght();
   // プレイヤーへのベクトルを単位化する
   MoveVec.Normalized();
   // プレイヤーへの向きのベクトルを右に既定の角度回転させる
   auto rightMoveVec = MoveVec * rightTransMatrix;
   // プレイヤーへの向きのベクトルを左に既定の角度回転させる
   auto leftMoveVec = MoveVec * leftTransMatrix;
   // プレイヤーへの向きのベクトルを右か左にランダムに既定の角度回転させる
   auto randomtMoveVec = MoveVec * randomTransMatrix;
   // 既定の角度回転させた向きから落下オブジェクトを落とす距離の設定
   auto distance = LargeEnemyToPlyDistance * 0.5;
   // ランダムに回転させた向きから落下オブジェクトを落とす距離の設定
   auto randomDistance = LargeEnemyToPlyDistance * 0.3;
   // 落下オブジェクトを落とす位置の配列の作成
   std::array<Vector4, 4> startPosition = {
      playerPos + Vector4(0.0, 500.0, 0.0),
      _position + Vector4(0.0, 500.0, 0.0) + (rightMoveVec * distance),
      _position + Vector4(0.0, 500.0, 0.0) + (leftMoveVec * distance),
      _position + Vector4(0.0, 500.0, 0.0) + (randomtMoveVec * randomDistance),
   };
   // 落下オブジェクトを生成しオブジェクトサーバーに追加
   for (auto i = 0; i < 4; ++i) {
      auto fallObject = gameMain().objFactory().Create("FallObject");
      // 生成された回数目の位置の配列の要素に位置を設定
      fallObject->position(startPosition[i]);
      GetObjServer().Add(std::move(fallObject));
   }
}

void LargeEnemy::SetLaserPosition() {
   // オブジェクトサーバーの各オブジェクトの取得
   for (auto&& objects : _gameMain.objServer().runObjects()) {
      // 落下オブジェクトか確認
      if (objects->GetObjType() == Object::ObjectBase::ObjectType::FallObject) {
         // 落下オブジェクトだったら
         // 落下オブジェクトからプレイヤーへのベクトルの取得
         auto fallObjectToPly = _gameMain.objServer().GetVecData("PlayerPos") - objects->position();
         // 落下オブジェクトからプレイヤーへのベクトルの各成分を分解
         auto [x, y, z] = fallObjectToPly.GetVec3();
         // 落下オブジェクトからプレイヤーへのベクトルの距離の2乗を取得
         auto checkSize = x * x + y * y + z * z;
         // 落下オブジェクトからプレイヤーへのベクトルの距離の2乗と落下オブジェクトの位置のペアを作成
         auto checkPair = std::make_pair(checkSize, objects->position());
         // 落下オブジェクトからプレイヤーへのベクトルの距離の2乗と落下オブジェクトの位置のペアの動的配列へ追加
         _objectDistance.emplace_back(checkPair);
         continue;
      }
   }
   // 落下オブジェクトからプレイヤーへの距離の2乗の値を比べて昇順に並び変える
   std::sort(_objectDistance.begin(), _objectDistance.end());
   // 落下オブジェクトがなかった場合プレイヤーの位置にレーザーを打つ向きを設定
   if (_objectDistance.empty()) {
      GetObjServer().RegistVector("LaserDirectionPos", _gameMain.objServer().GetVecData("PlayerPos"));
   }
   // 落下オブジェクトがあった場合
   else {
      // 距離の2乗の値が一番プレイヤーと近い落下オブジェクトの位置の成分を分解
      auto [x, y, z] = _objectDistance[0].second.GetVec3();
      // レーザーの打つ向きに高さをなくした落下オブジェクトへの向きを設定
      GetObjServer().RegistVector("LaserDirectionPos", Vector4(x, 0.0, z));
   }
   // 各落下オブジェクトからプレイヤーへの距離の2乗と各落下オブジェクトの位置のペアの動的配列のすべての要素を削除する
   _objectDistance.clear();
}

void LargeEnemy::HitCheckFromFallObject() {
   // 当たり判定結果クラスの参照の取得
   auto report = _collisionComponent->report();
   // 当たり判定結果の確認
   if (report.id() == Collision::CollisionComponent::ReportId::HitFromFallObject) {
      // 落下オブジェクトと当たっていたら
      // ヒットポイントをダメージ量分減らす
      _hp -= _collisionComponent->damage();
      // ダメージ量が0じゃなかった場合
      if (_collisionComponent->damage() != 0.0) {
         // スタン値を既定の値増やす
         _stunValue += ObjectStunValue;
      }
      // ヒットポイントが0以下だった場合死亡状態へ
      if (_hp <= 0) {
         _stateServer->GoToState("Die");
         // スタン値の設定
         _stunValue = 0.0;
      }
      // 当たり判定の結果を当たっていないと設定
      _collisionComponent->report().id(Collision::CollisionComponent::ReportId::None);
   }

}

void LargeEnemy::HitCheckFromBullet() {
   // 当たり判定結果クラスの参照の取得
   auto report = _collisionComponent->report();
   // 当たり判定結果の確認
   if (report.id() == Collision::CollisionComponent::ReportId::HitFromBullet) {
      // 遠隔弱攻撃の弾と当たっていたら
      // ヒットポイントをダメージ量分減らす
      _hp -= _collisionComponent->damage();
      // スタン値を既定の値増やす
      _stunValue += BulletStunValue;
      // ヒットポイントが0以下だった場合死亡状態へ
      if (_hp <= 0) {
         _stateServer->GoToState("Die");
         // スタン値の設定
         _stunValue = 0.0;
      }
      // 当たり判定の結果を当たっていないと設定
      _collisionComponent->report().id(Collision::CollisionComponent::ReportId::None);
   }
}

void LargeEnemy::Move(const Vector4& moved) {
   // 移動量のベクトルの成分を分解
   auto [x, y, z] = moved.GetVec3();
   // 位置を保存
   auto position = _position;
   // x成分の移動後の位置を取得
   position = _collisionComponent->LargeEnemyCheckStage(position, Vector4(x, y, 0.0));
   // z成分の移動後の位置を取得
   position = _collisionComponent->LargeEnemyCheckStage(position, Vector4(0.0, y, z));
   // 位置にxとzの成分の移動後の位置を設定する
   _position = position;
}

void LargeEnemy::Action() {
   // ヒットポイントがヒットポイントの最大値の50％以下になった最初の行動の場合
   if (_hp <= MaxHitPoint * 0.5 && _firstAngryAction) {
      // 各行動状態への文字列の動的配列にオブジェクト連続落下状態を追加
      _actionList.emplace_back("Consecutive");
      // オブジェクト連続落下状態へ
      _stateServer->GoToState("Consecutive");
      // ヒットポイントがヒットポイントの最大値の50％以下になった最初の行動でないと設定
      _firstAngryAction = false;
      // 処理をスキップして戻る
      return;
   }
   // 行動させる範囲を制限した行動状態への文字列の動的配列が空だったら各行動状態への文字列の動的配列を代入する
   if (_action.empty()) {
      _action = _actionList;
   }
   // 行動状態への文字列の動的配列の要素の個数が最大だった場合
   if (_action.size() >= _actionList.size()) {
      // 最初の行動状態へ移動
      _stateServer->GoToState(_action[0]);
      // 最初の行動状態への文字列を動的配列から削除
      _action.erase(_action.begin());
   }
   else {
      // 0から行動させる範囲を制限した行動状態への文字列の動的配列の要素の個数を-1した数の範囲の値をランダムに取得
      auto random = AppFrame::Math::Utility::GetRandom(0, static_cast<int>(_action.size()) - 1);
      // ランダムに取得した値番目の行動状態へ移動
      _stateServer->GoToState(_action[random]);
      // 移動した行動状態への文字列を動的配列から削除
      _action.erase(_action.begin() + random);
   }
}

void LargeEnemy::AugularRotate(bool& rotating) {
   // 各速度分回転させる
   _rotation.Add(0.0, _addRotate, 0.0);
   // 向いている方向へ回転させるマトリクスの作成
   Matrix44 rotateY = Matrix44();
   rotateY.RotateY(_rotation.GetY(), true);
   // 初期で向いている方向を90度回転させた方向のベクトルの作成
   Vector4 forward = Vector4(-1.0, 0.0, 0.0);
   // フォワードベクトルを90度回転させたベクトルの作成
   forward = forward * rotateY;
   // 単位化をする
   forward.Normalized();
   // 向かせたい方向とフォワードベクトルを90度回転させたベクトルの内積の結果を取得
   auto dot = _moved.Dot(forward);
   // 各速度が0より大きいか確認
   if (_addRotate > 0.0) {
      // 各速度が0より大きいとき内積の結果が0より小さくなったら回転しないと設定
      if (dot < 0) {
         rotating = false;
      }
   }
   else {
      // 各速度が0より小さいとき内積の結果が0より大きくなったら回転しないと設定
      if (dot >= 0) {
         rotating = false;
      }
   }
}

void LargeEnemy::AreaRotate(bool& rotating) {
   // モデルがデフォルトでZ座標のマイナス方向を向いているのでフォワードベクトルを反転させておく
   auto forward = GetForward() * -1.0;
   // 回転の向きのベクトルを単位化する
   _rotateDir.Normalized();
   // フォワードベクトルと回転の向きのベクトルとの内積の結果を確認
   if (forward.Dot(_rotateDir) <= -0.9) {
      // 内積の結果から真逆に近かった場合
      // ベクトルを3度回転させるマトリクスを作る
      Matrix44 dirRotate;
      dirRotate.RotateY(3.0, true);
      // 回転の向きのベクトルを回転させる
      _rotateDir = _rotateDir * dirRotate;
    }
   // 回転の向きのベクトルを既定の値大きくする
   _rotateDir = _rotateDir * _rotateEnlarge;
   // フォワードベクトルと回転の向きのベクトルとの三角形の面積の取得
   auto area = forward.Cross(_rotateDir);
   auto rotateValue = 0.5 * area.GetY();
   // 三角形の面積の値が0に近かった場合回転しないと設定
   if (rotateValue <= 0.0 && rotateValue >= -0.1 || rotateValue >= 0.0 && rotateValue <= 0.1) {
      rotating = false;
   }
   // 面積の値分回転させる(計算の順番をフォワードベクトルから向かせたい方向のベクトルにすることで計算結果の符号をそのまま利用することで最短距離の回転を行う)
   _rotation.Add(0.0, rotateValue, 0.0);
}

void LargeEnemy::SetAddRotate() {
   // 向いている方向へ回転させるマトリクスの作成
   Matrix44 rotateY = Matrix44();
   rotateY.RotateY(_rotation.GetY(), true);
   // 向いている方向を90度回転させたベクトルの作成
   Vector4 forward = Vector4(-1.0, 0.0, 0.0) * rotateY;
   // 単位化する
   forward.Normalized();
   // 向かせたい方向とフォワードベクトルを90度回転させたベクトルの内積の結果を取得
   _rotateDot = _moved.Dot(forward);
   // 内積の結果が0より大きい場合各速度をプラスにする
   if (_rotateDot >= 0) {
      _addRotate = 1.0;
   }
   // 内積の結果が0より大きくない場合各速度をマイナスにする
   else {
      _addRotate = -1.0;
   }
}

void LargeEnemy::StunCheck() {
   // スタン値の更新
   _stunValue -= StunDecrease;
   // スタン値が既定の値以上の場合
   if (_stunValue >= MaxStun) {
      // 気絶状態へ
      _stateServer->GoToState("Stun");
   }
   // スタン値が0以下の場合
   else if (_stunValue <= 0.0) {
      // スタン値を0に
      _stunValue = 0.0;
   }
}

void LargeEnemy::StateBase::Draw() {
   // モデルの描画処理を回す
   _owner._modelAnimeComponent->Draw();
}

void LargeEnemy::StateFall::Enter() {
   // この状態になった時のゲームのフレームカウントの保存
   _stateCnt = _owner._gameMain.modeServer().frameCount();
   // モデルのアニメーションの設定
   _owner._modelAnimeComponent->ChangeAnime("idle", true);
}

void LargeEnemy::StateFall::Update() {
   // この状態に入ってからのフレーム数の取得
   auto cnt = _owner._gameMain.modeServer().frameCount() - _stateCnt;
   // 位置のY成分に足していく値の取得
   auto addY = -0.5 * Gravity * cnt * cnt;
   // 位置の更新
   _owner._position.Add(0.0, addY, 0.0);
   // 位置のY成分が0以下だったら
   if (_owner._position.GetY() <= 0.0) {
      // 位置のY成分を0にする
      auto [x, y, z] = _owner._position.GetVec3();
      _owner._position = Vector4(x, 0.0, z);
      // 待機状態へ
      _owner._stateServer->GoToState("Idle");
   }
   // 当たり判定処理を行うクラスでプレイヤーがラージエネミーと当たっているか確認
   _owner._collisionComponent->LargeEnemyFromPlayer();
}

void LargeEnemy::StateFall::Exit() {
   auto efcFall = std::make_unique<Effect::EffectBossFall>(_owner._gameMain,"BossFall");
   efcFall->position(_owner._position);
   _owner.GetEfcServer().Add(std::move(efcFall));
   _owner._cameraComponent->SetVibValue(0.0);
   _owner.GetSoundComponent().PlayLoop("BossBattleBgm");
}

void LargeEnemy::StateIdle::Enter() {
   // この状態になった時のゲームのフレームカウントの保存
   _stateCnt = _owner.gameMain().modeServer().frameCount();
   // モデルのアニメーションの設定
   _owner._modelAnimeComponent->ChangeAnime("idle", true);
}

void LargeEnemy::StateIdle::Update() {
   // ゲームのフレームカウントの取得
   auto gameCount = _owner.gameMain().modeServer().frameCount();
   // この状態に入ってからの経過フレーム数の取得
   auto count = static_cast<int>(gameCount) - _stateCnt;
   // 一定フレーム数たったら行動をする
   if (count >= ActionCoolTime) {
      _owner.Action();
   }
   // 当たり判定処理を行うクラスでプレイヤーがラージエネミーと当たっているか確認
   _owner._collisionComponent->LargeEnemyFromPlayer();
   // 落下オブジェクトと当たったか確認
   _owner.HitCheckFromFallObject();
   // 遠隔弱攻撃の弾と当たっているか確認
   _owner.HitCheckFromBullet();
   // スタン値の更新と確認
   _owner.StunCheck();
}

void LargeEnemy::StateFallObject::Enter() {
   // モデルのアニメーションの設定
   _owner._modelAnimeComponent->ChangeAnime("object_attack", false);
}

void LargeEnemy::StateFallObject::Update() {
   // アニメーションを繰り返した回数の取得
   auto cnt = _owner._modelAnimeComponent->repeatedCount();
   // アニメーションを1回行ったら待機状態へ
   if (cnt > 0) {
      // カメラを振動させるためにカメラの振動に使うYの位置を0.0に設定
      _owner._cameraComponent->SetVibValue(0.0);
      // 落下オブジェクトの生成
      _owner.CreateFallObject();
      SetPoorSpawn();
      _owner._stateServer->GoToState("Idle");
   }
   // 当たり判定処理を行うクラスでプレイヤーがラージエネミーと当たっているか確認
   _owner._collisionComponent->LargeEnemyFromPlayer();
   // 落下オブジェクトと当たったか確認
   _owner.HitCheckFromFallObject();
   // 遠隔弱攻撃の弾と当たっているか確認
   _owner.HitCheckFromBullet();
   // スタン値の更新と確認
   _owner.StunCheck();
}

void LargeEnemy::StateGatling::Enter() {
   // ガトリング攻撃をするための回転をしていない時に経過させるフレームカウントの設定
   _gatlingFrameCnt = 0;
   // ガトリングの弾を打つ回数の設定
   _owner._gatlingCnt = 10;
   // 向かせたい方向のベクトルを大きくする値を設定
   _owner._rotateEnlarge = 50.0;
   // モデルのアニメーションの設定
   _owner._modelAnimeComponent->ChangeAnime("gatoring", true);
}

void LargeEnemy::StateGatling::Update() {
   // 回転していない時のフレームカウントが既定の値よりも大きく既定のフレーム数経過し攻撃していない場合
   if (_gatlingFrameCnt >= 100 && _gatlingFrameCnt % GatlingFrame == 0 && _owner._attack == false) {
      // 向かせたい方向の設定
      _owner._rotateDir = _owner.GetObjServer().GetVecData("PlayerPos") - _owner._position;
      // 攻撃していると設定
      _owner._attack = true;
      // 回転していると設定
      _owner._rotating = true;
      // 回転していない時のフレームカウントの更新
      ++_gatlingFrameCnt;
   }
   // 攻撃しているか確認
   if (_owner._attack) {
      // 攻撃している場合
      // 攻撃をする方向へ回転させる
      _owner.AreaRotate(_owner._rotating);
      // 回転が終了しているか確認
      if (!_owner._rotating) {
         // 終了していたらガトリングを生成
         _owner.CreateGatling();
         // ガトリングの弾を打つ回数の更新
         --_owner._gatlingCnt;
         // 鳴らすサウンドの設定
         _owner.GetSoundComponent().Play("BossGatling", _owner._position);
         // 攻撃していないと設定
         _owner._attack = false;
      }
   }
   // 攻撃していない場合回転していない時のフレームカウントの更新をする
   else {
      ++_gatlingFrameCnt;
   }
   // ガトリングを打つ回数が0以下になったら待機状態へ
   if (_owner._gatlingCnt <= 0) {
      _owner._stateServer->GoToState("Idle");
   }
   // 当たり判定処理を行うクラスでプレイヤーがラージエネミーと当たっているか確認
   _owner._collisionComponent->LargeEnemyFromPlayer();
   // 落下オブジェクトと当たったか確認
   _owner.HitCheckFromFallObject();
   // 遠隔弱攻撃の弾と当たっているか確認
   _owner.HitCheckFromBullet();
   // スタン値の更新と確認
   _owner.StunCheck();
}

void LargeEnemy::StateDie::Enter() {
   // モデルのアニメーションの設定
   _owner._modelAnimeComponent->ChangeAnime("dead", false, 0.5);
   auto efcCrash = std::make_unique<Effect::EffectBossCrash>(_owner._gameMain, "BossCrash");
   _owner.GetEfcServer().Add(std::move(efcCrash));
}

void LargeEnemy::StateDie::Update() {
   if (_owner._modelAnimeComponent->repeatedCount() > 0) {
      _owner._gameMain.modeServer().PushBack("MissionCompleted");
   }
}

void LargeEnemy::StateMove::Enter() {
   // この状態になった時のゲームのフレームカウントの保存
   _stateCnt = _owner.gameMain().modeServer().frameCount();
   // 移動後のプレイヤーへの向きのベクトルを取得すると設定
   _endGetplyPos = true;
   // 移動中最初に移動方向に回転すると設定
   _owner._firstRotating = true;
   // 移動中最後にプレイヤーの方向に回転すると設定
   _owner._endRotating = true;
   // モデルのアニメーションの設定
   _owner._modelAnimeComponent->ChangeAnime("walk_0", true);
   // 0か1をランダムに取得
   auto result = /*AppFrame::Math::Utility::GetRandom(0, 1)*/true;
   // 1だった場合
   if (result) {
      // プレイヤーへの向きのベクトルを取得
      _owner._moved = _owner.GetObjServer().GetVecData("PlayerPos") - _owner._position;
      // 単位化する
      _owner._moved.Normalized();
      // 回転の角速度の設定
      _owner.SetAddRotate();
      // 移動量のベクトルを設定
      _owner._moved = _owner._moved * 30.0;
   }
   // 0だった場合
   else {
      // ランダムに角度を取得
      auto degree = AppFrame::Math::Utility::GetRandom(0.0, 360.0);
      // 取得した角度回転させるマトリクスの作成
      Matrix44 rotate = Matrix44();
      rotate.RotateY(degree, true);
      // Z軸のプラス方向のベクトルを取得したマトリクス分回転させたベクトルを移動方向のベクトルに設定
      _owner._moved = Vector4(0.0, 0.0, 1.0) * rotate;
      // 単位化する
      _owner._moved.Normalized();
      // 回転の角速度の設定
      _owner.SetAddRotate();
      // 移動量のベクトルを設定
      _owner._moved = _owner._moved * 30.0;
   }
   _footCnt = 0;
}

void LargeEnemy::StateMove::Update() {
   // ゲームのフレームカウントの取得
   auto gameCount = _owner.gameMain().modeServer().frameCount();
   // この状態に入ってからの経過フレーム数の取得
   auto count = gameCount - _stateCnt;
   // 足音の処理
   FootStepSound();
   // 移動中最初に移動方向に回転する場合移動方向へ回転させる
   if (_owner._firstRotating) {
      _owner.AugularRotate(_owner._firstRotating);
   }
   // 移動方向への回転が終了した場合
   else {
      // 既定のフレーム数経過したら
      if (count >= 60 * 3) {
         // 移動後にプレイヤーへの向きを取得する場合
         if (_endGetplyPos) {
            // プレイヤーへの向きのベクトルを取得
            _owner._moved = _owner.GetObjServer().GetVecData("PlayerPos") - _owner._position;
            // 単位化する
            _owner._moved.Normalized();
            // 回転の各速度の設定
            _owner.SetAddRotate();
            // 移動後のプレイヤーへの向きを取得しないと設定
            _endGetplyPos = false;
         }
         // 移動中最後にプレイヤーの方向に回転する場合プレイヤーの方向へ回転させる
         if (_owner._endRotating) {
            _owner.AugularRotate(_owner._endRotating);
         }
         // 回転が終了した場合待機状態へ
         else {
            _owner._stateServer->GoToState("Idle");
         }
      }
      // 既定のフレーム数経過したら
      else if (count >= 60 * 1) {
         // 移動処理
         _owner.Move(_owner._moved);
      }
   }
   // 当たり判定処理を行うクラスでプレイヤーがラージエネミーと当たっているか確認
   _owner._collisionComponent->LargeEnemyFromPlayer();
   // 落下オブジェクトと当たったか確認
   _owner.HitCheckFromFallObject();
   // 遠隔弱攻撃の弾と当たっているか確認
   _owner.HitCheckFromBullet();
   // スタン値の更新と確認
   _owner.StunCheck();
}

void LargeEnemy::StateLaser::Enter() {
   // この状態になった時のゲームのフレームカウントの保存
   _stateCnt = _owner.gameMain().modeServer().frameCount();
   // レーザーを生成する位置を設定
   _owner.SetLaserPosition();
   // 攻撃する方向を設定
   _owner._rotateDir = _owner._gameMain.objServer().GetVecData("LaserDirectionPos") - _owner._position;
   // 向かせたい方向のベクトルを大きくする値を設定
   _owner._rotateEnlarge = 10.0;
   // 回転処理をすると設定
   _owner._rotating = true;
   // 攻撃をしていないと設定
   _owner._attack = false;
   // レーザー攻撃フラグを設定
   _owner._isLaser = true;
   // レーザーを生成しないと設定
   _createLaser = false;
   // モデルのアニメーションの設定
   _owner._modelAnimeComponent->ChangeAnime("beem", false);
   // 鳴らすサウンドの設定
   _owner.GetSoundComponent().Play("BossCharge",_owner._position);
   // レーザー生成位置を取得
   auto laserPos = GetLaserPos();
   // レーザー生成位置をオブジェクトサーバーに登録
   _owner.GetObjServer().RegistVector("LaserPos", laserPos);
   // チャージエフェクトのインスタンスを生成
   auto efcCharge = std::make_unique<Effect::EffectBossCharge>(_owner._gameMain, "BossCharge");
   // エフェクトの位置設定
   efcCharge->position(laserPos);
   // エフェクトサーバーに登録
   _owner.GetEfcServer().Add(std::move(efcCharge));

}

void LargeEnemy::StateLaser::Update() {
   // ゲームのフレームカウントの取得
   auto gameCount = _owner.gameMain().modeServer().frameCount();
   // この状態に入ってからの経過フレーム数の取得
   auto count = gameCount - _stateCnt;
   // アニメーション再生が終了したら待機状態へ
   auto repeated = _owner._modelAnimeComponent->repeatedCount();
   auto playTime = _owner._modelAnimeComponent->playTime();
   // 攻撃する方向へ回転させる
   _owner.AreaRotate(_owner._rotating);
   if (repeated >= 1) {
      _owner._stateServer->GoToState("Idle");
   }
   // 既定のフレーム数経過していてレーザーを生成しないとなっていたら
   else if (playTime > LaserIrradiationTime && !_createLaser) {
      // 攻撃していると設定
      _owner._attack = true;
      // レーザーを生成すると設定
      _createLaser = true;
   }
   // 攻撃しているか確認
   if (_owner._attack) {
      // 攻撃していた場合
      // 回転が終了したら
      if (!_owner._rotating) {
         // レーザーを生成する
         _owner.CreateLaser();
         // 鳴らすサウンドの設定
         _owner.GetSoundComponent().Play("BossBeam", _owner._position);
         // 攻撃していないと設定
         _owner._attack = false;
      }
   }
   // 当たり判定処理を行うクラスでプレイヤーがラージエネミーと当たっているか確認
   _owner._collisionComponent->LargeEnemyFromPlayer();
   // 落下オブジェクトと当たったか確認
   _owner.HitCheckFromFallObject();
   // 遠隔弱攻撃の弾と当たっているか確認
   _owner.HitCheckFromBullet();
   // スタン値の更新と確認
   _owner.StunCheck();
   // レーザー生成位置をオブジェクトサーバーに登録
   _owner.GetObjServer().RegistVector("LaserPos", GetLaserPos());
}

void LargeEnemy::StateLaser::Exit() {
   // ビーム関連エフェクトを再生していたなら停止する
   for (auto& effect : _owner.GetEfcServer().runEffects()) {
      // 列挙型のusing宣言
      using enum Effect::EffectBase::EffectType;
      switch (effect->GetEfcType()) {
      // ビーム関連エフェクト
      case BossCharge:
      case BossBeam:
         effect->StopEffect();   // エフェクトの停止
         break;
      default:
         break;
      }
   }
   // レーザー攻撃フラグを設定
   _owner._isLaser = false;
}

void LargeEnemy::StateFanGatling::Enter() {
   // 扇状ガトリング攻撃をするための回転をしていない時に経過させるフレームカウントの設定
   _fanGatlingFrameCnt = 0;
   // ガトリングの弾を打つ回数を設定
   _owner._gatlingCnt = 10;
   // 最初に扇状にガトリング攻撃をするときの向きのベクトルを取得するときベクトルを回転させる角度を設定
   _owner._fanAngle = -45.0;
   // プレイヤーへの向きを取得
   _owner._rotateDir = _owner.GetObjServer().GetVecData("PlayerPos") - _owner._position;
   // 向かせたい方向のベクトルを大きくする値を設定
   _owner._rotateEnlarge = 50.0;
   // モデルのアニメーションの設定
   _owner._modelAnimeComponent->ChangeAnime("gatoring", true);
}

void LargeEnemy::StateFanGatling::Update() {
   // 回転していない時のフレームカウントが既定のフレーム数経過し攻撃しておらず既定のフレーム数よりも大きかったら
   if (_fanGatlingFrameCnt % GatlingFrame == 0 && _owner._attack == false && _fanGatlingFrameCnt >= 100) {
      // プレイヤーへの向きのベクトルを回転させるマトリクスを作成
      Matrix44 dirRotate;
      dirRotate.RotateY(_owner._fanAngle, true);
      // ガトリング攻撃を打つ向きの設定
      _owner._rotateDir = _owner._rotateDir * dirRotate;
      // 攻撃していると設定
      _owner._attack = true;
      // 回転していると設定
      _owner._rotating = true;
      // 回転していないときのフレームカウントの更新
      ++_fanGatlingFrameCnt;
   }
   // 攻撃していたら
   if (_owner._attack) {
      // 攻撃をする方向へ回転させる
      _owner.AreaRotate(_owner._rotating);
      // 回転が終了したら
      if (!_owner._rotating) {
         // ガトリングを生成
         _owner.CreateGatling();
         // ガトリングの弾を打つ回数の更新
         --_owner._gatlingCnt;
         // 扇状にガトリング攻撃をするときの向きのベクトルを取得するときベクトルを回転させる角度を設定
         _owner._fanAngle = 10.0;
         // 鳴らすサウンドの設定 
         _owner.GetSoundComponent().Play("BossGatling",_owner._position);
         // 攻撃していないと設定
         _owner._attack = false;
      }
   }
   // 攻撃していない場合
   else {
      // 回転していないときのフレームカウントの更新
      ++_fanGatlingFrameCnt;
   }
   // ガトリングを打つ回数が0以下になったら待機状態へ
   if (_owner._gatlingCnt <= 0) {
      _owner._stateServer->GoToState("Idle");
   }
   // 当たり判定処理を行うクラスでプレイヤーがラージエネミーと当たっているか確認
   _owner._collisionComponent->LargeEnemyFromPlayer();
   // 落下オブジェクトと当たったか確認
   _owner.HitCheckFromFallObject();
   // 遠隔弱攻撃の弾と当たっているか確認
   _owner.HitCheckFromBullet();
   // スタン値の更新と確認
   _owner.StunCheck();
}

void LargeEnemy::StateStun::Enter() {
   // モデルのアニメーションの設定
   _owner._modelAnimeComponent->ChangeAnime("stan", true, 0.5);
   auto efcStan = std::make_unique<Effect::EffectBossStan>(_owner._gameMain, "BossStan");
   efcStan->position(_owner._position);
   _owner.GetEfcServer().Add(std::move(efcStan));

}

void LargeEnemy::StateStun::Update() {
   // アニメーションの繰り返した回数の取得
   auto repeated = _owner._modelAnimeComponent->repeatedCount();
   // アニメーションを一回再生したら待機状態へ
   if (repeated >= 1) {
      _owner._stateServer->GoToState("Idle");
   }
   // 当たり判定処理を行うクラスでプレイヤーがラージエネミーと当たっているか確認
   _owner._collisionComponent->LargeEnemyFromPlayer();
   // 落下オブジェクトと当たったか確認
   _owner.HitCheckFromFallObject();
   // 遠隔弱攻撃の弾と当たっているか確認
   _owner.HitCheckFromBullet();
}

void LargeEnemy::StateStun::Exit() {
   // スタン値を初期化
   _owner._stunValue = 0.0;
}

void LargeEnemy::StateConsecutiveFallObject::Enter() {
   // モデルのアニメーションの設定
   _owner._modelAnimeComponent->ChangeAnime("object_attack", false, 1.0); 
   // 落下オブジェクトを生成する数を既定の値に設定
   _fallObjectNum = ConsecutiveNum;
   auto efcPreliminary = std::make_unique<Effect::EffectPreliminaryLight>(_owner._gameMain, "PreliminaryLight");
   // ボスの向きの取得(ボスのZ座標が逆の為、反転させておく)
   auto bossDir = _owner.GetForward() * -1.0;
   // エフェクト生成位置に近いフレームの取得
   auto rootFramePos = _owner.modelAnimeComponent().GetFrameChildPosion("root", "root1");
   // 指定座標分ずらす
   auto efcPos = rootFramePos + bossDir * 300.0;
   efcPreliminary->position(efcPos);
   _owner.GetEfcServer().Add(std::move(efcPreliminary));
   _cntInit = true;
}

void LargeEnemy::StateConsecutiveFallObject::Update() {
   // この状態に入ってからの経過フレーム数の取得
   if (_owner._modelAnimeComponent->repeatedCount() > 0&& _cntInit) {
      _stateCnt = _owner.gameMain().modeServer().frameCount();
      _cntInit = false;
   }
   auto count = _owner.gameMain().modeServer().frameCount() - _stateCnt;
   if (!_cntInit && count % ConsecutiveFallObjectFrame == 0) {
      // 落下オブジェクトの生成
      CreateFallObject();
      // カメラを振動させるためにカメラの振動に使うYの位置を0.0に設定
      _owner._cameraComponent->SetVibValue(0.0);
      --_fallObjectNum;
   }
   if (_fallObjectNum <= 0) {
      _owner._stateServer->GoToState("Idle");
   }
   // 当たり判定処理を行うクラスでプレイヤーがラージエネミーと当たっているか確認
   _owner._collisionComponent->LargeEnemyFromPlayer();
   // 落下オブジェクトと当たったか確認
   _owner.HitCheckFromFallObject();
   // 遠隔弱攻撃の弾と当たっているか確認
   _owner.HitCheckFromBullet();
   // スタン値の更新と確認
   _owner.StunCheck();
}

void LargeEnemy::StateFallObject::SetPoorSpawn() {
   auto&& runObjects = _owner.GetObjServer().runObjects();
   auto activePoorEnemyCount = std::count_if(runObjects.begin(), runObjects.end(),
      [](std::unique_ptr<Object::ObjectBase>& obj) {
         // 生存状態の雑魚敵は何体いるか
         return (obj->GetObjType() == Object::ObjectBase::ObjectType::PoorEnemy) && obj->IsActive(); });
   // 生存状態の雑魚敵が一定未満なら
   if (activePoorEnemyCount <= MaxPoorEnemy) {
      // ランダムなスポーンテーブルを割り当てる
      auto random = AppFrame::Math::Utility::GetRandom(MinWave, MaxWave);
      _owner._gameMain.objFactory().SetSpawnTable("bosswave" + std::to_string(random));
   }
}

void LargeEnemy::StateMove::FootStepSound() {
   // フレームカウントの取得
   auto count = _owner.gameMain().modeServer().frameCount();
   // ボスの両前足の接地部分のフレームを取得
   auto rightFootFramePos = _owner._modelAnimeComponent->GetFrameChildPosion("root", "front_right_hand");
   auto leftFootFramePos = _owner._modelAnimeComponent->GetFrameChildPosion("root", "front_left_hand");
   // ボスの両前足の接地部分のフレームの高さを取得
   auto rightFootY = rightFootFramePos.GetY();
   auto leftFootY = leftFootFramePos.GetY();
   // 右前足の接地部分フレームは一定以上高さか
   if (rightFootY >= FootStepHeight) {
      _footRightStep = true;    // 足音が鳴るフラグをtrue
   }
   else {
      // 右足音が鳴るフラグをtrueか
      if (_footRightStep) {
         _owner.GetSoundComponent().Play("BossFootStep", _owner._position);   // 足音の再生
         _footRightStep = false;                                              // 足音が鳴るフラグをfalse
      }
   }
   // 左前足の接地部分フレームは一定以上高さか
   if (leftFootY >= FootStepHeight) {
      _footLeftStep = true;    // 足音が鳴るフラグをtrue
   }
   else {
      // 左足音が鳴るフラグをtrueか
      if (_footLeftStep) {
         _owner.GetSoundComponent().Play("BossFootStep", _owner._position);   // 足音の再生
         _footLeftStep = false;                                               // 足音が鳴るフラグをfalse
      }
   }
}

AppFrame::Math::Vector4 LargeEnemy::StateLaser::GetLaserPos() {
   // ボスの向きの取得(ボスのZ座標が逆の為、反転させておく)
   auto bossDir = _owner.GetForward() * -1.0;
   // レーザー生成位置に近いフレームの取得
   auto laserFramePos = _owner.modelAnimeComponent().GetFramePosion("pasted__laser_collision");
   // 指定座標分ずらす
   auto laserPos = laserFramePos + bossDir * LaserDiffPos;
   return laserPos;
}

void LargeEnemy::StateConsecutiveFallObject::CreateFallObject() {
   // プレイヤーの位置の取得
   auto plyPos = _owner.GetObjServer().GetVecData("PlayerPos");
   // プレイヤーの位置に一定の高さを足す
   plyPos.Add(0.0, 500.0, 0.0);
   // 落下オブジェクトの生成
   auto fallObjectBase = _owner.gameMain().objFactory().Create("FallObject");
   // 落下オブジェクトの参照型にキャスト
   auto& fallObject = dynamic_cast<Enemy::FallObject&>(*fallObjectBase);
   // プレイヤーの位置に一定の高さを足した値を位置に設定
   fallObject.position(plyPos);
   // 残留しないと設定
   fallObject.residual(false);
   // オブジェクト一括管理クラスの動的配列に追加
   _owner.GetObjServer().Add(std::move(fallObjectBase));
}
