
/*****************************************************************//**
 * \file   Player.cpp
 * \brief  プレイヤーの処理を回すクラス
 *
 * \author AHMD2000,NAOFUMISATO
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
#include "EffectHeal.h"
#include "EffectServer.h"
#include "ParamPlayer.h"
#include "ParamCollision.h"

namespace {
    constexpr auto FootStepHeight = 3.0;  //!< 走り状態時の足音発生高さ(足の甲からの位置)
    constexpr auto FootStepStart = 10;    //!< 走り状態遷移時からの足音未発生フレーム
}

using namespace FragmentValkyria::Player;

Player::Player(Game::GameMain& gameMain) : ObjectBase{ gameMain } {
   _param = std::make_unique<Param::ParamPlayer>(_gameMain, "player");
   _collParam = std::make_unique<Param::ParamCollision>(_gameMain, "collision");
}

void Player::Init(){
    _rightRotation.RotateY(90.0, true);     // ベクトルを90度回転させるマトリクスの作成
    _leftRotation.RotateY(-90.0, true);     // ベクトルを-90度回転させるマトリクスの作成
    _backRotation.RotateY(180.0, true);     // ベクトルを180度回転させるマトリクスの作成
    _isAim = false;                         // エイム中かのフラグをfalse
    _isDeadMotion = false;                  // 死亡モーション中かのフラグをfalse
    // 回転で向かせたい方向に最初に向いている方向を設定
    _rotateDir = GetForward() * _param->GetDoubleParam("rotate_rate");
}

void Player::Input(InputManager& input) {
   // カメラの入力処理を回す
   _cameraComponent->Input(input);
   // 各状態の入力処理を回す
   _stateServer->Input(input);
}

void Player::Update() {
   // 状態の更新
   _stateServer->Update();
   // ワールド行列の更新
   ComputeWorldTransform();
   // モデルの更新
   _modelAnimeComponent->Update();
   // カメラにプレイヤーの位置を設定
   _cameraComponent->SetPlyPos(_position);
   // カメラの更新
   _cameraComponent->Update();
   // オブジェクトサーバーに位置を通知
   GetObjServer().RegistVector("PlayerPos", _position);
   // オブジェクトサーバーに回転を通知
   GetObjServer().RegistVector("PlayerRot", _rotation);
   // オブジェクトサーバーにフォワードベクトルを通知
   GetObjServer().RegistVector("PlayerFor",GetForward());
   auto headFramePos = modelAnimeComponent().GetFrameChildPosion("Kamilla_kari_Reference", "Kamilla_kari_Head");
   GetObjServer().RegistVector("PlayerHeadPos", headFramePos);
   GetObjServer().RegistVector("CamTarget", _cameraComponent->GetTarget());
   // オブジェクトサーバーにカメラの位置を通知
   GetObjServer().RegistVector("CamPos", _cameraComponent->GetPos());
}

void Player::Draw() {
   // 各状態の描画処理を回す
   _stateServer->Draw();
}

void Player::ComputeWorldTransform() {
   // ワールド行列の作成
   auto [sx, sy, sz] = _scale.GetVec3();
   auto [rx, ry, rz] = _rotation.GetVec3();
   auto [px, py, pz] = _position.GetVec3();
   auto world = Matrix44();
   world.Scale(sx, sy, sz, true);
   world.RotateZ(rz, false);
   world.RotateX(rx, false);
   // デフォルトでモデルが180度回転しているので180度回転させておく
   world.RotateY(ry + AppFrame::Math::DEGREES_180, false);
   world.Transfer(px, py, pz, false);
   _worldTransform = world;
}

void Player::Move(const Vector4& moved) {
    // 移動量のベクトルの成分を分解
    auto [x, y, z] = moved.GetVec3();
    // 位置を保存
    auto position = _position;
    // x成分の移動後の位置を取得
    position = _collisionComponent->PlayerCheckStage(position, Vector4(x, y, 0.0));
    // z成分の移動後の位置を取得
    position = _collisionComponent->PlayerCheckStage(position, Vector4(0.0, y, z));
    // 位置にxとzの成分の移動後の位置を設定する
    _position = position;
}

void Player::ShootRotate() {
    // カメラから注視点への単位ベクトルをもとめる
    auto camForward = _cameraComponent->GetForward();
    // カメラから注視点への単位ベクトルの高さをなくす
    auto [x, y, z] = camForward.GetVec3();
    auto direction = Vector4(x, 0.0, z);
    // Y軸からみたモデルの向いている方向の角度を設定
    auto radius = std::atan2(x, z);
    _rotation.SetY(AppFrame::Math::Utility::RadianToDegree(radius));
}

void Player::Rotate() {
   // フォワードベクトルの取得
   auto forward = GetForward();
   forward = forward * _param->GetDoubleParam("rotate_rate");
   // フォワードベクトルと向かせたい方向のベクトルからなる三角形の面積の取得
   auto rotateVec = forward.Cross(_rotateDir);
   auto rotateValue = 0.5 * rotateVec.GetY();
   // 面積の値分回転させる(計算の順番をフォワードベクトルから向かせたい方向のベクトルにすることで計算結果の符号をそのまま利用することで最短距離の回転を行う)
   _rotation.Add(Vector4(0.0, rotateValue, 0.0));
}

void Player::HitCheckFromFallObjectRange() {
    // 当たり判定結果クラスの参照の取得
    auto report = _collisionComponent->report();
    // 当たり判定結果の確認
    if (report.id() == Collision::CollisionComponent::ReportId::HitFromObjectRange) {
        // 当たり判定結果がオブジェクトを持ち上げられる範囲にいるとなっていたらオブジェクト射撃準備状態へ
        _stateServer->GoToState("ShootReady");
        // カメラのズームをさせる
        _cameraComponent->SetZoom(true);
    }
}

void Player::HitCheckFromIdleFallObject(std::string_view state) {
    // 当たり判定結果クラスの参照の取得
    auto report = _collisionComponent->report();
    // 当たり判定結果の確認
    if (report.id() == Collision::CollisionComponent::ReportId::HitFromIdleFallObject) {
        // 待機状態の落下オブジェクトと当たっていたら
        // 落下オブジェクトからプレイヤーの位置までのベクトルの取得
        auto fromFallObject = _position - _collisionComponent->hitPos();
        // 単位化する
        fromFallObject.Normalized();
        /**
         * \brief double型の値を文字列で指定し、値管理クラスから取得する
         * \param paramName 値を指定する文字列
         * \return 文字列により指定された値
         */
        const auto _DoubleParam = [&](std::string paramName) {
           return _param->GetDoubleParam(paramName);
        };
        // 走り状態の場合
        if ("Run" == state) {
           // 落下オブジェクトからプレイヤーの位置までのベクトルに移動の速さをかけたベクトル分位置をずらす
           _position = _position + fromFallObject * _DoubleParam("run_speed");
        }
        // 歩き状態の場合
        else if ("Walk" == state) {
           // 落下オブジェクトからプレイヤーの位置までのベクトルに移動の速さをかけたベクトル分位置をずらす
           _position = _position + fromFallObject * _DoubleParam("walk_speed");
        }
        // ノックバック状態の場合
        else if ("KnockBack" == state) {
           // 落下オブジェクトからプレイヤーの位置までのベクトルに移動の速さをかけたベクトル分位置をずらす
           _position = _position + fromFallObject * 10.0;
        }
        // 当たり判定の結果を当たっていないと設定
        _collisionComponent->report().id(Collision::CollisionComponent::ReportId::None);
    }
}

