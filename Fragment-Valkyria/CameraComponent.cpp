
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
}

void CameraComponent::Init() {
	_plyToTarget = _target - _plyPos;
	_plyToPos = _position - _plyPos;
}

void CameraComponent::Input(AppFrame::Input::InputManager& input) {
	_stateServer->Input(input);
}

void CameraComponent::Update() {
	_stateServer->Update();

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

void CameraComponent::StateBase::Draw() {
	_owner.Draw();
}

void CameraComponent::StateNormal::Enter() {

}

void CameraComponent::StateNormal::Input(InputManager& input) {
	if (input.GetXJoypad().RightStickX() >= 10000) {
		auto rightMatrix = Matrix44();
		rightMatrix.RotateY(2.0, true);

		_owner._plyToTarget = _owner._plyToTarget * rightMatrix;
		_owner._plyToPos = _owner._plyToPos * rightMatrix;
	}
	if (input.GetXJoypad().RightStickX() <= -10000) {
		auto leftMatrix = Matrix44();
		leftMatrix.RotateY(-2.0, true);

		_owner._plyToTarget = _owner._plyToTarget * leftMatrix;
		_owner._plyToPos = _owner._plyToPos * leftMatrix;
	}
}

void CameraComponent::StateNormal::Update() {
	//�v���C���[�̔w���ɃJ�����ʒu��ݒ肷��
	_owner._target = _owner._plyPos + _owner._plyToTarget;
	_owner._position = _owner._plyPos + _owner._plyToPos;
	
}
