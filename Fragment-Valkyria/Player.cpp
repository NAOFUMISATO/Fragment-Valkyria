
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

namespace {
    auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("player",{
       "idle_animespeed","walk_animespeed","run_animespeed","shootready_animespeed","shoot_animespeed",
       "move_speed", "capsule_pos1", "capsule_pos2", "capsule_radius",
        "rocovery_rate", "max_hp"});

    const double IdleAnimeSpeed = paramMap["idle_animespeed"];                //!< 待機状態のアニメーションスピード
    const double WalkAnimeSpeed = paramMap["walk_animespeed"];                //!< 歩き状態のアニメーションスピード
    const double RunAnimeSpeed = paramMap["run_animespeed"];                  //!< 走り状態のアニメーションスピード
    const double ShootReadyAnimeSpeed = paramMap["shootready_animespeed"];    //!< 射撃準備中のアニメーションスピード
    const double ShootAnimeSpeed = paramMap["shoot_animespeed"];              //!< 射撃のアニメーションスピード
    const double MoveSpeed = paramMap["move_speed"];                          //!< 移動の速さ
    const double CapsulePos1 = paramMap["capsule_pos1"];                      //!< カプセルの一つ目の座標までの位置からの高さ
    const double CapsulePos2 = paramMap["capsule_pos2"];                      //!< カプセルの二つ目の座標までの位置からの高さ
    const double CapsuleRadius = paramMap["capsule_radius"];                  //!< カプセルの半径
    const double MaxHp = paramMap["max_hp"];                                  //!< ヒットポイントの最大値
    const double RecoveryRate = paramMap["rocovery_rate"];                     //!< ヒットポイントの最大値からの回復する割合

    constexpr auto FootStepHeight = 3.0;                                      //!< 走り状態時の足音発生高さ(足の甲からの位置)
    constexpr auto FootStepStart = 10;                                        //!< 走り状態遷移時からの足音未発生フレーム
}

using namespace FragmentValkyria::Player;

Player::Player(Game::GameMain& gameMain) : ObjectBase{ gameMain } {
}

void Player::Init(){
    // ベクトルを90度回転させるマトリクスの作成
    _rightRotation.RotateY(90.0, true);
    // ベクトルを-90度回転させるマトリクスの作成
    _leftRotation.RotateY(-90.0, true);
    // ベクトルを180度回転させるマトリクスの作成
    _backRotation.RotateY(180.0, true);
    _isAim = false;        // エイム中かのフラグをfalse
    _isDeadMotion = false; // 死亡モーション中かのフラグをfalse
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
   // カメラの更新
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
   GetObjServer().RegistDouble("PlayerPortionStock", static_cast<double>(_portionStock));
}