void Player::HitCheckFromGatling() {
    // 当たり判定結果クラスの参照の取得
    auto report = _collisionComponent->report();
    // 当たり判定結果の確認
    if (report.id() == Collision::CollisionComponent::ReportId::HitFromGatling) {
        // ガトリングと当たっていたら
        // ガトリングの位置を取得
        auto hitPos = _collisionComponent->hitPos();
        // ノックバックさせる向きの取得
        auto knockBackVec = _position - hitPos;
        // 高さがあるので高さをなくす
        auto [x, y, z] = knockBackVec.GetVec3();
        auto knockBackDelta = Vector4(x, 0.0, z);
        // 単位化する
        knockBackDelta.Normalized();
        // ノックバック量の設定
        _knockBack = knockBackDelta * 10.0;
        // ダメージ量分ヒットポイントを減らす
        _gameMain.playerHp(_gameMain.playerHp() - _collisionComponent->damage());
        // ノックバックしていると設定
        _collisionComponent->knockBack(true);
        // 当たり判定の結果を当たっていないと設定
        _collisionComponent->report().id(Collision::CollisionComponent::ReportId::None);
        // カメラのズームをしないと設定
        _cameraComponent->SetZoom(false);
        // ノックバック状態へ
        _stateServer->GoToState("KnockBack");
    }
}

void Player::HitCheckFromFallObject() {
    // 当たり判定結果クラスの参照の取得
    auto report = _collisionComponent->report();
    // 当たり判定結果の確認
    if (report.id() == Collision::CollisionComponent::ReportId::HitFromFallObject) {
        // 落下中の落下オブジェクトと当たっていたら
        // 落下オブジェクトの位置を取得
        auto hitPos = _collisionComponent->hitPos();
        // 落下オブジェクトの位置の成分を分解
        auto [hitX, hitY, hitZ] = hitPos.GetVec3();
        // プレイヤーの位置の成分を分解
        auto [posX, posY, posZ] = _position.GetVec3();
        // もし落下オブジェクトが真上にあったら
        if (hitX == posX && posZ == hitZ) {
            // Y軸からみたモデルの回転角度の取得
            auto rotateY = _rotation.GetY();
            // Y軸からみたモデルの回転マトリクス作成
            Matrix44 mat = Matrix44();
            mat.RotateY(rotateY, true);
            // モデルが向いている方向の反対方向のベクトル生成
            Vector4 vec = Vector4(0.0, 0.0, -1.0);
            // 単位化をする
            vec.Normalized();
            // ノックバックの向きのベクトルを設定
            auto knockBackDelta = vec * mat;
            // ノックバック量のベクトルを設定
            _knockBack = knockBackDelta * 10.0;
        }
        else {
            // 落下オブジェクトが真上にない場合
            // ノックバックさせる向きの取得
            auto knockBackVec = _position - hitPos;
            // 高さをなくす
            auto [x, y, z] = knockBackVec.GetVec3();
            auto knockBackDelta = Vector4(x, 0.0, z);
            // 単位化する
            knockBackDelta.Normalized();
            // ノックバック量のベクトルを設定
            _knockBack = knockBackDelta * 10.0;
        }
        // ダメージ量分ヒットポイントを減らす
        _gameMain.playerHp(_gameMain.playerHp() - _collisionComponent->damage());
        // ノックバックしていると設定
        _collisionComponent->knockBack(true);
        // 当たり判定の結果を当たっていないと設定
        _collisionComponent->report().id(Collision::CollisionComponent::ReportId::None);
        // カメラのズームをしないと設定
        _cameraComponent->SetZoom(false);
        // ノックバック状態へ
        _stateServer->GoToState("KnockBack");
    }
}

void Player::HitCheckFromLaser() {
    // 当たり判定結果クラスの参照の取得
    auto report = _collisionComponent->report();
    // 当たり判定結果の確認
    if (report.id() == Collision::CollisionComponent::ReportId::HitFromLaser) {
        // レーザーと当たっていたら
        // レーザーの位置を取得
        auto hitPos = _collisionComponent->hitPos();
        // ノックバックさせる向きの取得
        auto knockBackVec = _position - hitPos;
        // 高さをなくす
        auto [x, y, z] = knockBackVec.GetVec3();
        auto knockBackDelta = Vector4(x, 0.0, z);
        // 単位化する
        knockBackDelta.Normalized();
        // ノックバック量のベクトルを設定
        _knockBack = knockBackDelta * 10.0;
        // ダメージ量分ヒットポイントを減らす
        _gameMain.playerHp(_gameMain.playerHp() - _collisionComponent->damage());
        // ノックバックしていると設定
        _collisionComponent->knockBack(true);
        // 当たり判定の結果を当たっていないと設定
        _collisionComponent->report().id(Collision::CollisionComponent::ReportId::None);
        // カメラのズームをしないと設定
        _cameraComponent->SetZoom(false);
        // ノックバック状態へ
        _stateServer->GoToState("KnockBack");
    }
}

void Player::HitCheckFromLargeEnemy() {
    // 当たり判定結果クラスの参照の取得
    auto report = _collisionComponent->report();
    // 当たり判定結果の確認
    if (report.id() == Collision::CollisionComponent::ReportId::HitFromLargeEnemy) {
        // ラージエネミーと当たっていたら
        // ラージエネミーの位置を取得
        auto hitPos = _collisionComponent->hitPos();
        // ノックバックさせる向きの取得
        auto knockBackVec = _position - hitPos;
        knockBackVec.Normalized();
        // ノックバック量のベクトルを設定
        _knockBack = knockBackVec * 10.0;
        // ダメージ量分ヒットポイントを減らす
        _gameMain.playerHp(_gameMain.playerHp() - _collisionComponent->damage());
        // ノックバックしていると設定
        _collisionComponent->knockBack(true);
        // カメラのズームをしないと設定
        _cameraComponent->SetZoom(false);
        // ノックバック状態へ
        _stateServer->GoToState("KnockBack");
    }
}

void Player::HitCheckFromPoorEnemy() {
   // 当たり判定結果クラスの参照の取得
   auto report = _collisionComponent->report();
   // 当たり判定結果の確認
   if (report.id() == Collision::CollisionComponent::ReportId::HitFromPoorEnemy) {
      // 雑魚敵と当たっていたら
      // 雑魚敵の位置を取得
      auto hitPos = _collisionComponent->hitPos();
      // ノックバックさせる向きの取得
      auto knockBackVec = _position - hitPos;
      knockBackVec.Normalized();
      // ノックバック量のベクトルを設定
      _knockBack = knockBackVec * 10.0;
      // ダメージ量分ヒットポイントを減らす
      _gameMain.playerHp(_gameMain.playerHp() - _collisionComponent->damage());
      // ノックバックしていると設定
      _collisionComponent->knockBack(true);
      // カメラのズームをしないと設定
      _cameraComponent->SetZoom(false);
      // ノックバック状態へ
      _stateServer->GoToState("KnockBack");
   }
}

