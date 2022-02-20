
/*****************************************************************//**
 * \file   CameraComponent.cpp
 * \brief  カメラ管理
 *
 * \author NAOFUMISATO, AHMD2000
 * \date   December 2021
 *********************************************************************/
#include "CameraComponent.h"
#include "GameMain.h"
using namespace FragmentValkyria::Camera;

namespace {
    const double CenterHeight = 60.0;
    const double SpringK = 1.0;
    const double DivideT = 10.0;
}

CameraComponent::CameraComponent(Game::GameMain& gameMain) :_gameMain{gameMain} {
}

void CameraComponent::Init() {
    //プレイヤーからのカメラの注視点へのベクトルを作成
    _firstPlyToTarget = _target - _plyPos;
    //プレイヤーからのカメラの位置へのベクトルを作成
    _firstPlyToPos = _position - _plyPos;
    //ベクトルを90度回転させるマトリクスの作成
    _anyAxisMatrix.RotateY(90.0, true);
    //カメラの位置から注視点へのベクトルの作成
    auto posToTarget = _target - _position;
    posToTarget.Normalized();
    _posToTarget = posToTarget * 300.0;
}

void CameraComponent::Input(AppFrame::Input::InputManager& input) {
    //各状態の入力処理を回す
    _stateServer->Input(input);
}

void CameraComponent::Update() {
    //各状態の更新処理を回す
    _stateServer->Update();

    //ビュー行列の設定
    auto cameraMatrix = GetCameraViewMatrix(_position, _target, _up);
    SetCameraViewMatrix(ToDX(cameraMatrix));

    //投影行列の設定
    auto [cameraNear, cameraFar, fov] = _nearFarFov;
    auto projectionMatrix = GetCameraProjectionMatrix(cameraNear, cameraFar, fov);
    SetupCamera_ProjectionMatrix(ToDX(projectionMatrix));

}

AppFrame::Math::Matrix44 CameraComponent::GetCameraViewMatrix(AppFrame::Math::Vector4& cameraPosition, AppFrame::Math::Vector4& cameraTarget, AppFrame::Math::Vector4& cameraUp) {
   Matrix44 cameraMatrix = Matrix44();
   // カメラ視点の設定
   cameraMatrix.LookAt(cameraPosition, cameraTarget, cameraUp);

   return cameraMatrix;
}

AppFrame::Math::Matrix44 CameraComponent::GetCameraProjectionMatrix(double cameraNear, double cameraFar, double fov) {
   auto aspect = static_cast<float>(FragmentValkyria::ScreenHeight) / static_cast<float>(FragmentValkyria::ScreenWidth);
   Matrix44 projection_matrix = Matrix44();
   // 透視変換
   projection_matrix.Perspective(fov, aspect, cameraNear, cameraFar);

   return projection_matrix;
}

void CameraComponent::Rotate() {
    //プレイヤーの位置からの注視点へのベクトルを作成
    _plyToTarget = _firstPlyToTarget * _rotateMatrix;
    //ズーム率をサインの値から算出
    auto sinValue = std::sin(_zoomRateRadian);
    _zoomRate = _posToTarget * sinValue;
    //プレイヤーからカメラの位置へのベクトルを作成
    _plyToPos = (_firstPlyToPos + _zoomRate) * _rotateMatrix;
}

void CameraComponent::Placement() {
    //プレイヤーの位置からカメラの注視点を設定する
    _target = _plyPos + _plyToTarget;
    //プレイヤーの位置からカメラの位置を設定する
    _position = _plyPos + _plyToPos;
    _position.Add(0.0, _vibrationValue, 0.0);
}

void CameraComponent::Vibration() {
    for (auto i = 0.0; i < DivideT; ++i) {
        _vibrationValue += _vibrationVelocity / DivideT;
        _vibrationVelocity += (-SpringK * (_vibrationValue - CenterHeight)) / DivideT;
    }
    _vibrationVelocity *= 0.9;
}

void CameraComponent::StateNormal::Input(InputManager& input) {
   auto [cameraSens, aimSens, deadZone] = _owner._gameMain.sensitivity();
   //右スティックが上に傾いていたらカメラの上下の回転の角度を増やす
    if (input.GetXJoypad().RightStickY() >= deadZone) {
        _owner._upDownAngle += cameraSens;
        //10度以上傾いていたら10度にする
        if (_owner._upDownAngle >= 10.0) {
            _owner._upDownAngle = 10.0;
        }
    }
    //右スティックが下に傾いていたらカメラの上下の回転の角度を減らす
    if (input.GetXJoypad().RightStickY() <= -deadZone) {
        _owner._upDownAngle -= cameraSens;
        //-20度以上傾いていたら-20度にする
        if (_owner._upDownAngle <= -20.0) {
            _owner._upDownAngle = -20.0;
        }
    }
    //右スティックが右に傾いていたらカメラの左右の回転の角度を減らす
    if (input.GetXJoypad().RightStickX() >= deadZone) {
        _owner._sideAngle -= cameraSens;
        //-360度以下になったら0度にする
        if (_owner._sideAngle <= -360.0) {
            _owner._sideAngle = 0.0;
        }
    }
    //右スティックが左に傾いていたらカメラの左右の回転の角度を増やす
    if (input.GetXJoypad().RightStickX() <= -deadZone) {
        _owner._sideAngle += cameraSens;
        if (_owner._sideAngle >= 360.0) {
            _owner._sideAngle = 0.0;
        }
    }
    //カメラの位置から注視点へのベクトル作成
    auto posToTarget = _owner._target - _owner._position;
    //カメラの位置から注視点へのベクトルを90度回転させる
    auto anyAxisVec = posToTarget * _owner._anyAxisMatrix;
    //上下の回転のマトリクス作成
    Matrix44 rotateUpDown = Matrix44();
    rotateUpDown.RotateAnyVecQuaternion(anyAxisVec, _owner._upDownAngle, true);
    //左右の回転のマトリクス作成
    Matrix44 rotateSide = Matrix44();
    rotateSide.RotateAnyVecQuaternion(Vector4(0.0, 1.0, 0.0), _owner._sideAngle, true);
    //上下の回転と左右の回転を合わせたマトリクスを作成
    _owner._rotateMatrix = rotateSide * rotateUpDown;
}

