
/*****************************************************************//**
 * \file   CameraComponent.cpp
 * \brief  カメラ管理
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "CameraComponent.h"
#include "GameMain.h"
using namespace FragmentValkyria::Camera;



CameraComponent::CameraComponent(Game::GameMain& gameMain) :_gameMain{gameMain} {
}

void CameraComponent::Init() {
    _firstPlyToTarget = _target - _plyPos;
    _firstPlyToPos = _position - _plyPos;
}

void CameraComponent::Input(AppFrame::Input::InputManager& input) {
    _stateServer->Input(input);
}

void CameraComponent::Update() {

    _stateServer->Update();

    //ビュー行列の設定
    auto cameraMatrix = GetCameraViewMatrix(_position, _target, _up);
    SetCameraViewMatrix(ToDX(cameraMatrix));

    //投影行列の設定
    auto [cameraNear, cameraFar, fov] = _nearFarFov;
    auto projectionMatrix = GetCameraProjectionMatrix(cameraNear, cameraFar, fov);
    SetupCamera_ProjectionMatrix(ToDX(projectionMatrix));

}

void CameraComponent::Draw() {

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
    _plyToTarget = _firstPlyToTarget * _rotateMatrix;
    _plyToPos = _firstPlyToPos * _rotateMatrix;
}

void CameraComponent::Placement() {
    //プレイヤーの背部にカメラ位置を設定する
    _target = _plyPos + _plyToTarget;
    _position = _plyPos + _plyToPos + _zoomRate;
}

void CameraComponent::StateBase::Draw() {
    _owner.Draw();
}

void CameraComponent::StateNormal::Enter() {

}

void CameraComponent::StateNormal::Input(InputManager& input) {
   /* _owner._rotateMatrix = Matrix44();*/
    if (input.GetXJoypad().RightStickY() >= 10000) {
        /*auto upMatrix = Matrix44();
        upMatrix.RotateX(-2.0, true);

        _owner._rotateMatrix = _owner._rotateMatrix * upMatrix;*/
        _owner._rotateMatrix.RotateX(-2.0, false);
    }
    if (input.GetXJoypad().RightStickY() <= -10000) {
        /*auto downMatrix = Matrix44();
        downMatrix.RotateX(2.0, true);

        _owner._rotateMatrix = _owner._rotateMatrix * downMatrix;*/
        _owner._rotateMatrix.RotateX(2.0, false);
    }
    if (input.GetXJoypad().RightStickX() >= 10000) {
        /*auto rightMatrix = Matrix44();
        rightMatrix.RotateY(2.0, true);

        _owner._rotateMatrix = _owner._rotateMatrix * rightMatrix;*/

        _owner._rotateMatrix.RotateY(2.0, false);
    }
    if (input.GetXJoypad().RightStickX() <= -10000) {
        /*auto leftMatrix = Matrix44();
        leftMatrix.RotateY(-2.0, true);

        _owner._rotateMatrix = _owner._rotateMatrix * leftMatrix;*/

        _owner._rotateMatrix.RotateY(-2.0, false);
    }
}

void CameraComponent::StateNormal::Update() {
    _owner.Rotate();
    _owner.Placement();
    
}

void CameraComponent::StateZoomIn::Enter() {
    auto posToTarget = _owner._target - _owner._position;
    posToTarget.Normalized();
    _owner._posToTarget = posToTarget * 100.0;
}

void CameraComponent::StateZoomIn::Input(InputManager& input) {

}

void CameraComponent::StateZoomIn::Update() {

    _owner.Placement();
}

void CameraComponent::StateShootReady::Enter() {

}

void CameraComponent::StateShootReady::Input(InputManager& input) {

}

void CameraComponent::StateShootReady::Update() {

}

void CameraComponent::StateZoomOut::Enter() {

}

void CameraComponent::StateZoomOut::Input(InputManager& input) {

}

void CameraComponent::StateZoomOut::Update() {

}