void Player::WeakAttack() {
    // 遠隔弱攻撃の弾を生成してオブジェクトサーバーへ追加
    auto bullet = gameMain().objFactory().Create("Bullet");
    gameMain().objServer().Add(std::move(bullet));
}

void Player::StateBase::Update() {
   // 無敵時間中の場合
   if (_owner._invincibleCnt > 0) {
      // 無敵状態に入ってからのフレームカウント数の取得
      auto cnt = _owner.gameMain().modeServer().frameCount() - _owner._invincibleModeCnt;
      // 既定のフレーム数経過したら
      const auto blinkingFrame = _owner._param->GetIntParam("blinking_frame");
      if (cnt % (blinkingFrame * 2) == 0) {
         _owner._modelAnimeComponent->SetBlendModeAdd(0);
      }
      if (cnt % (blinkingFrame * 2) == blinkingFrame) {
         _owner._modelAnimeComponent->SetBlendModeReset(0);
      }
   }
   else {
      _owner._modelAnimeComponent->SetBlendModeReset(0);
   }
}

void Player::StateBase::Draw() {
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
   // プレイヤーのカプセルの一つ目の座標の設定
   auto pos1 = _owner._position + Vector4(0.0, _DoubleParam("ply_capsule_pos1"), 0.0);
   // プレイヤーのカプセルの二つ目の座標の設定
   auto pos2 = _owner._position + Vector4(0.0, _DoubleParam("ply_capsule_pos2"), 0.0);
   // プレイヤーのカプセルの半径の設定
   auto radian = static_cast<float>(_DoubleParam("ply_radius"));
   // プレイヤーのステージとの判定用の線分の始点の設定
   auto start = _owner._position + Vector4(0.0, 50.0, 0.0);
   // プレイヤーのステージとの判定用の線分の終点の設定
   auto end = _owner._position + Vector4(0.0, -10000.0, 0.0);
  // プレイヤーのカプセルの描画
   DrawCapsule3D(AppFrame::Math::ToDX(pos1), AppFrame::Math::ToDX(pos2),
      radian, 20, GetColor(0, 255, 0), GetColor(0, 0, 0), FALSE);
   // プレイヤーのステージとの判定用の線分の描画
   DrawLine3D(AppFrame::Math::ToDX(start), AppFrame::Math::ToDX(end), 
      AppFrame::Math::Utility::GetColorCode(0, 255, 255));
#endif
}

void Player::StateIdle::Enter() {
   _owner._stateCnt = _owner.gameMain().modeServer().frameCount();
   // モデルのアニメーションの設定
   _owner._modelAnimeComponent->ChangeAnime("stay", true, 
      _owner._param->GetDoubleParam("idle_animespeed"));
   // オブジェクトを持ち上げられると設定
   _owner._isLift = true;
}

void Player::StateIdle::Input(InputManager& input) {
   /**
    * \brief int型の値を文字列で指定し、値管理クラスから取得する
    * \param paramName 値を指定する文字列
    * \return 文字列により指定された値
    */
   const auto _IntParam = [&](std::string paramName) {
      return _owner._param->GetIntParam(paramName);
   };
   // デッドゾーンの取得
   auto [cameraSens, aimSens, deadZone] = _owner._gameMain.sensitivity();
   auto joypad = input.GetXJoypad();
   const auto DeadZoneRange = _IntParam("walk_dead_zone_range");
   // 左スティックが動いているか確認
   // 左スティックが右に動いていて歩きの入力範囲より大きい場合
   if (joypad.LeftStickX() > DeadZoneRange) {
      // 走り状態へ
      _owner._stateServer->GoToState("Run");
   }
   // 歩きの入力範囲以下でデッドゾーン以上だった場合
   else if (joypad.LeftStickX() >= deadZone) {
      // 歩き状態へ
      _owner._stateServer->GoToState("Walk");
   }
   // 左スティックが左に動いていて歩きの入力範囲より小さい場合
   if (joypad.LeftStickX() < -DeadZoneRange) {
      // 走り状態へ
      _owner._stateServer->GoToState("Run");
   }
   // 歩きの入力範囲以上でデッドゾーン以下だった場合
   else if (joypad.LeftStickX() <= -deadZone) {
      // 歩き状態へ
      _owner._stateServer->GoToState("Walk");
   }
   // 左スティックが上に動いていて歩きの入力範囲より大きい場合
   if (joypad.LeftStickY() > DeadZoneRange) {
      // 走り状態へ
      _owner._stateServer->GoToState("Run");
   }
   // 歩きの入力範囲以下でデッドゾーン以上だった場合
   else if (joypad.LeftStickY() >= deadZone) {
      // 歩き状態へ
      _owner._stateServer->GoToState("Walk");
   }
   // 左スティックが下に動いていて歩きの入力範囲より小さい場合
   if (joypad.LeftStickY() < -DeadZoneRange) {
      // 走り状態へ
      _owner._stateServer->GoToState("Run");
   }
   // 歩きの入力範囲以下でデッドゾーン以下だった場合
   else if (joypad.LeftStickY() <= -deadZone) {
      // 歩き状態へ
      _owner._stateServer->GoToState("Walk");
   }
   // 左トリガーが押されているか確認
   if (joypad.LeftTrigger() >= 20) {
      // 左トリガーが押されていたらオブジェクトを持ち上げる範囲にいるか確認
      _owner.HitCheckFromFallObjectRange();
   }
   // 左トリガーが押されていないときにLBボタンが押されているか確認
   else if (joypad.LBClick()) {
      // 左トリガーが押されていないときにLBボタンが押されていたら遠隔弱攻撃射撃準備状態へ
      _owner._stateServer->GoToState("WeakShootReady");
      // カメラのズームをさせる
      _owner._cameraComponent->SetZoom(true);
   }
   // Xボタンが押されていて、遠隔弱攻撃の残り弾数が遠隔弱攻撃の最大弾数未満だったら装填状態へ
   if (joypad.XClick() && _owner._gameMain.playerBullet() < _IntParam("max_bullet")) {
      _owner._stateServer->GoToState("Reload");
   }
   // Yボタンが押されていて、ポーションの数が0より大きくヒットポイントが最大ヒットポイント未満だったら回復状態へ
   if (joypad.YClick() && _owner._gameMain.playerPortion() > 0 &&
      _owner._gameMain.playerHp() < _owner._param->GetDoubleParam("max_hp")) {
      _owner._stateServer->GoToState("Recovery");
   }
}

void Player::StateIdle::Update() {
   StateBase::Update();
   _owner.Rotate();
   // 当たり判定処理を行うクラスでプレイヤーがガトリングと当たっているか確認
   _owner._collisionComponent->GatlingFromPlayer();
   // 当たり判定処理を行うクラスでプレイヤーがレーザーと当たっているか確認
   _owner._collisionComponent->PlayerFromLaser();
   // ラージエネミーと当たっているか確認
   _owner.HitCheckFromLargeEnemy();
   // 雑魚敵と当たっているか確認
   _owner.HitCheckFromPoorEnemy();
   // 落下中の落下オブジェクトと当たっているか確認
   _owner.HitCheckFromFallObject();
   // ガトリングと当たっているか確認
   _owner.HitCheckFromGatling();
   // レーザーと当たっているか確認
   _owner.HitCheckFromLaser();
   // クールタイムの更新
   --_owner._coolTime;
   // 無敵時間更新
   --_owner._invincibleCnt;
}