void Player::Draw() {
    // 各状態の描画処理
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
    // xとzの移動後の成分を位置に設定する
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

void Player::HitCheckFromFallObjectRange() {
    // 当たり判定結果クラスの参照の取得
    auto report = _collisionComponent->report();
    // 当たり判定結果の確認
    if (report.id() == Collision::CollisionComponent::ReportId::HitFromObjectRange) {
        // 当たり判定結果がオブジェクトを持ち上げられる範囲にいるとなっていたら射撃準備状態へ
        _stateServer->PushBack("ShootReady");
        // カメラのズームをさせる
        _cameraComponent->SetZoom(true);
    }
}

void Player::HitCheckFromIdleFallObject() {
    // 当たり判定結果クラスの参照の取得
    auto report = _collisionComponent->report();
    // 当たり判定結果の確認
    if (report.id() == Collision::CollisionComponent::ReportId::HitFromIdleFallObject) {
        // 待機状態の落下オブジェクトとあたっていたら
        // 当たったポリゴンの法線を取得
        auto normal = _collisionComponent->hitPos();

        //// 移動量のベクトルの取得
        //auto moveVec = _moved * MoveSpeed;
        //// 移動量のベクトルと法線の外積をとる
        //auto slideVec = moveVec.Cross(normal);
        //// 法線と移動量のベクトルと法線の外積の結果との外積の結果からスライドさせるべき方向のベクトルを取得
        //slideVec = normal.Cross(slideVec);
   
        // 法線に移動の速さをかけたベクトル分位置をずらす
        _position = _position + normal * MoveSpeed;
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
        knockBackDelta.Normalized();
        // ノックバック量の設定
        _knockBack = knockBackDelta * 10.0;
        // ダメージ量分ヒットポイントを減らす
        _hp -= _collisionComponent->damage();
        // ノックバックしていると設定
        _collisionComponent->knockBack(true);
        // 当たり判定の結果を当たっていないと設定
        _collisionComponent->report().id(Collision::CollisionComponent::ReportId::None);
        // カメラのズームをしないと設定
        _cameraComponent->SetZoom(false);
        // ノックバック状態へ
        _stateServer->PushBack("KnockBack");
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
            vec.Normalized();
            auto knockBackDelta = vec * mat;
            // ノックバック量のベクトルを設定
            _knockBack = knockBackDelta * 10.0;
        }
        else {
            // 落下オブジェクトが真上にない場合
            // ノックバック量のベクトルを設定
            auto knockBackVec = _position - hitPos;
            // 高さをなくす
            auto [x, y, z] = knockBackVec.GetVec3();
            auto knockBackDelta = Vector4(x, 0.0, z);
            knockBackDelta.Normalized();
            _knockBack = knockBackDelta * 10.0;
        }
        // ダメージ量分ヒットポイントを減らす
        _hp -= _collisionComponent->damage();
        // ノックバックしていると設定
        _collisionComponent->knockBack(true);
        // 当たり判定の結果を当たっていないと設定
        _collisionComponent->report().id(Collision::CollisionComponent::ReportId::None);
        // カメラのズームをしないと設定
        _cameraComponent->SetZoom(false);
        // ノックバック状態へ
        _stateServer->PushBack("KnockBack");
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
        // ノックバック量のベクトルを設定
        auto knockBackVec = _position - hitPos;
        // 高さをなくす
        auto [x, y, z] = knockBackVec.GetVec3();
        auto knockBackDelta = Vector4(x, 0.0, z);
        knockBackDelta.Normalized();
        _knockBack = knockBackDelta * 20.0;
        // ダメージ量分ヒットポイントを減らす
        _hp -= _collisionComponent->damage();
        // ノックバックしていると設定
        _collisionComponent->knockBack(true);
        // 当たり判定の結果を当たっていないと設定
        _collisionComponent->report().id(Collision::CollisionComponent::ReportId::None);
        // カメラのズームをしないと設定
        _cameraComponent->SetZoom(false);
        // ノックバック状態へ
        _stateServer->PushBack("KnockBack");
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
        // ノックバック量のベクトルを設定
        auto knockBackVec = _position - hitPos;
        knockBackVec.Normalized();
        _knockBack = knockBackVec * 10.0;
        // ダメージ量分ヒットポイントを減らす
        _hp -= _collisionComponent->damage();
        // ノックバックしていると設定
        _collisionComponent->knockBack(true);
        // カメラのズームをしないと設定
        _cameraComponent->SetZoom(false);
        // ノックバック状態へ
        _stateServer->PushBack("KnockBack");
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
      // ノックバック量のベクトルを設定
      auto knockBackVec = _position - hitPos;
      knockBackVec.Normalized();
      _knockBack = knockBackVec * 10.0;
      // ダメージ量分ヒットポイントを減らす
      _hp -= _collisionComponent->damage();
      // ノックバックしていると設定
      _collisionComponent->knockBack(true);
      // カメラのズームをしないと設定
      _cameraComponent->SetZoom(false);
      // ノックバック状態へ
      _stateServer->PushBack("KnockBack");
   }
}

void Player::WeakAttack() {
    // 遠隔弱攻撃の弾を生成してオブジェクトサーバーへ追加
    auto bullet = gameMain().objFactory().Create("Bullet");
    gameMain().objServer().Add(std::move(bullet));
}

void Player::StateBase::Draw() {
   _owner._modelAnimeComponent->Draw();
   if (_owner._invincibleCnt > 0) {
       MV1SetOpacityRate(_owner._modelAnimeComponent->modelHandle(), 0.1f);
   }
   else {
       MV1SetOpacityRate(_owner._modelAnimeComponent->modelHandle(), 1.0f);
   }
#ifdef _DEBUG
   auto pos1 = _owner._position + Vector4(0.0, CapsulePos1, 0.0);
   auto pos2 = _owner._position + Vector4(0.0, CapsulePos2, 0.0);
   auto radian = static_cast<float>(CapsuleRadius);

   auto start = _owner._position + Vector4(0.0, 50.0, 0.0);
   auto end = _owner._position + Vector4(0.0, -10000.0, 0.0);
  
   DrawCapsule3D(AppFrame::Math::ToDX(pos1), AppFrame::Math::ToDX(pos2), radian, 20, GetColor(0, 255, 0), GetColor(0, 0, 0), FALSE);
   DrawLine3D(AppFrame::Math::ToDX(start), AppFrame::Math::ToDX(end), AppFrame::Math::Utility::GetColorCode(0, 255, 255));
#endif

}
/// 待機
void Player::StateIdle::Enter() {
   /*_owner._forwardSpeed = 0.0;*/
   _owner._modelAnimeComponent->ChangeAnime("stay", true, IdleAnimeSpeed);
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
   if (input.GetXJoypad().YClick() && _owner._portionStock > 0 && _owner._hp < 100.0) {
       --_owner._portionStock;
       _owner._stateServer->GoToState("Recovery");
   }
}
void Player::StateIdle::Update() {
    _owner._collisionComponent->ObjectRangeFromPlayer();
    _owner.HitCheckFromIdleFallObject();
    
    _owner._collisionComponent->GatlingFromPlayer();
    _owner._collisionComponent->PlayerFromLaser();
    _owner.HitCheckFromLargeEnemy();
    _owner.HitCheckFromPoorEnemy();
    _owner.HitCheckFromFallObject();
    _owner.HitCheckFromGatling();
    _owner.HitCheckFromLaser();
    
    //無敵時間更新
    --_owner._invincibleCnt;
}

