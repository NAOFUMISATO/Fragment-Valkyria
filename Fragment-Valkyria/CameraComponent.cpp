
/*****************************************************************//**
 * \file   CameraComponent.cpp
 * \brief  �J�����Ǘ�
 *
 * \author NAOFUMISATO, AHMD2000
 * \date   December 2021
 *********************************************************************/
#include "CameraComponent.h"
#include "GameMain.h"

namespace {
   auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("cameracomponent", {
      "dead_zone_divide_value", "center_height", "spring_k",
      "divide_t" });

   const int DeadZoneDivideValue = paramMap["dead_zone_divide_value"];     //!< �p�b�h�̃X�e�B�b�N�̈������̓��͔͈͂����̕������Ŋ�������
   const double CenterHeight = paramMap["center_height"];                  //!< �΂˂̓������n�߂�ʒu�܂ł̍���
   const double SpringK = paramMap["spring_k"];                            //!< �΂˒萔
   const double DivideT = paramMap["divide_t"];                            //!< �I�C���[�@�̎��Ԃ̕�����
}

using namespace FragmentValkyria::Camera;

CameraComponent::CameraComponent(Game::GameMain& gameMain) :_gameMain{gameMain} {
}

void CameraComponent::Init() {
    //�v���C���[����̃J�����̒����_�ւ̃x�N�g�����쐬
    _firstPlyToTarget = _target - _plyPos;
    //�v���C���[����̃J�����̈ʒu�ւ̃x�N�g�����쐬
    _firstPlyToPos = _position - _plyPos;
    //�x�N�g����90�x��]������}�g���N�X�̍쐬
    _anyAxisMatrix.RotateY(90.0, true);
    //�J�����̈ʒu���璍���_�ւ̃x�N�g���̍쐬
    auto posToTarget = _target - _position;
    posToTarget.Normalized();
    _posToTarget = posToTarget * 300.0;
    // �J�����̕`����E����сA����p�̏�����
    _nearFarFov = { 100.0,10000.0,AppFrame::Math::Utility::DegreeToRadian(60.0) };
}

void CameraComponent::Input(AppFrame::Input::InputManager& input) {
    //�e��Ԃ̓��͏�������
    _stateServer->Input(input);
}