void Player::StateWalk::Enter() {
   _owner._stateCnt = _owner.gameMain().modeServer().frameCount();
   // オブジェクトを持ち上げられると設定
   _owner._isLift = true;
   // モデルのアニメーションの設定
   _owner._modelAnimeComponent->ChangeAnime("walk", true,
      _owner._param->GetDoubleParam("walk_animespeed"));
}

void Player::StateWalk::Input(InputManager& input) {
   /**
    * \brief int型の値を文字列で指定し、値管理クラスから取得する
    * \param paramName 値を指定する文字列
    * \return 文字列により指定された値
    */
   const auto _IntParam = [&](std::string paramName) {
      return _owner._param->GetIntParam(paramName);
   };
   /**
    * \brief double型の値を文字列で指定し、値管理クラスから取得する
    * \param paramName 値を指定する文字列
    * \return 文字列により指定された値
    */
   const auto _DoubleParam = [&](std::string paramName) {
      return _owner._param->GetDoubleParam(paramName);
   };
   // 待機状態の落下オブジェクトと当たっているか確認
   _owner.HitCheckFromIdleFallObject("Walk");
   // デッドゾーンの取得
   auto [cameraSens, aimSens, deadZone] = _owner._gameMain.sensitivity();
   // 移動しているかのフラグを作成して初期ではしていないと設定
   auto moved = false;
   // カメラから注視点への方向単位ベクトルをもとめる
   auto camForward = _owner._cameraComponent->GetForward();
   auto [x, y, z] = camForward.GetVec3();
   // 高さをなくす
   _owner._direction = Vector4(x, 0.0, z);
   // 移動量のベクトルを初期化
   _owner._moved = Vector4();
   const auto DeadZoneRange = _IntParam("walk_dead_zone_range");
   // 左スティックが右に動いているか確認
   // 左スティックが右に動いていてスティックの入力範囲がデッドゾーン以上で歩きの入力範囲以下だった場合
   if (input.GetXJoypad().LeftStickX() >= deadZone && input.GetXJoypad().LeftStickX() <= DeadZoneRange) {
      // 移動量のベクトルにカメラから注視点への方向単位ベクトルを90度回転させたベクトルを足す
      _owner._moved = _owner._moved + (_owner._direction * _owner._rightRotation);
      // 移動しているかのフラグを移動していると設定
      moved = true;
   }
   // 左スティックが右に動いていてスティックの入力範囲が歩きの入力範囲より大きい場合
   else if (input.GetXJoypad().LeftStickX() > DeadZoneRange) {
      // 移動量のベクトルにカメラから注視点への方向単位ベクトルを90度回転させたベクトルを足す
      _owner._moved = _owner._moved + (_owner._direction * _owner._rightRotation);
      // 走り状態へ
      _owner._stateServer->GoToState("Run");
      // 移動しているかのフラグを移動していると設定
      moved = true;
   }
   // 左スティックが左に動いているか確認
   // 左スティックが左に動いていてスティックの入力範囲がデッドゾーン以下で歩きの入力範囲以上だった場合
   if (input.GetXJoypad().LeftStickX() <= -deadZone && input.GetXJoypad().LeftStickX() >= -DeadZoneRange) {
      // 移動量のベクトルにカメラから注視点への方向単位ベクトルを-90度回転させたベクトルを足す
      _owner._moved = _owner._moved + (_owner._direction * _owner._leftRotation);
      // 移動しているかのフラグを移動していると設定
      moved = true;
   }
   // 左スティックが左に動いていてスティックの入力範囲が歩きの入力範囲より小さい場合
   else if (input.GetXJoypad().LeftStickX() < -DeadZoneRange) {
      // 移動量のベクトルにカメラから注視点への方向単位ベクトルを-90度回転させたベクトルを足す
      _owner._moved = _owner._moved + (_owner._direction * _owner._leftRotation);
      // 走り状態へ
      _owner._stateServer->GoToState("Run");
      // 移動しているかのフラグを移動していると設定
      moved = true;
   }
   // 左スティックが上に動いているか確認
   // 左スティックが上に動いていてスティックの入力範囲がデッドゾーン以上で歩きの入力範囲以下だった場合
   if (input.GetXJoypad().LeftStickY() >= deadZone && input.GetXJoypad().LeftStickY() <= DeadZoneRange) {
      // 移動量のベクトルにカメラから注視点への方向単位ベクトルを足す
      _owner._moved = _owner._moved + _owner._direction;
      // 移動しているかのフラグを移動していると設定
      moved = true;
   }
   // 左スティックが上に動いていてスティックの入力範囲が歩きの入力範囲より大きい場合
   else if (input.GetXJoypad().LeftStickY() > DeadZoneRange) {
      // 移動量のベクトルにカメラから注視点への方向単位ベクトルを足す
      _owner._moved = _owner._moved + _owner._direction;
      // 走り状態へ
      _owner._stateServer->GoToState("Run");
      // 移動しているかのフラグを移動していると設定
      moved = true;
   }
   // 左スティックが下に動いているか確認
   // 左スティックが下に動いていてスティックの入力範囲がデッドゾーン以下で歩きの入力範囲以上だった場合
   if (input.GetXJoypad().LeftStickY() <= -deadZone && input.GetXJoypad().LeftStickY() >= -DeadZoneRange) {
      // 移動量のベクトルにカメラから注視点への方向単位ベクトルを180度回転させたベクトルを足す
      _owner._moved = _owner._moved + (_owner._direction * _owner._backRotation);
      // 移動しているかのフラグを移動していると設定
      moved = true;
   }
   // 左スティックが下に動いていてスティックの入力範囲が歩きの入力範囲より小さい場合
   else if (input.GetXJoypad().LeftStickY() < -DeadZoneRange) {
      // 移動量のベクトルにカメラから注視点への方向単位ベクトルを180度回転させたベクトルを足す
      _owner._moved = _owner._moved + (_owner._direction * _owner._backRotation);
      // 走り状態へ
      _owner._stateServer->GoToState("Run");
      // 移動しているかのフラグを移動していると設定
      moved = true;
   }
   // 左トリガーが押されているか確認
   if (input.GetXJoypad().LeftTrigger() >= 20) {
      // 左トリガーが押されていたらオブジェクトを持ち上げる範囲にいるか確認
      _owner.HitCheckFromFallObjectRange();
   }
   // 左トリガーが押されていないときにLBボタンが押されているか確認
   else if (input.GetXJoypad().LBClick()) {
      // 左トリガーが押されていないときにLBボタンが押されていたら遠隔弱攻撃射撃準備状態へ
      _owner._stateServer->GoToState("WeakShootReady");
      // カメラのズームをさせる
      _owner._cameraComponent->SetZoom(true);
   }
   // Xボタンが押されていて、遠隔弱攻撃の残り弾数が遠隔弱攻撃の最大弾数未満だったら装填状態へ
   if (input.GetXJoypad().XClick() && _owner._gameMain.playerBullet() < _IntParam("max_bullet")) {
      _owner._stateServer->GoToState("Reload");
   }
   // Yボタンが押されていて、ポーションの数が0より大きくヒットポイントが最大ヒットポイント未満だったら回復状態へ
   if (input.GetXJoypad().YClick() && _owner._gameMain.playerPortion() > 0 && 
      _owner._gameMain.playerHp() < _DoubleParam("max_hp")) {
      _owner._stateServer->GoToState("Recovery");
   }
   // 移動しているかのフラグが移動していないとなっているか確認
   if (!moved) {
      // 移動していないとなっていたらこの状態を状態のリストから除外する
      _owner._stateServer->GoToState("Idle");
   }
   // 移動している場合
   else {
      // 移動量のベクトルを単位化する
      _owner._moved.Normalized();
      // 移動する方向が現在向いている方向と比べて真逆に近いか確認
      if (_owner.GetForward().Dot(_owner._moved) < -0.5) {
         // 移動する方向が現在向いている方向と比べて真逆に近い場合
         // 1度回転させるマトリクスを作成
         Matrix44 mat;
         mat.RotateY(1.0, true);
         // 向かせたい方向のベクトルを移動方向のベクトルを1度回転させて既定の真逆のときの大きさを掛けて設定
         _owner._rotateDir = _owner._moved * mat * 20.0;
      }
      // 真逆に近くない場合
      else {
         // 向かせたい方向のベクトルを移動方向のベクトルを回転を求めるときの既定の大きさを掛けて設定
         _owner._rotateDir = _owner._moved * _DoubleParam("rotate_rate");
      }
      // 移動量のベクトルを設定
      _owner._moved = _owner._moved * _DoubleParam("rotate_rate");
   }
}