void CameraComponent::StateNormal::Update() {
    _owner.Rotate();
    _owner.Vibration();
    _owner.Placement();
    //ズームする場合はズーム状態へ
    if (_owner._zoom) {
        _owner._stateServer->PushBack("ZoomIn");
    }
}

void CameraComponent::StateZoomIn::Enter() {
    //ズームの割合のサインの値を取るラジアンの0.0に
    _owner._zoomRateRadian = 0.0;
}

void CameraComponent::StateZoomIn::Update() {
    //ズームの割合のサインの値を取るラジアンを5ラジアン増やす
    _owner._zoomRateRadian += AppFrame::Math::RADIAN_1 * 5.0;
    //もし二分のパイラジアンより大きくなったら射撃準備状態へ
    if (_owner._zoomRateRadian >= AppFrame::Math::PI / 2.0) {
        _owner._stateServer->PopBack();
        _owner._stateServer->PushBack("ShootReady");
    }
    _owner.Rotate();
    _owner.Vibration();
    _owner.Placement();
}

void CameraComponent::StateShootReady::Input(InputManager& input) {
    auto [cameraSens, aimSens, deadZone] = _owner._gameMain.sensitivity();
    //右スティックが上に傾いていたらカメラの上下の回転の角度を増やす
    if (input.GetXJoypad().RightStickY() >= deadZone) {
        _owner._upDownAngle += aimSens;
        //10度以上傾いていたら10度にする
        if (_owner._upDownAngle >= 10.0) {
            _owner._upDownAngle = 10.0;
        }
    }
    //右スティックが下に傾いていたらカメラの上下の回転の角度を減らす
    if (input.GetXJoypad().RightStickY() <= -deadZone) {
        
        _owner._upDownAngle -= aimSens;
        //-20度以上傾いていたら-20度にする
        if (_owner._upDownAngle <= -20.0) {
            _owner._upDownAngle = -20.0;
        }
    }
    //右スティックが右に傾いていたらカメラの左右の回転の角度を減らす
    if (input.GetXJoypad().RightStickX() >= deadZone) {

        _owner._sideAngle -= aimSens;
        //-360度以下になったら0度にする
        if (_owner._sideAngle <= -360.0) {
            _owner._sideAngle = 0.0;
        }
    }
    //右スティックが左に傾いていたらカメラの左右の回転の角度を増やす
    if (input.GetXJoypad().RightStickX() <= -deadZone) {
        _owner._sideAngle += aimSens;
        //360度以上になったら0度にする
        if (_owner._sideAngle >= 360.0) {
            _owner._sideAngle = 0.0;
        }
    }
    //カメラの位置から注視点へのベクトル作成
    auto posToTarget = _owner._target - _owner._position;
    //カメラの位置から注視点へのベクトルを90度回転させる
    auto anyAxisVec = posToTarget * _owner._anyAxisMatrix;
    //上下の回転のマトリクス作成
    Matrix44 rotateUpDown = Matrix44();
    rotateUpDown.RotateAnyVec(anyAxisVec, _owner._upDownAngle, true);
    //左右の回転のマトリクス作成
    Matrix44 rotateSide = Matrix44();
    rotateSide.RotateAnyVec(Vector4(0.0, 1.0, 0.0), _owner._sideAngle, true);
    //上下の回転と左右の回転を合わせたマトリクスを作成
    _owner._rotateMatrix = rotateSide * rotateUpDown;
}

void CameraComponent::StateShootReady::Update() {
    _owner.Rotate();
    _owner.Vibration();
    _owner.Placement();
    //ズームしない場合ズームアウト状態へ
    if (!_owner._zoom) {
        _owner._stateServer->PopBack();
        _owner._stateServer->PushBack("ZoomOut");
    }
}

void CameraComponent::StateZoomOut::Update() {
    //ズームの割合のサインの値を取るラジアンを5ラジアン減らす
    _owner._zoomRateRadian -= AppFrame::Math::RADIAN_1 * 5.0;
    //もし0ラジアンより小さくなったら通常状態へ
    if (_owner._zoomRateRadian <= 0.0) {
        _owner._stateServer->PopBack();
        _owner._stateServer->PushBack("Normal");
    }
    _owner.Rotate();
    _owner.Vibration();
    _owner.Placement();
}
