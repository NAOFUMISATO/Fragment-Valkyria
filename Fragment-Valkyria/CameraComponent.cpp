
/*****************************************************************//**
 * \file   CameraComponent.cpp
 * \brief  �J�����Ǘ�
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "CameraComponent.h"
#include "GameMain.h"
using namespace FragmentValkyria::Camera;

CameraComponent::CameraComponent() {
   Init();
}

void CameraComponent::Init() {
}

void CameraComponent::Input(AppFrame::Input::InputManager& input) {
}

void CameraComponent::Update() {
   //�v���C���[�̔w���ɃJ�����ʒu��ݒ肷��
   auto forward = _forwardOfTarget;
   auto fromTarget = forward * -_targetDistance;
   fromTarget.SetY(_vertDistance);
   _position = _target + fromTarget;
   //�r���[�s��̐ݒ�
   auto cameraMatrix = GetCameraViewMatrix(_position, _target, _up);
   SetCameraViewMatrix(ToDX(cameraMatrix));

   //���e�s��̐ݒ�
   auto [cameraNear, cameraFar, fov] = _nearFarFov;
   auto projectionMatrix = GetCameraProjectionMatrix(cameraNear, cameraFar, fov);
   SetupCamera_ProjectionMatrix(ToDX(projectionMatrix));
}

void CameraComponent::Draw() {

}

AppFrame::Math::Matrix44 CameraComponent::GetCameraViewMatrix(AppFrame::Math::Vector4& cameraPosition, AppFrame::Math::Vector4& cameraTarget, AppFrame::Math::Vector4& cameraUp) {
   Matrix44 cameraMatrix = Matrix44();
   // �J�������_�̐ݒ�
   cameraMatrix.LookAt(cameraPosition, cameraTarget, cameraUp);

   return cameraMatrix;
}

AppFrame::Math::Matrix44 CameraComponent::GetCameraProjectionMatrix(double cameraNear, double cameraFar, double fov) {
   auto aspect = static_cast<float>(FragmentValkyria::ScreenHeight) / static_cast<float>(FragmentValkyria::ScreenWidth);
   Matrix44 projection_matrix = Matrix44();
   // �����ϊ�
   projection_matrix.Perspective(fov, aspect, cameraNear, cameraFar);

   return projection_matrix;
}