void Player::StateWalk::Update() {
   StateBase::Update();
   _owner.Move(_owner._moved);
   _owner.Rotate();
   // 当たり判定処理を行うクラスでプレイヤーがガトリングと当たっているか確認
   _owner._collisionComponent->GatlingFromPlayer();
   // 当たり判定処理を行うクラスでプレイヤーがレーザーと当たっているか確認
   _owner._collisionComponent->PlayerFromLaser();
   // 落下中の落下オブジェクトと当たっているか確認
   _owner.HitCheckFromFallObject();
   // ラージエネミーと当たっているか確認
   _owner.HitCheckFromLargeEnemy();
   // 雑魚敵と当たっているか確認
   _owner.HitCheckFromPoorEnemy();
   // ガトリングと当たっているか確認
   _owner.HitCheckFromGatling();
   // レーザーと当たっているか確認
   _owner.HitCheckFromLaser();
   // クールタイムの更新
   --_owner._coolTime;
   // 無敵時間更新
   --_owner._invincibleCnt;
}

void Player::StateRun::Enter() {
   // オブジェクトを持ち上げられると設定
   _owner._isLift = true;
   // モデルのアニメーションの設定
   _owner._modelAnimeComponent->ChangeAnime("run_02", true,
      _owner._param->GetDoubleParam("run_animespeed"));
   // ゲームのフレームカウントの取得
   _owner._stateCnt = _owner.gameMain().modeServer().frameCount();
   auto count = _owner.gameMain().modeServer().frameCount();
   // この状態へ入った時のゲームのフレームカウントの
   _footCnt = count;
}