void CameraComponent::Update() {
    //�e��Ԃ̍X�V��������
    _stateServer->Update();

    //�r���[�s��̐ݒ�
    auto cameraMatrix = GetCameraViewMatrix(_position, _target, _up);
    SetCameraViewMatrix(ToDX(cameraMatrix));

    //���e�s��̐ݒ�
    auto [cameraNear, cameraFar, fov] = _nearFarFov;
    auto projectionMatrix = GetCameraProjectionMatrix(cameraNear, cameraFar, fov);
    SetupCamera_ProjectionMatrix(ToDX(projectionMatrix));

    // 3D�T�E���h�̃��X�i�[�ݒ�(�J�����Ɠ���)
    Set3DSoundListenerPosAndFrontPosAndUpVec(ToDX(_position), ToDX(_target), ToDX(_up));
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

void CameraComponent::Rotate() {
    //�v���C���[�̈ʒu����̒����_�ւ̃x�N�g�����쐬
    _plyToTarget = _firstPlyToTarget * _rotateMatrix;
    //�Y�[�������T�C���̒l����Z�o
    auto sinValue = std::sin(_zoomRateRadian);
    _zoomRate = _posToTarget * sinValue;
    //�v���C���[����J�����̈ʒu�ւ̃x�N�g�����쐬
    _plyToPos = (_firstPlyToPos + _zoomRate) * _rotateMatrix;
}

void CameraComponent::Placement() {
    //�v���C���[�̈ʒu����J�����̒����_��ݒ肷��
    _target = _plyPos + _plyToTarget;
    //�v���C���[�̈ʒu����J�����̈ʒu��ݒ肷��
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
   // �Q�[�������x�̎擾
   auto [cameraSens, aimSens, deadZone] = _owner._gameMain.sensitivity();
   // �f�b�h�]�[���͈̔͂�ݒ肷�邽�߂Ƀf�t�H���g�̃f�b�g�]�[���Ɋ|����l�̐ݒ�
   auto deadZoneMulti = DeadZoneDivideValue / deadZone;
   // �E�X�e�B�b�N����ɌX���Ă�����J�����̏㉺�̉�]�̊p�x�𑝂₷
   // �f�b�h�]�[�����傫���p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̍ŏ��͈͓̔��������ꍇ
   if (input.GetXJoypad().RightStickY() >= deadZone && input.GetXJoypad().RightStickY() < deadZone * deadZoneMulti) {
      // �J�������x��0.2�{�������̂𑫂�
      _owner._upDownAngle += cameraSens * 0.2;
      // 10�x�ȏ�X���Ă�����10�x�ɂ���
      if (_owner._upDownAngle >= 10.0) {
         _owner._upDownAngle = 10.0;
      }
   }
   // �p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̓�ڂ͈͓̔��������ꍇ
   else if (input.GetXJoypad().RightStickY() >= deadZone * deadZoneMulti && input.GetXJoypad().RightStickY() < deadZone * deadZoneMulti * 2) {
      // �J�������x��0.4�{�������̂𑫂�
      _owner._upDownAngle += cameraSens * 0.4;
      // 10�x�ȏ�X���Ă�����10�x�ɂ���
      if (_owner._upDownAngle >= 10.0) {
         _owner._upDownAngle = 10.0;
      }
   }
   // �p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̎O�ڂ͈͓̔��������ꍇ
   else if (input.GetXJoypad().RightStickY() >= deadZone * deadZoneMulti * 2 && input.GetXJoypad().RightStickY() < deadZone * deadZoneMulti * 3) {
      // �J�������x��0.6�{�������̂𑫂�
      _owner._upDownAngle += cameraSens * 0.6;
      // 10�x�ȏ�X���Ă�����10�x�ɂ���
      if (_owner._upDownAngle >= 10.0) {
         _owner._upDownAngle = 10.0;
      }
   }
   // �p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̎l�ڂ͈͓̔��������ꍇ
   else if (input.GetXJoypad().RightStickY() >= deadZone * deadZoneMulti * 3 && input.GetXJoypad().RightStickY() < deadZone * deadZoneMulti * 4) {
      // �J�������x��0.8�{�������̂𑫂�
      _owner._upDownAngle += cameraSens * 0.8;
      // 10�x�ȏ�X���Ă�����10�x�ɂ���
      if (_owner._upDownAngle >= 10.0) {
         _owner._upDownAngle = 10.0;
      }
   }
   // �p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̎l�ڂ͈̔͂��傫�������ꍇ
   else if (input.GetXJoypad().RightStickY() >= deadZone * deadZoneMulti * 4) {
      // �J�������x�𑫂�
      _owner._upDownAngle += cameraSens;
      // 10�x�ȏ�X���Ă�����10�x�ɂ���
      if (_owner._upDownAngle >= 10.0) {
         _owner._upDownAngle = 10.0;
      }
   }
   // �E�X�e�B�b�N�����ɌX���Ă�����J�����̏㉺�̉�]�̊p�x�����炷
   // �f�b�h�]�[����菬�����p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̍ŏ��͈͓̔��������ꍇ
   if (input.GetXJoypad().RightStickY() <= -deadZone && input.GetXJoypad().RightStickY() > -deadZone * deadZoneMulti) {
      // �J�������x��0.2�{�������̂�����
      _owner._upDownAngle -= cameraSens * 0.2;
      // -20�x�ȏ�X���Ă�����-20�x�ɂ���
      if (_owner._upDownAngle <= -20.0) {
         _owner._upDownAngle = -20.0;
      }
   }
   // �p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̓�ڂ͈͓̔��������ꍇ
   else if (input.GetXJoypad().RightStickY() <= -deadZone * deadZoneMulti && input.GetXJoypad().RightStickY() > -deadZone * deadZoneMulti * 2) {
      // �J�������x��0.4�{�������̂�����
      _owner._upDownAngle -= cameraSens * 0.4;
      // -20�x�ȏ�X���Ă�����-20�x�ɂ���
      if (_owner._upDownAngle <= -20.0) {
         _owner._upDownAngle = -20.0;
      }
   }
   // �p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̎O�ڂ͈͓̔��������ꍇ
   else if (input.GetXJoypad().RightStickY() <= -deadZone * deadZoneMulti * 2 && input.GetXJoypad().RightStickY() > -deadZone * deadZoneMulti * 3) {
      // �J�������x��0.6�{�������̂�����
      _owner._upDownAngle -= cameraSens * 0.6;
      // -20�x�ȏ�X���Ă�����-20�x�ɂ���
      if (_owner._upDownAngle <= -20.0) {
         _owner._upDownAngle = -20.0;
      }
   }
   // �p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̎l�ڂ͈͓̔��������ꍇ
   else if (input.GetXJoypad().RightStickY() <= -deadZone * deadZoneMulti * 3 && input.GetXJoypad().RightStickY() > -deadZone * deadZoneMulti * 4) {
      // �J�������x��0.8�{�������̂�����
      _owner._upDownAngle -= cameraSens * 0.8;
      // -20�x�ȏ�X���Ă�����-20�x�ɂ���
      if (_owner._upDownAngle <= -20.0) {
         _owner._upDownAngle = -20.0;
      }
   }
   // �p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̎l�ڂ͈̔͂�菬���������ꍇ
   else if (input.GetXJoypad().RightStickY() <= -deadZone * deadZoneMulti * 4) {
      // �J�������x������
      _owner._upDownAngle -= cameraSens;
      // -20�x�ȏ�X���Ă�����-20�x�ɂ���
      if (_owner._upDownAngle <= -20.0) {
         _owner._upDownAngle = -20.0;
      }
   }
   // �E�X�e�B�b�N���E�ɌX���Ă�����J�����̍��E�̉�]�̊p�x�����炷
   // �f�b�h�]�[�����傫���p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̍ŏ��͈͓̔��������ꍇ
   if (input.GetXJoypad().RightStickX() >= deadZone && input.GetXJoypad().RightStickX() < deadZone * deadZoneMulti) {
      // �J�������x��0.2�{�������̂�����
      _owner._sideAngle -= cameraSens * 0.2;
      // -360�x�ȉ��ɂȂ�����0�x�ɂ���
      if (_owner._sideAngle <= -360.0) {
         _owner._sideAngle = 0.0;
      }
   }
   // �p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̓�ڂ͈͓̔��������ꍇ
   else if (input.GetXJoypad().RightStickX() >= deadZone * deadZoneMulti && input.GetXJoypad().RightStickX() < deadZone * deadZoneMulti * 2) {
      // �J�������x��0.4�{�������̂�����
      _owner._sideAngle -= cameraSens * 0.4;
      //-360�x�ȉ��ɂȂ�����0�x�ɂ���
      if (_owner._sideAngle <= -360.0) {
         _owner._sideAngle = 0.0;
      }
   }
   // �p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̎O�ڂ͈͓̔��������ꍇ
   else if (input.GetXJoypad().RightStickX() >= deadZone * deadZoneMulti * 2 && input.GetXJoypad().RightStickX() < deadZone * deadZoneMulti * 3) {
      // �J�������x��0.6�{�������̂�����
      _owner._sideAngle -= cameraSens * 0.6;
      // -360�x�ȉ��ɂȂ�����0�x�ɂ���
      if (_owner._sideAngle <= -360.0) {
         _owner._sideAngle = 0.0;
      }
   }
   // �p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̎l�ڂ͈͓̔��������ꍇ
   else if (input.GetXJoypad().RightStickX() >= deadZone * deadZoneMulti * 3 && input.GetXJoypad().RightStickX() < deadZone * deadZoneMulti * 4) {
      // �J�������x��0.8�{�������̂�����
      _owner._sideAngle -= cameraSens * 0.8;
      // -360�x�ȉ��ɂȂ�����0�x�ɂ���
      if (_owner._sideAngle <= -360.0) {
         _owner._sideAngle = 0.0;
      }
   }
   // �p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̎l�ڂ͈̔͂��傫�������ꍇ
   else if (input.GetXJoypad().RightStickX() >= deadZone * deadZoneMulti * 4) {
      // �J�������x������
      _owner._sideAngle -= cameraSens;
      // -360�x�ȉ��ɂȂ�����0�x�ɂ���
      if (_owner._sideAngle <= -360.0) {
         _owner._sideAngle = 0.0;
      }
   }
   // �E�X�e�B�b�N�����ɌX���Ă�����J�����̍��E�̉�]�̊p�x�𑝂₷
   // �f�b�h�]�[����菬�����p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̍ŏ��͈͓̔��������ꍇ
   if (input.GetXJoypad().RightStickX() <= -deadZone && input.GetXJoypad().RightStickX() > -deadZone * deadZoneMulti) {
      // �J�������x��0.2�{�������̂𑫂�
      _owner._sideAngle += cameraSens * 0.2;
      // 360�x�ȏ�ɂȂ�����0�x�ɂ���
      if (_owner._sideAngle >= 360.0) {
         _owner._sideAngle = 0.0;
      }
   }
   // �p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̓�ڂ͈͓̔��������ꍇ
   else if (input.GetXJoypad().RightStickX() <= -deadZone * deadZoneMulti && input.GetXJoypad().RightStickX() > -deadZone * deadZoneMulti * 2) {
      // �J�������x��0.4�{�������̂𑫂�
      _owner._sideAngle += cameraSens * 0.4;
      // 360�x�ȏ�ɂȂ�����0�x�ɂ���
      if (_owner._sideAngle >= 360.0) {
         _owner._sideAngle = 0.0;
      }
   }
   // �p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̎O�ڂ͈͓̔��������ꍇ
   else if (input.GetXJoypad().RightStickX() <= -deadZone * deadZoneMulti * 2 && input.GetXJoypad().RightStickX() > -deadZone * deadZoneMulti * 3) {
      // �J�������x��0.6�{�������̂𑫂�
      _owner._sideAngle += cameraSens * 0.6;
      // 360�x�ȏ�ɂȂ�����0�x�ɂ���
      if (_owner._sideAngle >= 360.0) {
         _owner._sideAngle = 0.0;
      }
   }
   // �p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̎l�ڂ͈͓̔��������ꍇ
   else if (input.GetXJoypad().RightStickX() <= -deadZone * deadZoneMulti * 3 && input.GetXJoypad().RightStickX() > -deadZone * deadZoneMulti * 4) {
      // �J�������x��0.8�{�������̂𑫂�
      _owner._sideAngle += cameraSens * 0.8;
      // 360�x�ȏ�ɂȂ�����0�x�ɂ���
      if (_owner._sideAngle >= 360.0) {
         _owner._sideAngle = 0.0;
      }
   }
   // �p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̎l�ڂ͈̔͂�菬���������ꍇ
   else if (input.GetXJoypad().RightStickX() <= -deadZone * deadZoneMulti * 4) {
      // �J�������x�𑫂�
      _owner._sideAngle += cameraSens;
      // 360�x�ȏ�ɂȂ�����0�x�ɂ���
      if (_owner._sideAngle >= 360.0) {
         _owner._sideAngle = 0.0;
      }
   }
   // �J�����̈ʒu���璍���_�ւ̃x�N�g���쐬
   auto posToTarget = _owner._target - _owner._position;
   // �J�����̈ʒu���璍���_�ւ̃x�N�g����90�x��]������
   auto anyAxisVec = posToTarget * _owner._anyAxisMatrix;
   // �㉺�̉�]�̃}�g���N�X�쐬
   Matrix44 rotateUpDown = Matrix44();
   rotateUpDown.RotateAnyVecQuaternion(anyAxisVec, _owner._upDownAngle, true);
   // ���E�̉�]�̃}�g���N�X�쐬
   Matrix44 rotateSide = Matrix44();
   rotateSide.RotateAnyVecQuaternion(Vector4(0.0, 1.0, 0.0), _owner._sideAngle, true);
   // �㉺�̉�]�ƍ��E�̉�]�����킹���}�g���N�X���쐬
   _owner._rotateMatrix = rotateSide * rotateUpDown;
}

void CameraComponent::StateNormal::Update() {
    _owner.Rotate();
    _owner.Vibration();
    _owner.Placement();
    // �Y�[������ꍇ�̓Y�[����Ԃ�
    if (_owner._zoom) {
        _owner._stateServer->PushBack("ZoomIn");
    }
}

void CameraComponent::StateZoomIn::Enter() {
    // �Y�[���̊����̃T�C���̒l����郉�W�A����0.0��
    _owner._zoomRateRadian = 0.0;
}

void CameraComponent::StateZoomIn::Update() {
    // �Y�[���̊����̃T�C���̒l����郉�W�A����5���W�A�����₷
    _owner._zoomRateRadian += AppFrame::Math::RADIAN_1 * 5.0;
    // �����񕪂̃p�C���W�A�����傫���Ȃ�����ˌ�������Ԃ�
    if (_owner._zoomRateRadian >= AppFrame::Math::PI / 2.0) {
        _owner._stateServer->PopBack();
        _owner._stateServer->PushBack("ShootReady");
    }
    _owner.Rotate();
    _owner.Vibration();
    _owner.Placement();
}

void CameraComponent::StateShootReady::Input(InputManager& input) {
   // ���x�̎擾
    auto [cameraSens, aimSens, deadZone] = _owner._gameMain.sensitivity();
    // �f�b�h�]�[���͈̔͂�ݒ肷�邽�߂Ƀf�t�H���g�̃f�b�g�]�[���Ɋ|����l�̐ݒ�
    auto deadZoneMulti = DeadZoneDivideValue / deadZone;
    // �E�X�e�B�b�N����ɌX���Ă�����J�����̏㉺�̉�]�̊p�x�𑝂₷
   // �f�b�h�]�[�����傫���p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̍ŏ��͈͓̔��������ꍇ
    if (input.GetXJoypad().RightStickY() >= deadZone && input.GetXJoypad().RightStickY() < deadZone * deadZoneMulti) {
       // �G�C�����x��0.2�{�������̂𑫂�
       _owner._upDownAngle += aimSens * 0.2;
       // 10�x�ȏ�X���Ă�����10�x�ɂ���
       if (_owner._upDownAngle >= 10.0) {
          _owner._upDownAngle = 10.0;
       }
    }
    // �p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̓�ڂ͈͓̔��������ꍇ
    else if (input.GetXJoypad().RightStickY() >= deadZone * deadZoneMulti && input.GetXJoypad().RightStickY() < deadZone * deadZoneMulti * 2) {
       // �G�C�����x��0.4�{�������̂𑫂�
       _owner._upDownAngle += aimSens * 0.4;
       // 10�x�ȏ�X���Ă�����10�x�ɂ���
       if (_owner._upDownAngle >= 10.0) {
          _owner._upDownAngle = 10.0;
       }
    }
    // �p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̎O�ڂ͈͓̔��������ꍇ
    else if (input.GetXJoypad().RightStickY() >= deadZone * deadZoneMulti * 2 && input.GetXJoypad().RightStickY() < deadZone * deadZoneMulti * 3) {
       // �G�C�����x��0.6�{�������̂𑫂�
       _owner._upDownAngle += aimSens * 0.6;
       // 10�x�ȏ�X���Ă�����10�x�ɂ���
       if (_owner._upDownAngle >= 10.0) {
          _owner._upDownAngle = 10.0;
       }
    }
    // �p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̎l�ڂ͈͓̔��������ꍇ
    else if (input.GetXJoypad().RightStickY() >= deadZone * deadZoneMulti * 3 && input.GetXJoypad().RightStickY() < deadZone * deadZoneMulti * 4) {
       // �G�C�����x��0.8�{�������̂𑫂�
       _owner._upDownAngle += aimSens * 0.8;
       // 10�x�ȏ�X���Ă�����10�x�ɂ���
       if (_owner._upDownAngle >= 10.0) {
          _owner._upDownAngle = 10.0;
       }
    }
    // �p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̎l�ڂ͈̔͂��傫�������ꍇ
    else if (input.GetXJoypad().RightStickY() >= deadZone * deadZoneMulti * 4) {
       // �G�C�����x�𑫂�
       _owner._upDownAngle += aimSens;
       // 10�x�ȏ�X���Ă�����10�x�ɂ���
       if (_owner._upDownAngle >= 10.0) {
          _owner._upDownAngle = 10.0;
       }
    }
    // �E�X�e�B�b�N�����ɌX���Ă�����J�����̏㉺�̉�]�̊p�x�����炷
    // �f�b�h�]�[����菬�����p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̍ŏ��͈͓̔��������ꍇ
    if (input.GetXJoypad().RightStickY() <= -deadZone && input.GetXJoypad().RightStickY() > -deadZone * deadZoneMulti) {
       // �G�C�����x��0.2�{�������̂�����
       _owner._upDownAngle -= aimSens * 0.2;
       // -20�x�ȏ�X���Ă�����-20�x�ɂ���
       if (_owner._upDownAngle <= -20.0) {
          _owner._upDownAngle = -20.0;
       }
    }
    // �p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̓�ڂ͈͓̔��������ꍇ
    else if (input.GetXJoypad().RightStickY() <= -deadZone * deadZoneMulti && input.GetXJoypad().RightStickY() > -deadZone * deadZoneMulti * 2) {
       // �G�C�����x��0.4�{�������̂�����
       _owner._upDownAngle -= aimSens * 0.4;
       // -20�x�ȏ�X���Ă�����-20�x�ɂ���
       if (_owner._upDownAngle <= -20.0) {
          _owner._upDownAngle = -20.0;
       }
    }
    // �p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̎O�ڂ͈͓̔��������ꍇ
    else if (input.GetXJoypad().RightStickY() <= -deadZone * deadZoneMulti * 2 && input.GetXJoypad().RightStickY() > -deadZone * deadZoneMulti * 3) {
       // �G�C�����x��0.6�{�������̂�����
       _owner._upDownAngle -= aimSens * 0.6;
       // -20�x�ȏ�X���Ă�����-20�x�ɂ���
       if (_owner._upDownAngle <= -20.0) {
          _owner._upDownAngle = -20.0;
       }
    }
    // �p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̎l�ڂ͈͓̔��������ꍇ
    else if (input.GetXJoypad().RightStickY() <= -deadZone * deadZoneMulti * 3 && input.GetXJoypad().RightStickY() > -deadZone * deadZoneMulti * 4) {
       // �G�C�����x��0.8�{�������̂�����
       _owner._upDownAngle -= aimSens * 0.8;
       // -20�x�ȏ�X���Ă�����-20�x�ɂ���
       if (_owner._upDownAngle <= -20.0) {
          _owner._upDownAngle = -20.0;
       }
    }
    // �p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̎l�ڂ͈̔͂�菬���������ꍇ
    else if (input.GetXJoypad().RightStickY() <= -deadZone * deadZoneMulti * 4) {
       // �G�C�����x������
       _owner._upDownAngle -= aimSens;
       // -20�x�ȏ�X���Ă�����-20�x�ɂ���
       if (_owner._upDownAngle <= -20.0) {
          _owner._upDownAngle = -20.0;
       }
    }
    // �E�X�e�B�b�N���E�ɌX���Ă�����J�����̍��E�̉�]�̊p�x�����炷
    // �f�b�h�]�[�����傫���p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̍ŏ��͈͓̔��������ꍇ
    if (input.GetXJoypad().RightStickX() >= deadZone && input.GetXJoypad().RightStickX() < deadZone * deadZoneMulti) {
       // �G�C�����x��0.2�{�������̂�����
       _owner._sideAngle -= aimSens * 0.2;
       // -360�x�ȉ��ɂȂ�����0�x�ɂ���
       if (_owner._sideAngle <= -360.0) {
          _owner._sideAngle = 0.0;
       }
    }
    // �p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̓�ڂ͈͓̔��������ꍇ
    else if (input.GetXJoypad().RightStickX() >= deadZone * deadZoneMulti && input.GetXJoypad().RightStickX() < deadZone * deadZoneMulti * 2) {
       // �G�C�����x��0.4�{�������̂�����
       _owner._sideAngle -= aimSens * 0.4;
       //-360�x�ȉ��ɂȂ�����0�x�ɂ���
       if (_owner._sideAngle <= -360.0) {
          _owner._sideAngle = 0.0;
       }
    }
    // �p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̎O�ڂ͈͓̔��������ꍇ
    else if (input.GetXJoypad().RightStickX() >= deadZone * deadZoneMulti * 2 && input.GetXJoypad().RightStickX() < deadZone * deadZoneMulti * 3) {
       // �G�C�����x��0.6�{�������̂�����
       _owner._sideAngle -= aimSens * 0.6;
       // -360�x�ȉ��ɂȂ�����0�x�ɂ���
       if (_owner._sideAngle <= -360.0) {
          _owner._sideAngle = 0.0;
       }
    }
    // �p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̎l�ڂ͈͓̔��������ꍇ
    else if (input.GetXJoypad().RightStickX() >= deadZone * deadZoneMulti * 3 && input.GetXJoypad().RightStickX() < deadZone * deadZoneMulti * 4) {
       // �G�C�����x��0.8�{�������̂�����
       _owner._sideAngle -= aimSens * 0.8;
       // -360�x�ȉ��ɂȂ�����0�x�ɂ���
       if (_owner._sideAngle <= -360.0) {
          _owner._sideAngle = 0.0;
       }
    }
    // �p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̎l�ڂ͈̔͂��傫�������ꍇ
    else if (input.GetXJoypad().RightStickX() >= deadZone * deadZoneMulti * 4) {
       // �G�C�����x������
       _owner._sideAngle -= aimSens;
       // -360�x�ȉ��ɂȂ�����0�x�ɂ���
       if (_owner._sideAngle <= -360.0) {
          _owner._sideAngle = 0.0;
       }
    }
    // �E�X�e�B�b�N�����ɌX���Ă�����J�����̍��E�̉�]�̊p�x�𑝂₷
    // �f�b�h�]�[����菬�����p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̍ŏ��͈͓̔��������ꍇ
    if (input.GetXJoypad().RightStickX() <= -deadZone && input.GetXJoypad().RightStickX() > -deadZone * deadZoneMulti) {
       // �G�C�����x��0.2�{�������̂𑫂�
       _owner._sideAngle += aimSens * 0.2;
       // 360�x�ȏ�ɂȂ�����0�x�ɂ���
       if (_owner._sideAngle >= 360.0) {
          _owner._sideAngle = 0.0;
       }
    }
    // �p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̓�ڂ͈͓̔��������ꍇ
    else if (input.GetXJoypad().RightStickX() <= -deadZone * deadZoneMulti && input.GetXJoypad().RightStickX() > -deadZone * deadZoneMulti * 2) {
       // �G�C�����x��0.4�{�������̂𑫂�
       _owner._sideAngle += aimSens * 0.4;
       // 360�x�ȏ�ɂȂ�����0�x�ɂ���
       if (_owner._sideAngle >= 360.0) {
          _owner._sideAngle = 0.0;
       }
    }
    // �p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̎O�ڂ͈͓̔��������ꍇ
    else if (input.GetXJoypad().RightStickX() <= -deadZone * deadZoneMulti * 2 && input.GetXJoypad().RightStickX() > -deadZone * deadZoneMulti * 3) {
       // �G�C�����x��0.6�{�������̂𑫂�
       _owner._sideAngle += aimSens * 0.6;
       // 360�x�ȏ�ɂȂ�����0�x�ɂ���
       if (_owner._sideAngle >= 360.0) {
          _owner._sideAngle = 0.0;
       }
    }
    // �p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̎l�ڂ͈͓̔��������ꍇ
    else if (input.GetXJoypad().RightStickX() <= -deadZone * deadZoneMulti * 3 && input.GetXJoypad().RightStickX() > -deadZone * deadZoneMulti * 4) {
       // �G�C�����x��0.8�{�������̂𑫂�
       _owner._sideAngle += aimSens * 0.8;
       // 360�x�ȏ�ɂȂ�����0�x�ɂ���
       if (_owner._sideAngle >= 360.0) {
          _owner._sideAngle = 0.0;
       }
    }
    // �p�b�h�̃X�e�B�b�N�̓��͔͈͂𕪊������͈͂̎l�ڂ͈̔͂�菬���������ꍇ
    else if (input.GetXJoypad().RightStickX() <= -deadZone * deadZoneMulti * 4) {
       // �G�C�����x�𑫂�
       _owner._sideAngle += aimSens;
       // 360�x�ȏ�ɂȂ�����0�x�ɂ���
       if (_owner._sideAngle >= 360.0) {
          _owner._sideAngle = 0.0;
       }
    }
    // �J�����̈ʒu���璍���_�ւ̃x�N�g���쐬
    auto posToTarget = _owner._target - _owner._position;
    // �J�����̈ʒu���璍���_�ւ̃x�N�g����90�x��]������
    auto anyAxisVec = posToTarget * _owner._anyAxisMatrix;
    // �㉺�̉�]�̃}�g���N�X�쐬
    Matrix44 rotateUpDown = Matrix44();
    rotateUpDown.RotateAnyVec(anyAxisVec, _owner._upDownAngle, true);
    // ���E�̉�]�̃}�g���N�X�쐬
    Matrix44 rotateSide = Matrix44();
    rotateSide.RotateAnyVec(Vector4(0.0, 1.0, 0.0), _owner._sideAngle, true);
    // �㉺�̉�]�ƍ��E�̉�]�����킹���}�g���N�X���쐬
    _owner._rotateMatrix = rotateSide * rotateUpDown;
}

void CameraComponent::StateShootReady::Update() {
    _owner.Rotate();
    _owner.Vibration();
    _owner.Placement();
    // �Y�[�����Ȃ��ꍇ�Y�[���A�E�g��Ԃ�
    if (!_owner._zoom) {
        _owner._stateServer->PopBack();
        _owner._stateServer->PushBack("ZoomOut");
    }
}

void CameraComponent::StateZoomOut::Update() {
    // �Y�[���̊����̃T�C���̒l����郉�W�A����5���W�A�����炷
    _owner._zoomRateRadian -= AppFrame::Math::RADIAN_1 * 5.0;
    // ����0���W�A����菬�����Ȃ�����ʏ��Ԃ�
    if (_owner._zoomRateRadian <= 0.0) {
        _owner._stateServer->PopBack();
        _owner._stateServer->PushBack("Normal");
    }
    _owner.Rotate();
    _owner.Vibration();
    _owner.Placement();
}
