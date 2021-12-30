
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
	_stateServer->Update();
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

void CameraComponent::StateBase::Draw() {
	_owner.Draw();
}

void FragmentValkyria::Camera::CameraComponent::StateNormal::Enter() {

}

void FragmentValkyria::Camera::CameraComponent::StateNormal::Input(InputManager& input) {

}

void FragmentValkyria::Camera::CameraComponent::StateNormal::Update() {
	//�v���C���[�̔w���ɃJ�����ʒu��ݒ肷��
	auto forward = _owner._forwardOfTarget;
	auto fromTarget = forward * - _owner._targetDistance;
	fromTarget.SetY(_owner._vertDistance);
	_owner._position = _owner._target + fromTarget;
	//�r���[�s��̐ݒ�
	auto cameraMatrix = _owner.GetCameraViewMatrix(_owner._position, _owner._target, _owner._up);
	SetCameraViewMatrix(ToDX(cameraMatrix));

	//���e�s��̐ݒ�
	auto [cameraNear, cameraFar, fov] = _owner._nearFarFov;
	auto projectionMatrix = _owner.GetCameraProjectionMatrix(cameraNear, cameraFar, fov);
	SetupCamera_ProjectionMatrix(ToDX(projectionMatrix));
}