void Player::StateRun::Input(InputManager& input) {
   /**
    * \brief int型の値を文字列で指定し、値管理クラスから取得する
    * \param paramName 値を指定する文字列
    * \return 文字列により指定された値
    */
   const auto _IntParam = [&](std::string paramName) {
      return _owner._param->GetIntParam(paramName);
   };
   /**
    * \brief double型の値を文字列で指定し、値管理クラスから取得する
    * \param paramName 値を指定する文字列
    * \return 文字列により指定された値
    */
   const auto _DoubleParam = [&](std::string paramName) {
      return _owner._param->GetDoubleParam(paramName);
   };
   // 待機状態の落下オブジェクトと当たっているか確認
   _owner.HitCheckFromIdleFallObject("Run");
   // 左スティックの入力を数フレーム待つ
   if ((_owner.gameMain().modeServer().frameCount() - _footCnt) % _IntParam("wait_frame") == 0) {
      // デッドゾーンの取得
      auto [cameraSens, aimSens, deadZone] = _owner._gameMain.sensitivity();
      // 移動しているかのフラグを作成して初期ではしていないと設定
      auto moved = false;
      // カメラから注視点への方向単位ベクトルをもとめる
      auto camForward = _owner._cameraComponent->GetForward();
      auto [x, y, z] = camForward.GetVec3();
      // 高さをなくす
      _owner._direction = Vector4(x, 0.0, z);
      // 移動量のベクトルを初期化
      _owner._moved = Vector4();
      // 左スティックが右に動いているか確認
      // 左スティックが右に動いていて歩きの入力範囲より大きい場合
      const auto DeadZoneRange = _IntParam("walk_dead_zone_range");
      if (input.GetXJoypad().LeftStickX() > DeadZoneRange) {
         // 移動量のベクトルにカメラから注視点への方向単位ベクトルを90度回転させたベクトルを足す
         _owner._moved = _owner._moved + (_owner._direction * _owner._rightRotation);
         // 移動しているかのフラグを移動していると設定
         moved = true;
      }
      // 歩きの入力範囲内でデッドゾーン以上の場合
      else if (input.GetXJoypad().LeftStickX() >= deadZone) {
         // 移動量のベクトルにカメラから注視点への方向単位ベクトルを90度回転させたベクトルを足す
         _owner._moved = _owner._moved + (_owner._direction * _owner._rightRotation);
         // 移動しているかのフラグを移動していると設定
         moved = true;
         // 左スティックの縦軸が走りの範囲外だった場合
         if (!(input.GetXJoypad().LeftStickY() > DeadZoneRange || input.GetXJoypad().LeftStickY() < -DeadZoneRange)) {
            // 待機状態へ
            _owner._stateServer->GoToState("Idle");
         }
      }
      // 左スティックが左に動いているか確認
      // 左スティックが左に動いていて歩きの入力範囲より小さい場合
      if (input.GetXJoypad().LeftStickX() < -DeadZoneRange) {
         // 移動量のベクトルにカメラから注視点への方向単位ベクトルを-90度回転させたベクトルを足す
         _owner._moved = _owner._moved + (_owner._direction * _owner._leftRotation);
         // 移動しているかのフラグを移動していると設定
         moved = true;
      }
      // 歩きの入力範囲内でデッドゾーン以下の場合
      else if (input.GetXJoypad().LeftStickX() <= -deadZone) {
         // 移動量のベクトルにカメラから注視点への方向単位ベクトルを-90度回転させたベクトルを足す
         _owner._moved = _owner._moved + (_owner._direction * _owner._leftRotation);
         // 移動しているかのフラグを移動していると設定
         moved = true;
         // 左スティックの縦軸が走りの範囲外だった場合
         if (!(input.GetXJoypad().LeftStickY() > DeadZoneRange || input.GetXJoypad().LeftStickY() < -DeadZoneRange)) {
            // 待機状態へ
            _owner._stateServer->GoToState("Idle");
         }
      }
      // 左スティックが上に動いているか確認
      // 左スティックが上に動いていて歩きの入力範囲より大きい場合
      if (input.GetXJoypad().LeftStickY() > DeadZoneRange) {
         // 移動量のベクトルにカメラから注視点への方向単位ベクトルを足す
         _owner._moved = _owner._moved + _owner._direction;
         // 移動しているかのフラグを移動していると設定
         moved = true;
      }
      // 歩きの入力範囲内でデッドゾーン以上の場合
      else if (input.GetXJoypad().LeftStickY() >= deadZone) {
         // 移動量のベクトルにカメラから注視点への方向単位ベクトルを足す
         _owner._moved = _owner._moved + _owner._direction;
         // 移動しているかのフラグを移動していると設定
         moved = true;
         // 左スティックの横軸が走りの範囲外だった場合
         if (!(input.GetXJoypad().LeftStickX() > DeadZoneRange || input.GetXJoypad().LeftStickX() < -DeadZoneRange)) {
            // 待機状態へ
            _owner._stateServer->GoToState("Idle");
         }
      }
      // 左スティックが下に動いているか確認
      // 左スティックが下に動いていて歩きの入力範囲より小さい場合
      if (input.GetXJoypad().LeftStickY() < -DeadZoneRange) {
         // 移動量のベクトルにカメラから注視点への方向単位ベクトルを180度回転させたベクトルを足す
         _owner._moved = _owner._moved + (_owner._direction * _owner._backRotation);
         // 移動しているかのフラグを移動していると設定
         moved = true;
      }
      // 歩きの入力範囲内でデッドゾーン以下の場合
      else if (input.GetXJoypad().LeftStickY() <= -deadZone) {
         // 移動量のベクトルにカメラから注視点への方向単位ベクトルを180度回転させたベクトルを足す
         _owner._moved = _owner._moved + (_owner._direction * _owner._backRotation);
         // 移動しているかのフラグを移動していると設定
         moved = true;
         // 左スティックの横軸が走りの範囲外だった場合
         if (!(input.GetXJoypad().LeftStickX() > DeadZoneRange || input.GetXJoypad().LeftStickX() < -DeadZoneRange)) {
            // 待機状態へ
            _owner._stateServer->GoToState("Idle");
         }
      }
      // 移動しているかのフラグが移動していないとなっているか確認
      if (!moved) {
         // 移動していないとなっていたらこの状態を状態のリストから除外する
         _owner._stateServer->GoToState("Idle");
      }
      // 移動している場合
      else {
         // 移動量のベクトルを単位化する
         _owner._moved.Normalized();
         // 移動する方向が現在向いている方向と比べて真逆に近いか確認
         if (_owner.GetForward().Dot(_owner._moved) < -0.5) {
            // 移動する方向が現在向いている方向と比べて真逆に近い場合
            // 1度回転させるマトリクスを作成
            Matrix44 mat;
            mat.RotateY(1.0, true);
            // 向かせたい方向のベクトルを移動方向のベクトルを1度回転させて既定の真逆のときの大きさを掛けて設定
            _owner._rotateDir = _owner._moved * mat * 20.0;
         }
         // 真逆に近くない場合
         else {
            // 向かせたい方向のベクトルを移動方向のベクトルを回転を求めるときの既定の大きさを掛けて設定
            _owner._rotateDir = _owner._moved * _DoubleParam("rotate_rate");
         }
         // 移動量のベクトルを設定
         _owner._moved = _owner._moved * _DoubleParam("run_speed");
      }
   }
   // 左トリガーが押されているか確認
   if (input.GetXJoypad().LeftTrigger() >= 20) {
      // 左トリガーが押されていたらオブジェクトを持ち上げる範囲にいるか確認
      _owner.HitCheckFromFallObjectRange();
   }
   // 左トリガーが押されていないときにLBボタンが押されているか確認
   else if (input.GetXJoypad().LBClick()) {
      // 左トリガーが押されていないときにLBボタンが押されていたら遠隔弱攻撃射撃準備状態へ
      _owner._stateServer->GoToState("WeakShootReady");
      // カメラのズームをさせる
      _owner._cameraComponent->SetZoom(true);
   }
   // Xボタンが押されていて、遠隔弱攻撃の残り弾数が遠隔弱攻撃の最大弾数未満だったら装填状態へ
   if (input.GetXJoypad().XClick() && _owner._gameMain.playerBullet() < _IntParam("max_bullet")) {
      _owner._stateServer->GoToState("Reload");
   }
   // Yボタンが押されていて、ポーションの数が0より大きくヒットポイントが最大ヒットポイント未満だったら回復状態へ
   if (input.GetXJoypad().YClick() && _owner._gameMain.playerPortion() > 0 && 
      _owner._gameMain.playerHp() < _DoubleParam("max_hp")) {
      _owner._stateServer->GoToState("Recovery");
   }
}

void Player::StateRun::Update() {
   StateBase::Update();
   FootStepSound();
   _owner.Move(_owner._moved);
   _owner.Rotate();
   // 当たり判定処理を行うクラスでプレイヤーがガトリングと当たっているか確認
   _owner._collisionComponent->GatlingFromPlayer();
   // 当たり判定処理を行うクラスでプレイヤーがレーザーと当たっているか確認
   _owner._collisionComponent->PlayerFromLaser();
   // 落下中の落下オブジェクトと当たっているか確認
   _owner.HitCheckFromFallObject();
   // ラージエネミーと当たっているか確認
   _owner.HitCheckFromLargeEnemy();
   // 雑魚敵と当たっているか確認
   _owner.HitCheckFromPoorEnemy();
   // ガトリングと当たっているか確認
   _owner.HitCheckFromGatling();
   // レーザーと当たっているか確認
   _owner.HitCheckFromLaser();
   // クールタイムの更新
   --_owner._coolTime;
   // 無敵時間更新
   --_owner._invincibleCnt;
}

void Player::StateShootReady::Enter() {
   _owner._stateCnt = _owner.gameMain().modeServer().frameCount();
   // モデルのアニメーションの設定
   _owner._modelAnimeComponent->ChangeAnime("H_attack_pose_move", true,
      _owner._param->GetDoubleParam("shootready_animespeed"));
   // 鳴らすサウンドの設定
   _owner.GetSoundComponent().Play("PlayerShootReady");
   _owner.GetSoundComponent().Play("PlayerObjectUpVoice");
   // アニメーションは変えていないと設定
   _changeAnim = false;
   // エイム中と設定
   _owner._isAim = true;
   // オブジェクトを持ち上げられないと設定
   _owner._isLift = false;
   // オブジェクトを打てないと設定
   _owner._objectShoot = false;
}

void Player::StateShootReady::Input(InputManager& input) {
   // RBボタンが押されてオブジェクトを打てる場合待機状態へ
   if (input.GetXJoypad().RBClick() && _owner._objectShoot) {
      // 待機状態へ
      _owner._stateServer->GoToState("Idle");
      // 鳴らすサウンドの設定
      _owner.GetSoundComponent().Play("PlayerShoot");
      _owner.GetSoundComponent().Play("PlayerObjectShootVoice");
      // モデルのアニメーションの設定
      _owner._modelAnimeComponent->ChangeAnime("H_attack_attack",false, 
         _owner._param->GetDoubleParam("shoot_animespeed"));
      // カメラのズームをしないと設定
      _owner._cameraComponent->SetZoom(false);
   }
}