/// 走り
void Player::StateRun::Enter() {
   _owner._modelAnimeComponent->ChangeAnime("run", true, RunAnimeSpeed);
   auto count = _owner.gameMain().modeServer().frameCount();
   _footCnt = count;
}
void Player::StateRun::Input(InputManager& input) {
    // 待機状態の落下オブジェクトと当たっているか確認
    _owner.HitCheckFromIdleFallObject();
    // 移動しているかのフラグを作成して初期ではしていないと設定
    auto moved = false;
    // カメラから注視点への方向単位ベクトルをもとめる
    auto camForward = _owner._cameraComponent->GetForward();
    auto [x, y, z] = camForward.GetVec3();
    // 高さをなくす
    _owner._direction = Vector4(x, 0.0, z);
    // 移動量のベクトルを初期化
    _owner._moved = Vector4();
   if (input.GetKeyboard().SpaceClick()) {
      _owner._stateServer->PushBack("Attack");
      return;
   }
   
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
   if (input.GetXJoypad().YClick() && _owner._portionStock > 0 && _owner._hp < 100.0) {
       --_owner._portionStock;
       _owner._stateServer->GoToState("Recovery");
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
   _owner.HitCheckFromPoorEnemy();
   _owner.HitCheckFromGatling();
   _owner.HitCheckFromLaser();

   // 無敵時間更新
   --_owner._invincibleCnt;
}

/// 攻撃
void Player::StateAttack::Enter() {
   _owner._modelAnimeComponent->ChangeAnime("L_attack_pose_loop",false, ShootAnimeSpeed);
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
    _owner._modelAnimeComponent->ChangeAnime("H_attack_pose_loop", true, ShootReadyAnimeSpeed);
    _owner.GetSoundComponent().Play("PlayerShootReady");
    _owner._isAim = true;
}

void Player::StateShootReady::Input(InputManager& input) {
    if (input.GetXJoypad().RBClick()) {
       _owner.GetSoundComponent().Play("PlayerShoot");
       _owner._modelAnimeComponent->ChangeAnime("H_attack_attack", false, ShootAnimeSpeed);
        _owner._stateServer->PopBack();
        _owner._cameraComponent->SetZoom(false);
    }
}

void Player::StateShootReady::Update() {
    _owner.ShootRotate();
   
    _owner._collisionComponent->GatlingFromPlayer();
    _owner._collisionComponent->PlayerFromLaser();
    _owner.HitCheckFromLargeEnemy();
    _owner.HitCheckFromPoorEnemy();
    _owner.HitCheckFromFallObject();
    _owner.HitCheckFromGatling();
    _owner.HitCheckFromLaser();

    // 無敵時間更新
    --_owner._invincibleCnt;
   
}

void Player::StateShootReady::Draw() {
   _owner._modelAnimeComponent->Draw();
}

void Player::StateShootReady::Exit() {
   _owner._isAim = false;
}

void Player::StateKnockBack::Enter() {
   _owner.modelAnimeComponent().ChangeAnime("damaged", false,1.2);
    _owner._freezeTime = 30;

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
    _owner.modelAnimeComponent().ChangeAnime("dawn", false);
    _timeOver = 60 * 2;
    _owner._isDeadMotion = true;
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
    _owner._modelAnimeComponent->ChangeAnime("L_attack_pose_loop", true);
    _owner.GetSoundComponent().Play("PlayerShootReady");
    _coolTime = 0;
    _owner._isAim = true;
}

void Player::StateWeakShootReady::Input(InputManager& input) {
    if (input.GetXJoypad().RBClick() && _coolTime <= 0 && _owner._bulletStock > 0) {
        _owner.WeakAttack();
        _owner._modelAnimeComponent->ChangeAnime("L_attack_attack", false, ShootAnimeSpeed);
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
    // 無敵時間更新
    --_owner._invincibleCnt;
}

void Player::StateWeakShootReady::Exit() {
   _owner._isAim = false;
}

void Player::StateReload::Enter() {
    _owner._modelAnimeComponent->ChangeAnime("stealth_sit", true,0.8);
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

    // 無敵時間更新
    --_owner._invincibleCnt;
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

void Player::StateRecovery::Enter() {
    _owner._modelAnimeComponent->ChangeAnime("heal", false);
    _recoveryCnt = 0;
}

void Player::StateRecovery::Update() {
    if (_recoveryCnt > 60 * 2) {
        auto recovery = MaxHp * RecoveryRate;
        _owner._hp += recovery;
        if (_owner._hp >= 100.0) {
            _owner._hp = 100.0;
        }
        _owner._stateServer->GoToState("Idle");
    }

    ++_recoveryCnt;

    // 無敵時間更新
    --_owner._invincibleCnt;
}