void Player::StateShootReady::Update() {
   StateBase::Update();
   // 最初のアニメーションを1回再生していてアニメーションを変えていない場合次のアニメーションを設定
   if (_owner._modelAnimeComponent->repeatedCount() >= 1 && !_changeAnim) {
      // モデルのアニメーションの設定
      _owner._modelAnimeComponent->ChangeAnime("H_attack_pose_loop", true,
         _owner._param->GetDoubleParam("shootready_animespeed"));
      // アニメーションを変えたと設定
      _changeAnim = true;
   }
   _owner.ShootRotate();
   // 当たり判定処理を行うクラスでプレイヤーがガトリングと当たっているか確認
   _owner._collisionComponent->GatlingFromPlayer();
   // 当たり判定処理を行うクラスでプレイヤーがレーザーと当たっているか確認
   _owner._collisionComponent->PlayerFromLaser();
   // ラージエネミーと当たっているか確認
   _owner.HitCheckFromLargeEnemy();
   // 雑魚敵と当たっているか確認
   _owner.HitCheckFromPoorEnemy();
   // 落下中の落下オブジェクトと当たっているか確認
   _owner.HitCheckFromFallObject();
   // ガトリングと当たっているか確認
   _owner.HitCheckFromGatling();
   // レーザーと当たっているか確認
   _owner.HitCheckFromLaser();
   // クールタイムの更新
   --_owner._coolTime;
   // 無敵時間更新
   --_owner._invincibleCnt;
}

void Player::StateShootReady::Exit() {
   // 当たり判定の結果を当たっていないと設定
   _owner._collisionComponent->report().id(Collision::CollisionComponent::ReportId::None);
   // エイム中じゃないと設定
   _owner._isAim = false;
}

void Player::StateKnockBack::Enter() {
   _owner._stateCnt = _owner.gameMain().modeServer().frameCount();
   // モデルのアニメーションの設定
   _owner.modelAnimeComponent().ChangeAnime("damaged", false,
      _owner._param->GetDoubleParam("knockback_animespeed"));
   _owner.GetSoundComponent().Play("PlayerDamageVoice");
   // ノックバックする時間の設定
   _owner._freezeTime = 30;
   // オブジェクトを持ち上げられないと設定
   _owner._isLift = false;
}

void Player::StateKnockBack::Update() {
   // 待機状態の落下オブジェクトと当たっているか確認
   _owner.HitCheckFromIdleFallObject("KnockBack");
   // ノックバックする時間中か確認
   if (_owner._freezeTime > 0) {
      // ノックバック処理
      _owner.Move(_owner._knockBack);
      // ノックバックの向きの反対の向きへの角度を取得し、向きに設定する
      auto [x, y, z] = _owner._knockBack.GetVec3();
      auto radius = std::atan2(-x, -z);
      _owner._rotation.SetY(AppFrame::Math::Utility::RadianToDegree(radius));
      // ノックバック時間の更新
      --_owner._freezeTime;
      return;
   }
   // ヒットポイントが0以下だった場合死亡状態へ
   if (_owner._gameMain.playerHp() <= 0) {
      _owner._stateServer->GoToState("Die");
   }
   // ヒットポイントが0以下じゃなかった場合
   else {
      // 無敵時間の設定
      _owner._invincibleCnt = _owner._param->GetIntParam("invincible_frame");
      // 無敵状態に入った時のモードサーバーのフレームカウント数の設定
      _owner._invincibleModeCnt = _owner.gameMain().modeServer().frameCount();
      // ノックバックしていないと設定
      _owner.collisionComponent().knockBack(false);
      // 当たり判定の結果を当たっていないと設定
      _owner.collisionComponent().report().id(Collision::CollisionComponent::ReportId::None);
      // 待機状態へ
      _owner._stateServer->GoToState("Idle");
   }
   // クールタイムの更新
   --_owner._coolTime;
}

void Player::StateDie::Enter() {
   // モデルのアニメーションの設定
   _owner.modelAnimeComponent().ChangeAnime("dawn", false);
   // 死亡状態になってからゲームオーバーまでのフレーム数を設定
   _timeOver = 60 * 2;
   // 死亡モーション中と設定
   _owner._isDeadMotion = true;
   // オブジェクトを持ち上げられないと設定
   _owner._isLift = false;
}

void Player::StateDie::Update() {
   // ゲームオーバーまでのフレーム数が残っていたら減らす
   if (_timeOver > 0) {
      --_timeOver;
   }
   // ゲームオーバーまでのフレーム数がたった場合
   else {
      auto& soundComponent = _owner.GetSoundComponent();
      soundComponent.Stop("PoorBattleBgm");
      soundComponent.Stop("BossBattleBgm");
      soundComponent.Stop("TutorialBgm");
      // モードサーバーにゲームオーバーモードを挿入
      _owner.gameMain().modeServer().PushBack("MissionFailed");
   }
}

void Player::StateWeakShootReady::Enter() {
   _owner._stateCnt = _owner.gameMain().modeServer().frameCount();
   // モデルのアニメーションの設定
   _owner._modelAnimeComponent->ChangeAnime("L_attack_pose_loop", true);
   // 鳴らすサウンドの設定
   _owner.GetSoundComponent().Play("WeakShootReady");
   // エイム中と設定
   _owner._isAim = true;
   // オブジェクトを持ち上げられないと設定
   _owner._isLift = false;
}

void Player::StateWeakShootReady::Input(InputManager& input) {
   // RBボタンが押されていてクールタイムがなく、遠隔弱攻撃の残り弾数があるか確認
   if (input.GetXJoypad().RBClick() && _owner._coolTime <= 0 && _owner._gameMain.playerBullet() > 0) {
      // RBボタンが押されていてクールタイムがなく、遠隔弱攻撃の残り弾数があった場合
      // 遠隔弱攻撃処理
      _owner.WeakAttack();
      // モデルのアニメーションの設定
      _owner._modelAnimeComponent->ChangeAnime("L_attack_attack", false, 
         _owner._param->GetDoubleParam("shoot_animespeed"));
      // 鳴らすサウンドの設定
      _owner.GetSoundComponent().Play("PlayerShoot");
      // 遠隔弱攻撃の残り弾数を減らす
      _owner._gameMain.playerBullet(_owner._gameMain.playerBullet() - 1);
      // クールタイムの設定
      _owner._coolTime = _owner._param->GetIntParam("cooltime");
   }
   // LBボタンが押されたら待機状態へ
   if (input.GetXJoypad().LBClick()) {
      _owner._stateServer->GoToState("Idle");
      // カメラのズームをしないと設定
      _owner._cameraComponent->SetZoom(false);
   }
}

void Player::StateWeakShootReady::Update() {
   StateBase::Update();
   _owner.ShootRotate();
   // 当たり判定処理を行うクラスでプレイヤーがガトリングと当たっているか確認
   _owner._collisionComponent->GatlingFromPlayer();
   // 当たり判定処理を行うクラスでプレイヤーがレーザーと当たっているか確認
   _owner._collisionComponent->PlayerFromLaser();
   // ラージエネミーと当たっているか確認
   _owner.HitCheckFromLargeEnemy();
   // 雑魚敵と当たっているか確認
   _owner.HitCheckFromPoorEnemy();
   // 落下中の落下オブジェクトと当たっているか確認
   _owner.HitCheckFromFallObject();
   // ガトリングと当たっているか確認
   _owner.HitCheckFromGatling();
   // レーザーと当たっているか確認
   _owner.HitCheckFromLaser();
   // クールタイムの更新
   --_owner._coolTime;
   // 無敵時間更新
   --_owner._invincibleCnt;
}

void Player::StateWeakShootReady::Exit() {
   // エイム中じゃないと設定
   _owner._isAim = false;
}

void Player::StateReload::Enter() {
   _owner._stateCnt = _owner.gameMain().modeServer().frameCount();
   // モデルのアニメーションの設定
   _owner._modelAnimeComponent->ChangeAnime("reload", true,0.5);
   // リロード状態のカウントを0に設定
   _reloadCnt = 0;
   // オブジェクトを持ち上げられないと設定
   _owner._isLift = false;
   _owner.GetSoundComponent().Play("Reload");
}

void Player::StateReload::Update() {
   StateBase::Update();
   // リロード状態のカウントが既定の値よりも大きかったら待機状態へ
   if (_reloadCnt > 60 * 2) {
      _owner._gameMain.playerBullet(5);
      _owner._stateServer->GoToState("Idle");
   }
   // 当たり判定処理を行うクラスでプレイヤーがガトリングと当たっているか確認
   _owner._collisionComponent->GatlingFromPlayer();
   // 当たり判定処理を行うクラスでプレイヤーがレーザーと当たっているか確認
   _owner._collisionComponent->PlayerFromLaser();
   // ラージエネミーと当たっているか確認
   _owner.HitCheckFromLargeEnemy();
   // 雑魚敵と当たっているか確認
   _owner.HitCheckFromPoorEnemy();
   // 落下中の落下オブジェクトと当たっているか確認
   _owner.HitCheckFromFallObject();
   // ガトリングと当たっているか確認
   _owner.HitCheckFromGatling();
   // レーザーと当たっているか確認
   _owner.HitCheckFromLaser();
   // リロード状態のカウントの更新
   ++_reloadCnt;
   // クールタイムの更新
   --_owner._coolTime;
   // 無敵時間更新
   --_owner._invincibleCnt;
}

void Player::StateRecovery::Enter() {
   _owner._stateCnt = _owner.gameMain().modeServer().frameCount();
   // モデルのアニメーションの設定
   _owner._modelAnimeComponent->ChangeAnime("heal", false);
   // 回復状態のカウントを0に設定
   _recoveryCnt = 0;
   auto efcHeal = std::make_unique<Effect::EffectHeal>(_owner._gameMain, "Heal");
   auto hipsFramePos = _owner._modelAnimeComponent->GetFrameChildPosion("Kamilla_kari_Reference", "Kamilla_kari_Hips");
   efcHeal->position(hipsFramePos);
   _owner.GetEfcServer().Add(std::move(efcHeal));
   // オブジェクトを持ち上げられないと設定
   _owner._isLift = false;
}

void Player::StateRecovery::Update() {
   /**
    * \brief double型の値を文字列で指定し、値管理クラスから取得する
    * \param paramName 値を指定する文字列
    * \return 文字列により指定された値
    */
   const auto _DoubleParam = [&](std::string paramName) {
      return _owner._param->GetDoubleParam(paramName);
   };
   // 状態の基底クラスの更新処理
   StateBase::Update();
   // 回復状態のカウントが既定の値よりも大きいか確認
   if (_recoveryCnt > 60 * 1) {
      const auto MaxHp = _DoubleParam("max_hp");
      // 回復状態のカウントが既定の値よりも大きい場合
      // 回復量の設定
      auto recovery = MaxHp * _DoubleParam("recovery_rate");
      // ヒットポイントを回復量分増やす
      _owner._gameMain.playerHp(_owner._gameMain.playerHp() + recovery);
      // ヒットポイントが最大値よりも大きくなった場合ヒットポイントを最大値にする
      if (_owner._gameMain.playerHp() >= MaxHp) {
         _owner._gameMain.playerHp(MaxHp);
      }
      // ポーションの数を一つ減らす
      _owner._gameMain.playerPortion(_owner._gameMain.playerPortion() - 1);
      // 待機状態へ
      _owner._stateServer->GoToState("Idle");
   }
   // 当たり判定処理を行うクラスでプレイヤーがガトリングと当たっているか確認
   _owner._collisionComponent->GatlingFromPlayer();
   // 当たり判定処理を行うクラスでプレイヤーがレーザーと当たっているか確認
   _owner._collisionComponent->PlayerFromLaser();
   // ラージエネミーと当たっているか確認
   _owner.HitCheckFromLargeEnemy();
   // 雑魚敵と当たっているか確認
   _owner.HitCheckFromPoorEnemy();
   // 落下中の落下オブジェクトと当たっているか確認
   _owner.HitCheckFromFallObject();
   // ガトリングと当たっているか確認
   _owner.HitCheckFromGatling();
   // レーザーと当たっているか確認
   _owner.HitCheckFromLaser();
   // 回復状態のカウントの更新
   ++_recoveryCnt;
   // クールタイムの更新
   --_owner._coolTime;
   // 無敵時間更新
   --_owner._invincibleCnt;
}

void Player::StateRun::FootStepSound() {
   // フレームカウントの取得
   auto count = _owner.gameMain().modeServer().frameCount();
   // カウントが一定以上経過しているか
   if (count - _footCnt >= FootStepStart) {
      // プレイヤーの両踵フレームの取得
      auto rightFootFramePos = _owner.modelAnimeComponent().GetFrameChildPosion("Kamilla_kari_Reference", "Kamilla_kari_RightToeBase");
      auto leftFootFramePos = _owner.modelAnimeComponent().GetFrameChildPosion("Kamilla_kari_Reference", "Kamilla_kari_LeftToeBase");
      // プレイヤーの両踵フレームの高さを取得
      auto rightFootY = rightFootFramePos.GetY();
      auto leftFootY = leftFootFramePos.GetY();
      // プレイヤーの右踵フレームは一定以上の高さか
      if (rightFootY >= FootStepHeight) {
         _footRightStep = true;    // 足音が鳴るフラグをtrue
      }
      else {
         // 右足音が鳴るフラグがtrueか
         if (_footRightStep) {
            _owner.GetSoundComponent().Play("PlayerRightFootStep");  // 足音の再生
            _footRightStep = false;                                  // 足音が鳴るフラグをfalse
         }
      }
      // プレイヤーの左踵フレームは一定以上の高さか
      if (leftFootY >= FootStepHeight) {
         _footLeftStep = true;    // 足音が鳴るフラグをtrue
      }
      else {
         // 左足音が鳴るフラグがtrueか
         if (_footLeftStep) {
            auto& soundServer = _owner.GetSoundComponent();
            soundServer.Play("PlayerLeftFootStep");  // 足音の再生
            _footLeftStep = false;                   // 足音が鳴るフラグをfalse
         }
      }
   }
}
