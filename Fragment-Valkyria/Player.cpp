
/*****************************************************************//**
 * \file   Player.cpp
 * \brief  �v���C���[�̏������񂷃N���X
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
    constexpr auto FootStepHeight = 3.0;  //!< �����Ԏ��̑�����������(���̍b����̈ʒu)
    constexpr auto FootStepStart = 10;    //!< �����ԑJ�ڎ�����̑����������t���[��
}

using namespace FragmentValkyria::Player;

Player::Player(Game::GameMain& gameMain) : ObjectBase{ gameMain } {
   _param = std::make_unique<Param::ParamPlayer>(_gameMain, "player");
   _collParam = std::make_unique<Param::ParamCollision>(_gameMain, "collision");
}

void Player::Init(){
    _rightRotation.RotateY(90.0, true);     // �x�N�g����90�x��]������}�g���N�X�̍쐬
    _leftRotation.RotateY(-90.0, true);     // �x�N�g����-90�x��]������}�g���N�X�̍쐬
    _backRotation.RotateY(180.0, true);     // �x�N�g����180�x��]������}�g���N�X�̍쐬
    _isAim = false;                         // �G�C�������̃t���O��false
    _isDeadMotion = false;                  // ���S���[�V���������̃t���O��false
    // ��]�Ō��������������ɍŏ��Ɍ����Ă��������ݒ�
    _rotateDir = GetForward() * _param->GetDoubleParam("rotate_rate");
}

void Player::Input(InputManager& input) {
   // �J�����̓��͏�������
   _cameraComponent->Input(input);
   // �e��Ԃ̓��͏�������
   _stateServer->Input(input);
}

void Player::Update() {
   // ��Ԃ̍X�V
   _stateServer->Update();
   // ���[���h�s��̍X�V
   ComputeWorldTransform();
   // ���f���̍X�V
   _modelAnimeComponent->Update();
   // �J�����Ƀv���C���[�̈ʒu��ݒ�
   _cameraComponent->SetPlyPos(_position);
   // �J�����̍X�V
   _cameraComponent->Update();
   // �I�u�W�F�N�g�T�[�o�[�Ɉʒu��ʒm
   GetObjServer().RegistVector("PlayerPos", _position);
   // �I�u�W�F�N�g�T�[�o�[�ɉ�]��ʒm
   GetObjServer().RegistVector("PlayerRot", _rotation);
   // �I�u�W�F�N�g�T�[�o�[�Ƀt�H���[�h�x�N�g����ʒm
   GetObjServer().RegistVector("PlayerFor",GetForward());
   auto headFramePos = modelAnimeComponent().GetFrameChildPosion("Kamilla_kari_Reference", "Kamilla_kari_Head");
   GetObjServer().RegistVector("PlayerHeadPos", headFramePos);
   GetObjServer().RegistVector("CamTarget", _cameraComponent->GetTarget());
   // �I�u�W�F�N�g�T�[�o�[�ɃJ�����̈ʒu��ʒm
   GetObjServer().RegistVector("CamPos", _cameraComponent->GetPos());
}

void Player::Draw() {
   // �e��Ԃ̕`�揈������
   _stateServer->Draw();
}

void Player::ComputeWorldTransform() {
   // ���[���h�s��̍쐬
   auto [sx, sy, sz] = _scale.GetVec3();
   auto [rx, ry, rz] = _rotation.GetVec3();
   auto [px, py, pz] = _position.GetVec3();
   auto world = Matrix44();
   world.Scale(sx, sy, sz, true);
   world.RotateZ(rz, false);
   world.RotateX(rx, false);
   // �f�t�H���g�Ń��f����180�x��]���Ă���̂�180�x��]�����Ă���
   world.RotateY(ry + AppFrame::Math::DEGREES_180, false);
   world.Transfer(px, py, pz, false);
   _worldTransform = world;
}

void Player::Move(const Vector4& moved) {
    // �ړ��ʂ̃x�N�g���̐����𕪉�
    auto [x, y, z] = moved.GetVec3();
    // �ʒu��ۑ�
    auto position = _position;
    // x�����̈ړ���̈ʒu���擾
    position = _collisionComponent->PlayerCheckStage(position, Vector4(x, y, 0.0));
    // z�����̈ړ���̈ʒu���擾
    position = _collisionComponent->PlayerCheckStage(position, Vector4(0.0, y, z));
    // �ʒu��x��z�̐����̈ړ���̈ʒu��ݒ肷��
    _position = position;
}

void Player::ShootRotate() {
    // �J�������璍���_�ւ̒P�ʃx�N�g�������Ƃ߂�
    auto camForward = _cameraComponent->GetForward();
    // �J�������璍���_�ւ̒P�ʃx�N�g���̍������Ȃ���
    auto [x, y, z] = camForward.GetVec3();
    auto direction = Vector4(x, 0.0, z);
    // Y������݂����f���̌����Ă�������̊p�x��ݒ�
    auto radius = std::atan2(x, z);
    _rotation.SetY(AppFrame::Math::Utility::RadianToDegree(radius));
}

void Player::Rotate() {
   // �t�H���[�h�x�N�g���̎擾
   auto forward = GetForward();
   forward = forward * _param->GetDoubleParam("rotate_rate");
   // �t�H���[�h�x�N�g���ƌ��������������̃x�N�g������Ȃ�O�p�`�̖ʐς̎擾
   auto rotateVec = forward.Cross(_rotateDir);
   auto rotateValue = 0.5 * rotateVec.GetY();
   // �ʐς̒l����]������(�v�Z�̏��Ԃ��t�H���[�h�x�N�g��������������������̃x�N�g���ɂ��邱�ƂŌv�Z���ʂ̕��������̂܂ܗ��p���邱�ƂōŒZ�����̉�]���s��)
   _rotation.Add(Vector4(0.0, rotateValue, 0.0));
}

void Player::HitCheckFromFallObjectRange() {
    // �����蔻�茋�ʃN���X�̎Q�Ƃ̎擾
    auto report = _collisionComponent->report();
    // �����蔻�茋�ʂ̊m�F
    if (report.id() == Collision::CollisionComponent::ReportId::HitFromObjectRange) {
        // �����蔻�茋�ʂ��I�u�W�F�N�g�������グ����͈͂ɂ���ƂȂ��Ă�����I�u�W�F�N�g�ˌ�������Ԃ�
        _stateServer->GoToState("ShootReady");
        // �J�����̃Y�[����������
        _cameraComponent->SetZoom(true);
    }
}

void Player::HitCheckFromIdleFallObject(std::string_view state) {
    // �����蔻�茋�ʃN���X�̎Q�Ƃ̎擾
    auto report = _collisionComponent->report();
    // �����蔻�茋�ʂ̊m�F
    if (report.id() == Collision::CollisionComponent::ReportId::HitFromIdleFallObject) {
        // �ҋ@��Ԃ̗����I�u�W�F�N�g�Ɠ������Ă�����
        // �����I�u�W�F�N�g����v���C���[�̈ʒu�܂ł̃x�N�g���̎擾
        auto fromFallObject = _position - _collisionComponent->hitPos();
        // �P�ʉ�����
        fromFallObject.Normalized();
        /**
         * \brief double�^�̒l�𕶎���Ŏw�肵�A�l�Ǘ��N���X����擾����
         * \param paramName �l���w�肷�镶����
         * \return ������ɂ��w�肳�ꂽ�l
         */
        const auto _DoubleParam = [&](std::string paramName) {
           return _param->GetDoubleParam(paramName);
        };
        // �����Ԃ̏ꍇ
        if ("Run" == state) {
           // �����I�u�W�F�N�g����v���C���[�̈ʒu�܂ł̃x�N�g���Ɉړ��̑������������x�N�g�����ʒu�����炷
           _position = _position + fromFallObject * _DoubleParam("run_speed");
        }
        // ������Ԃ̏ꍇ
        else if ("Walk" == state) {
           // �����I�u�W�F�N�g����v���C���[�̈ʒu�܂ł̃x�N�g���Ɉړ��̑������������x�N�g�����ʒu�����炷
           _position = _position + fromFallObject * _DoubleParam("walk_speed");
        }
        // �m�b�N�o�b�N��Ԃ̏ꍇ
        else if ("KnockBack" == state) {
           // �����I�u�W�F�N�g����v���C���[�̈ʒu�܂ł̃x�N�g���Ɉړ��̑������������x�N�g�����ʒu�����炷
           _position = _position + fromFallObject * 10.0;
        }
        // �����蔻��̌��ʂ𓖂����Ă��Ȃ��Ɛݒ�
        _collisionComponent->report().id(Collision::CollisionComponent::ReportId::None);
    }
}

void Player::HitCheckFromGatling() {
    // �����蔻�茋�ʃN���X�̎Q�Ƃ̎擾
    auto report = _collisionComponent->report();
    // �����蔻�茋�ʂ̊m�F
    if (report.id() == Collision::CollisionComponent::ReportId::HitFromGatling) {
        // �K�g�����O�Ɠ������Ă�����
        // �K�g�����O�̈ʒu���擾
        auto hitPos = _collisionComponent->hitPos();
        // �m�b�N�o�b�N����������̎擾
        auto knockBackVec = _position - hitPos;
        // ����������̂ō������Ȃ���
        auto [x, y, z] = knockBackVec.GetVec3();
        auto knockBackDelta = Vector4(x, 0.0, z);
        // �P�ʉ�����
        knockBackDelta.Normalized();
        // �m�b�N�o�b�N�ʂ̐ݒ�
        _knockBack = knockBackDelta * 10.0;
        // �_���[�W�ʕ��q�b�g�|�C���g�����炷
        _gameMain.playerHp(_gameMain.playerHp() - _collisionComponent->damage());
        // �m�b�N�o�b�N���Ă���Ɛݒ�
        _collisionComponent->knockBack(true);
        // �����蔻��̌��ʂ𓖂����Ă��Ȃ��Ɛݒ�
        _collisionComponent->report().id(Collision::CollisionComponent::ReportId::None);
        // �J�����̃Y�[�������Ȃ��Ɛݒ�
        _cameraComponent->SetZoom(false);
        // �m�b�N�o�b�N��Ԃ�
        _stateServer->GoToState("KnockBack");
    }
}

void Player::HitCheckFromFallObject() {
    // �����蔻�茋�ʃN���X�̎Q�Ƃ̎擾
    auto report = _collisionComponent->report();
    // �����蔻�茋�ʂ̊m�F
    if (report.id() == Collision::CollisionComponent::ReportId::HitFromFallObject) {
        // �������̗����I�u�W�F�N�g�Ɠ������Ă�����
        // �����I�u�W�F�N�g�̈ʒu���擾
        auto hitPos = _collisionComponent->hitPos();
        // �����I�u�W�F�N�g�̈ʒu�̐����𕪉�
        auto [hitX, hitY, hitZ] = hitPos.GetVec3();
        // �v���C���[�̈ʒu�̐����𕪉�
        auto [posX, posY, posZ] = _position.GetVec3();
        // ���������I�u�W�F�N�g���^��ɂ�������
        if (hitX == posX && posZ == hitZ) {
            // Y������݂����f���̉�]�p�x�̎擾
            auto rotateY = _rotation.GetY();
            // Y������݂����f���̉�]�}�g���N�X�쐬
            Matrix44 mat = Matrix44();
            mat.RotateY(rotateY, true);
            // ���f���������Ă�������̔��Ε����̃x�N�g������
            Vector4 vec = Vector4(0.0, 0.0, -1.0);
            // �P�ʉ�������
            vec.Normalized();
            // �m�b�N�o�b�N�̌����̃x�N�g����ݒ�
            auto knockBackDelta = vec * mat;
            // �m�b�N�o�b�N�ʂ̃x�N�g����ݒ�
            _knockBack = knockBackDelta * 10.0;
        }
        else {
            // �����I�u�W�F�N�g���^��ɂȂ��ꍇ
            // �m�b�N�o�b�N����������̎擾
            auto knockBackVec = _position - hitPos;
            // �������Ȃ���
            auto [x, y, z] = knockBackVec.GetVec3();
            auto knockBackDelta = Vector4(x, 0.0, z);
            // �P�ʉ�����
            knockBackDelta.Normalized();
            // �m�b�N�o�b�N�ʂ̃x�N�g����ݒ�
            _knockBack = knockBackDelta * 10.0;
        }
        // �_���[�W�ʕ��q�b�g�|�C���g�����炷
        _gameMain.playerHp(_gameMain.playerHp() - _collisionComponent->damage());
        // �m�b�N�o�b�N���Ă���Ɛݒ�
        _collisionComponent->knockBack(true);
        // �����蔻��̌��ʂ𓖂����Ă��Ȃ��Ɛݒ�
        _collisionComponent->report().id(Collision::CollisionComponent::ReportId::None);
        // �J�����̃Y�[�������Ȃ��Ɛݒ�
        _cameraComponent->SetZoom(false);
        // �m�b�N�o�b�N��Ԃ�
        _stateServer->GoToState("KnockBack");
    }
}

void Player::HitCheckFromLaser() {
    // �����蔻�茋�ʃN���X�̎Q�Ƃ̎擾
    auto report = _collisionComponent->report();
    // �����蔻�茋�ʂ̊m�F
    if (report.id() == Collision::CollisionComponent::ReportId::HitFromLaser) {
        // ���[�U�[�Ɠ������Ă�����
        // ���[�U�[�̈ʒu���擾
        auto hitPos = _collisionComponent->hitPos();
        // �m�b�N�o�b�N����������̎擾
        auto knockBackVec = _position - hitPos;
        // �������Ȃ���
        auto [x, y, z] = knockBackVec.GetVec3();
        auto knockBackDelta = Vector4(x, 0.0, z);
        // �P�ʉ�����
        knockBackDelta.Normalized();
        // �m�b�N�o�b�N�ʂ̃x�N�g����ݒ�
        _knockBack = knockBackDelta * 10.0;
        // �_���[�W�ʕ��q�b�g�|�C���g�����炷
        _gameMain.playerHp(_gameMain.playerHp() - _collisionComponent->damage());
        // �m�b�N�o�b�N���Ă���Ɛݒ�
        _collisionComponent->knockBack(true);
        // �����蔻��̌��ʂ𓖂����Ă��Ȃ��Ɛݒ�
        _collisionComponent->report().id(Collision::CollisionComponent::ReportId::None);
        // �J�����̃Y�[�������Ȃ��Ɛݒ�
        _cameraComponent->SetZoom(false);
        // �m�b�N�o�b�N��Ԃ�
        _stateServer->GoToState("KnockBack");
    }
}

void Player::HitCheckFromLargeEnemy() {
    // �����蔻�茋�ʃN���X�̎Q�Ƃ̎擾
    auto report = _collisionComponent->report();
    // �����蔻�茋�ʂ̊m�F
    if (report.id() == Collision::CollisionComponent::ReportId::HitFromLargeEnemy) {
        // ���[�W�G�l�~�[�Ɠ������Ă�����
        // ���[�W�G�l�~�[�̈ʒu���擾
        auto hitPos = _collisionComponent->hitPos();
        // �m�b�N�o�b�N����������̎擾
        auto knockBackVec = _position - hitPos;
        knockBackVec.Normalized();
        // �m�b�N�o�b�N�ʂ̃x�N�g����ݒ�
        _knockBack = knockBackVec * 10.0;
        // �_���[�W�ʕ��q�b�g�|�C���g�����炷
        _gameMain.playerHp(_gameMain.playerHp() - _collisionComponent->damage());
        // �m�b�N�o�b�N���Ă���Ɛݒ�
        _collisionComponent->knockBack(true);
        // �J�����̃Y�[�������Ȃ��Ɛݒ�
        _cameraComponent->SetZoom(false);
        // �m�b�N�o�b�N��Ԃ�
        _stateServer->GoToState("KnockBack");
    }
}

void Player::HitCheckFromPoorEnemy() {
   // �����蔻�茋�ʃN���X�̎Q�Ƃ̎擾
   auto report = _collisionComponent->report();
   // �����蔻�茋�ʂ̊m�F
   if (report.id() == Collision::CollisionComponent::ReportId::HitFromPoorEnemy) {
      // �G���G�Ɠ������Ă�����
      // �G���G�̈ʒu���擾
      auto hitPos = _collisionComponent->hitPos();
      // �m�b�N�o�b�N����������̎擾
      auto knockBackVec = _position - hitPos;
      knockBackVec.Normalized();
      // �m�b�N�o�b�N�ʂ̃x�N�g����ݒ�
      _knockBack = knockBackVec * 10.0;
      // �_���[�W�ʕ��q�b�g�|�C���g�����炷
      _gameMain.playerHp(_gameMain.playerHp() - _collisionComponent->damage());
      // �m�b�N�o�b�N���Ă���Ɛݒ�
      _collisionComponent->knockBack(true);
      // �J�����̃Y�[�������Ȃ��Ɛݒ�
      _cameraComponent->SetZoom(false);
      // �m�b�N�o�b�N��Ԃ�
      _stateServer->GoToState("KnockBack");
   }
}

void Player::WeakAttack() {
    // ���u��U���̒e�𐶐����ăI�u�W�F�N�g�T�[�o�[�֒ǉ�
    auto bullet = gameMain().objFactory().Create("Bullet");
    gameMain().objServer().Add(std::move(bullet));
}

void Player::StateBase::Update() {
   // ���G���Ԓ��̏ꍇ
   if (_owner._invincibleCnt > 0) {
      // ���G��Ԃɓ����Ă���̃t���[���J�E���g���̎擾
      auto cnt = _owner.gameMain().modeServer().frameCount() - _owner._invincibleModeCnt;
      // ����̃t���[�����o�߂�����
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
   // ���f���̕`�揈������
   _owner._modelAnimeComponent->Draw();
#ifdef _DEBUG
   /**
    * \brief double�^�̒l�𕶎���Ŏw�肵�A�l�Ǘ��N���X����擾����
    * \param paramName �l���w�肷�镶����
    * \return ������ɂ��w�肳�ꂽ�l
    */
   const auto _DoubleParam = [&](std::string paramName) {
      return _owner._collParam->GetDoubleParam(paramName);
   };
   // �v���C���[�̃J�v�Z���̈�ڂ̍��W�̐ݒ�
   auto pos1 = _owner._position + Vector4(0.0, _DoubleParam("ply_capsule_pos1"), 0.0);
   // �v���C���[�̃J�v�Z���̓�ڂ̍��W�̐ݒ�
   auto pos2 = _owner._position + Vector4(0.0, _DoubleParam("ply_capsule_pos2"), 0.0);
   // �v���C���[�̃J�v�Z���̔��a�̐ݒ�
   auto radian = static_cast<float>(_DoubleParam("ply_radius"));
   // �v���C���[�̃X�e�[�W�Ƃ̔���p�̐����̎n�_�̐ݒ�
   auto start = _owner._position + Vector4(0.0, 50.0, 0.0);
   // �v���C���[�̃X�e�[�W�Ƃ̔���p�̐����̏I�_�̐ݒ�
   auto end = _owner._position + Vector4(0.0, -10000.0, 0.0);
  // �v���C���[�̃J�v�Z���̕`��
   DrawCapsule3D(AppFrame::Math::ToDX(pos1), AppFrame::Math::ToDX(pos2),
      radian, 20, GetColor(0, 255, 0), GetColor(0, 0, 0), FALSE);
   // �v���C���[�̃X�e�[�W�Ƃ̔���p�̐����̕`��
   DrawLine3D(AppFrame::Math::ToDX(start), AppFrame::Math::ToDX(end), 
      AppFrame::Math::Utility::GetColorCode(0, 255, 255));
#endif
}

void Player::StateIdle::Enter() {
   _owner._stateCnt = _owner.gameMain().modeServer().frameCount();
   // ���f���̃A�j���[�V�����̐ݒ�
   _owner._modelAnimeComponent->ChangeAnime("stay", true, 
      _owner._param->GetDoubleParam("idle_animespeed"));
   // �I�u�W�F�N�g�������グ����Ɛݒ�
   _owner._isLift = true;
}

void Player::StateIdle::Input(InputManager& input) {
   /**
    * \brief int�^�̒l�𕶎���Ŏw�肵�A�l�Ǘ��N���X����擾����
    * \param paramName �l���w�肷�镶����
    * \return ������ɂ��w�肳�ꂽ�l
    */
   const auto _IntParam = [&](std::string paramName) {
      return _owner._param->GetIntParam(paramName);
   };
   // �f�b�h�]�[���̎擾
   auto [cameraSens, aimSens, deadZone] = _owner._gameMain.sensitivity();
   auto joypad = input.GetXJoypad();
   const auto DeadZoneRange = _IntParam("walk_dead_zone_range");
   // ���X�e�B�b�N�������Ă��邩�m�F
   // ���X�e�B�b�N���E�ɓ����Ă��ĕ����̓��͔͈͂��傫���ꍇ
   if (joypad.LeftStickX() > DeadZoneRange) {
      // �����Ԃ�
      _owner._stateServer->GoToState("Run");
   }
   // �����̓��͔͈͈ȉ��Ńf�b�h�]�[���ȏゾ�����ꍇ
   else if (joypad.LeftStickX() >= deadZone) {
      // ������Ԃ�
      _owner._stateServer->GoToState("Walk");
   }
   // ���X�e�B�b�N�����ɓ����Ă��ĕ����̓��͔͈͂�菬�����ꍇ
   if (joypad.LeftStickX() < -DeadZoneRange) {
      // �����Ԃ�
      _owner._stateServer->GoToState("Run");
   }
   // �����̓��͔͈͈ȏ�Ńf�b�h�]�[���ȉ��������ꍇ
   else if (joypad.LeftStickX() <= -deadZone) {
      // ������Ԃ�
      _owner._stateServer->GoToState("Walk");
   }
   // ���X�e�B�b�N����ɓ����Ă��ĕ����̓��͔͈͂��傫���ꍇ
   if (joypad.LeftStickY() > DeadZoneRange) {
      // �����Ԃ�
      _owner._stateServer->GoToState("Run");
   }
   // �����̓��͔͈͈ȉ��Ńf�b�h�]�[���ȏゾ�����ꍇ
   else if (joypad.LeftStickY() >= deadZone) {
      // ������Ԃ�
      _owner._stateServer->GoToState("Walk");
   }
   // ���X�e�B�b�N�����ɓ����Ă��ĕ����̓��͔͈͂�菬�����ꍇ
   if (joypad.LeftStickY() < -DeadZoneRange) {
      // �����Ԃ�
      _owner._stateServer->GoToState("Run");
   }
   // �����̓��͔͈͈ȉ��Ńf�b�h�]�[���ȉ��������ꍇ
   else if (joypad.LeftStickY() <= -deadZone) {
      // ������Ԃ�
      _owner._stateServer->GoToState("Walk");
   }
   // ���g���K�[��������Ă��邩�m�F
   if (joypad.LeftTrigger() >= 20) {
      // ���g���K�[��������Ă�����I�u�W�F�N�g�������グ��͈͂ɂ��邩�m�F
      _owner.HitCheckFromFallObjectRange();
   }
   // ���g���K�[��������Ă��Ȃ��Ƃ���LB�{�^����������Ă��邩�m�F
   else if (joypad.LBClick()) {
      // ���g���K�[��������Ă��Ȃ��Ƃ���LB�{�^����������Ă����牓�u��U���ˌ�������Ԃ�
      _owner._stateServer->GoToState("WeakShootReady");
      // �J�����̃Y�[����������
      _owner._cameraComponent->SetZoom(true);
   }
   // X�{�^����������Ă��āA���u��U���̎c��e�������u��U���̍ő�e�������������瑕�U��Ԃ�
   if (joypad.XClick() && _owner._gameMain.playerBullet() < _IntParam("max_bullet")) {
      _owner._stateServer->GoToState("Reload");
   }
   // Y�{�^����������Ă��āA�|�[�V�����̐���0���傫���q�b�g�|�C���g���ő�q�b�g�|�C���g������������񕜏�Ԃ�
   if (joypad.YClick() && _owner._gameMain.playerPortion() > 0 &&
      _owner._gameMain.playerHp() < _owner._param->GetDoubleParam("max_hp")) {
      _owner._stateServer->GoToState("Recovery");
   }
}

void Player::StateIdle::Update() {
   StateBase::Update();
   _owner.Rotate();
   // �����蔻�菈�����s���N���X�Ńv���C���[���K�g�����O�Ɠ������Ă��邩�m�F
   _owner._collisionComponent->GatlingFromPlayer();
   // �����蔻�菈�����s���N���X�Ńv���C���[�����[�U�[�Ɠ������Ă��邩�m�F
   _owner._collisionComponent->PlayerFromLaser();
   // ���[�W�G�l�~�[�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromLargeEnemy();
   // �G���G�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromPoorEnemy();
   // �������̗����I�u�W�F�N�g�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromFallObject();
   // �K�g�����O�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromGatling();
   // ���[�U�[�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromLaser();
   // �N�[���^�C���̍X�V
   --_owner._coolTime;
   // ���G���ԍX�V
   --_owner._invincibleCnt;
}

void Player::StateWalk::Enter() {
   _owner._stateCnt = _owner.gameMain().modeServer().frameCount();
   // �I�u�W�F�N�g�������グ����Ɛݒ�
   _owner._isLift = true;
   // ���f���̃A�j���[�V�����̐ݒ�
   _owner._modelAnimeComponent->ChangeAnime("walk", true,
      _owner._param->GetDoubleParam("walk_animespeed"));
}

void Player::StateWalk::Input(InputManager& input) {
   /**
    * \brief int�^�̒l�𕶎���Ŏw�肵�A�l�Ǘ��N���X����擾����
    * \param paramName �l���w�肷�镶����
    * \return ������ɂ��w�肳�ꂽ�l
    */
   const auto _IntParam = [&](std::string paramName) {
      return _owner._param->GetIntParam(paramName);
   };
   /**
    * \brief double�^�̒l�𕶎���Ŏw�肵�A�l�Ǘ��N���X����擾����
    * \param paramName �l���w�肷�镶����
    * \return ������ɂ��w�肳�ꂽ�l
    */
   const auto _DoubleParam = [&](std::string paramName) {
      return _owner._param->GetDoubleParam(paramName);
   };
   // �ҋ@��Ԃ̗����I�u�W�F�N�g�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromIdleFallObject("Walk");
   // �f�b�h�]�[���̎擾
   auto [cameraSens, aimSens, deadZone] = _owner._gameMain.sensitivity();
   // �ړ����Ă��邩�̃t���O���쐬���ď����ł͂��Ă��Ȃ��Ɛݒ�
   auto moved = false;
   // �J�������璍���_�ւ̕����P�ʃx�N�g�������Ƃ߂�
   auto camForward = _owner._cameraComponent->GetForward();
   auto [x, y, z] = camForward.GetVec3();
   // �������Ȃ���
   _owner._direction = Vector4(x, 0.0, z);
   // �ړ��ʂ̃x�N�g����������
   _owner._moved = Vector4();
   const auto DeadZoneRange = _IntParam("walk_dead_zone_range");
   // ���X�e�B�b�N���E�ɓ����Ă��邩�m�F
   // ���X�e�B�b�N���E�ɓ����Ă��ăX�e�B�b�N�̓��͔͈͂��f�b�h�]�[���ȏ�ŕ����̓��͔͈͈ȉ��������ꍇ
   if (input.GetXJoypad().LeftStickX() >= deadZone && input.GetXJoypad().LeftStickX() <= DeadZoneRange) {
      // �ړ��ʂ̃x�N�g���ɃJ�������璍���_�ւ̕����P�ʃx�N�g����90�x��]�������x�N�g���𑫂�
      _owner._moved = _owner._moved + (_owner._direction * _owner._rightRotation);
      // �ړ����Ă��邩�̃t���O���ړ����Ă���Ɛݒ�
      moved = true;
   }
   // ���X�e�B�b�N���E�ɓ����Ă��ăX�e�B�b�N�̓��͔͈͂������̓��͔͈͂��傫���ꍇ
   else if (input.GetXJoypad().LeftStickX() > DeadZoneRange) {
      // �ړ��ʂ̃x�N�g���ɃJ�������璍���_�ւ̕����P�ʃx�N�g����90�x��]�������x�N�g���𑫂�
      _owner._moved = _owner._moved + (_owner._direction * _owner._rightRotation);
      // �����Ԃ�
      _owner._stateServer->GoToState("Run");
      // �ړ����Ă��邩�̃t���O���ړ����Ă���Ɛݒ�
      moved = true;
   }
   // ���X�e�B�b�N�����ɓ����Ă��邩�m�F
   // ���X�e�B�b�N�����ɓ����Ă��ăX�e�B�b�N�̓��͔͈͂��f�b�h�]�[���ȉ��ŕ����̓��͔͈͈ȏゾ�����ꍇ
   if (input.GetXJoypad().LeftStickX() <= -deadZone && input.GetXJoypad().LeftStickX() >= -DeadZoneRange) {
      // �ړ��ʂ̃x�N�g���ɃJ�������璍���_�ւ̕����P�ʃx�N�g����-90�x��]�������x�N�g���𑫂�
      _owner._moved = _owner._moved + (_owner._direction * _owner._leftRotation);
      // �ړ����Ă��邩�̃t���O���ړ����Ă���Ɛݒ�
      moved = true;
   }
   // ���X�e�B�b�N�����ɓ����Ă��ăX�e�B�b�N�̓��͔͈͂������̓��͔͈͂�菬�����ꍇ
   else if (input.GetXJoypad().LeftStickX() < -DeadZoneRange) {
      // �ړ��ʂ̃x�N�g���ɃJ�������璍���_�ւ̕����P�ʃx�N�g����-90�x��]�������x�N�g���𑫂�
      _owner._moved = _owner._moved + (_owner._direction * _owner._leftRotation);
      // �����Ԃ�
      _owner._stateServer->GoToState("Run");
      // �ړ����Ă��邩�̃t���O���ړ����Ă���Ɛݒ�
      moved = true;
   }
   // ���X�e�B�b�N����ɓ����Ă��邩�m�F
   // ���X�e�B�b�N����ɓ����Ă��ăX�e�B�b�N�̓��͔͈͂��f�b�h�]�[���ȏ�ŕ����̓��͔͈͈ȉ��������ꍇ
   if (input.GetXJoypad().LeftStickY() >= deadZone && input.GetXJoypad().LeftStickY() <= DeadZoneRange) {
      // �ړ��ʂ̃x�N�g���ɃJ�������璍���_�ւ̕����P�ʃx�N�g���𑫂�
      _owner._moved = _owner._moved + _owner._direction;
      // �ړ����Ă��邩�̃t���O���ړ����Ă���Ɛݒ�
      moved = true;
   }
   // ���X�e�B�b�N����ɓ����Ă��ăX�e�B�b�N�̓��͔͈͂������̓��͔͈͂��傫���ꍇ
   else if (input.GetXJoypad().LeftStickY() > DeadZoneRange) {
      // �ړ��ʂ̃x�N�g���ɃJ�������璍���_�ւ̕����P�ʃx�N�g���𑫂�
      _owner._moved = _owner._moved + _owner._direction;
      // �����Ԃ�
      _owner._stateServer->GoToState("Run");
      // �ړ����Ă��邩�̃t���O���ړ����Ă���Ɛݒ�
      moved = true;
   }
   // ���X�e�B�b�N�����ɓ����Ă��邩�m�F
   // ���X�e�B�b�N�����ɓ����Ă��ăX�e�B�b�N�̓��͔͈͂��f�b�h�]�[���ȉ��ŕ����̓��͔͈͈ȏゾ�����ꍇ
   if (input.GetXJoypad().LeftStickY() <= -deadZone && input.GetXJoypad().LeftStickY() >= -DeadZoneRange) {
      // �ړ��ʂ̃x�N�g���ɃJ�������璍���_�ւ̕����P�ʃx�N�g����180�x��]�������x�N�g���𑫂�
      _owner._moved = _owner._moved + (_owner._direction * _owner._backRotation);
      // �ړ����Ă��邩�̃t���O���ړ����Ă���Ɛݒ�
      moved = true;
   }
   // ���X�e�B�b�N�����ɓ����Ă��ăX�e�B�b�N�̓��͔͈͂������̓��͔͈͂�菬�����ꍇ
   else if (input.GetXJoypad().LeftStickY() < -DeadZoneRange) {
      // �ړ��ʂ̃x�N�g���ɃJ�������璍���_�ւ̕����P�ʃx�N�g����180�x��]�������x�N�g���𑫂�
      _owner._moved = _owner._moved + (_owner._direction * _owner._backRotation);
      // �����Ԃ�
      _owner._stateServer->GoToState("Run");
      // �ړ����Ă��邩�̃t���O���ړ����Ă���Ɛݒ�
      moved = true;
   }
   // ���g���K�[��������Ă��邩�m�F
   if (input.GetXJoypad().LeftTrigger() >= 20) {
      // ���g���K�[��������Ă�����I�u�W�F�N�g�������グ��͈͂ɂ��邩�m�F
      _owner.HitCheckFromFallObjectRange();
   }
   // ���g���K�[��������Ă��Ȃ��Ƃ���LB�{�^����������Ă��邩�m�F
   else if (input.GetXJoypad().LBClick()) {
      // ���g���K�[��������Ă��Ȃ��Ƃ���LB�{�^����������Ă����牓�u��U���ˌ�������Ԃ�
      _owner._stateServer->GoToState("WeakShootReady");
      // �J�����̃Y�[����������
      _owner._cameraComponent->SetZoom(true);
   }
   // X�{�^����������Ă��āA���u��U���̎c��e�������u��U���̍ő�e�������������瑕�U��Ԃ�
   if (input.GetXJoypad().XClick() && _owner._gameMain.playerBullet() < _IntParam("max_bullet")) {
      _owner._stateServer->GoToState("Reload");
   }
   // Y�{�^����������Ă��āA�|�[�V�����̐���0���傫���q�b�g�|�C���g���ő�q�b�g�|�C���g������������񕜏�Ԃ�
   if (input.GetXJoypad().YClick() && _owner._gameMain.playerPortion() > 0 && 
      _owner._gameMain.playerHp() < _DoubleParam("max_hp")) {
      _owner._stateServer->GoToState("Recovery");
   }
   // �ړ����Ă��邩�̃t���O���ړ����Ă��Ȃ��ƂȂ��Ă��邩�m�F
   if (!moved) {
      // �ړ����Ă��Ȃ��ƂȂ��Ă����炱�̏�Ԃ���Ԃ̃��X�g���珜�O����
      _owner._stateServer->GoToState("Idle");
   }
   // �ړ����Ă���ꍇ
   else {
      // �ړ��ʂ̃x�N�g����P�ʉ�����
      _owner._moved.Normalized();
      // �ړ�������������݌����Ă�������Ɣ�ׂĐ^�t�ɋ߂����m�F
      if (_owner.GetForward().Dot(_owner._moved) < -0.5) {
         // �ړ�������������݌����Ă�������Ɣ�ׂĐ^�t�ɋ߂��ꍇ
         // 1�x��]������}�g���N�X���쐬
         Matrix44 mat;
         mat.RotateY(1.0, true);
         // ���������������̃x�N�g�����ړ������̃x�N�g����1�x��]�����Ċ���̐^�t�̂Ƃ��̑傫�����|���Đݒ�
         _owner._rotateDir = _owner._moved * mat * 20.0;
      }
      // �^�t�ɋ߂��Ȃ��ꍇ
      else {
         // ���������������̃x�N�g�����ړ������̃x�N�g������]�����߂�Ƃ��̊���̑傫�����|���Đݒ�
         _owner._rotateDir = _owner._moved * _DoubleParam("rotate_rate");
      }
      // �ړ��ʂ̃x�N�g����ݒ�
      _owner._moved = _owner._moved * _DoubleParam("rotate_rate");
   }
}

void Player::StateWalk::Update() {
   StateBase::Update();
   _owner.Move(_owner._moved);
   _owner.Rotate();
   // �����蔻�菈�����s���N���X�Ńv���C���[���K�g�����O�Ɠ������Ă��邩�m�F
   _owner._collisionComponent->GatlingFromPlayer();
   // �����蔻�菈�����s���N���X�Ńv���C���[�����[�U�[�Ɠ������Ă��邩�m�F
   _owner._collisionComponent->PlayerFromLaser();
   // �������̗����I�u�W�F�N�g�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromFallObject();
   // ���[�W�G�l�~�[�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromLargeEnemy();
   // �G���G�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromPoorEnemy();
   // �K�g�����O�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromGatling();
   // ���[�U�[�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromLaser();
   // �N�[���^�C���̍X�V
   --_owner._coolTime;
   // ���G���ԍX�V
   --_owner._invincibleCnt;
}

void Player::StateRun::Enter() {
   // �I�u�W�F�N�g�������グ����Ɛݒ�
   _owner._isLift = true;
   // ���f���̃A�j���[�V�����̐ݒ�
   _owner._modelAnimeComponent->ChangeAnime("run_02", true,
      _owner._param->GetDoubleParam("run_animespeed"));
   // �Q�[���̃t���[���J�E���g�̎擾
   _owner._stateCnt = _owner.gameMain().modeServer().frameCount();
   auto count = _owner.gameMain().modeServer().frameCount();
   // ���̏�Ԃ֓��������̃Q�[���̃t���[���J�E���g��
   _footCnt = count;
}

void Player::StateRun::Input(InputManager& input) {
   /**
    * \brief int�^�̒l�𕶎���Ŏw�肵�A�l�Ǘ��N���X����擾����
    * \param paramName �l���w�肷�镶����
    * \return ������ɂ��w�肳�ꂽ�l
    */
   const auto _IntParam = [&](std::string paramName) {
      return _owner._param->GetIntParam(paramName);
   };
   /**
    * \brief double�^�̒l�𕶎���Ŏw�肵�A�l�Ǘ��N���X����擾����
    * \param paramName �l���w�肷�镶����
    * \return ������ɂ��w�肳�ꂽ�l
    */
   const auto _DoubleParam = [&](std::string paramName) {
      return _owner._param->GetDoubleParam(paramName);
   };
   // �ҋ@��Ԃ̗����I�u�W�F�N�g�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromIdleFallObject("Run");
   // ���X�e�B�b�N�̓��͂𐔃t���[���҂�
   if ((_owner.gameMain().modeServer().frameCount() - _footCnt) % _IntParam("wait_frame") == 0) {
      // �f�b�h�]�[���̎擾
      auto [cameraSens, aimSens, deadZone] = _owner._gameMain.sensitivity();
      // �ړ����Ă��邩�̃t���O���쐬���ď����ł͂��Ă��Ȃ��Ɛݒ�
      auto moved = false;
      // �J�������璍���_�ւ̕����P�ʃx�N�g�������Ƃ߂�
      auto camForward = _owner._cameraComponent->GetForward();
      auto [x, y, z] = camForward.GetVec3();
      // �������Ȃ���
      _owner._direction = Vector4(x, 0.0, z);
      // �ړ��ʂ̃x�N�g����������
      _owner._moved = Vector4();
      // ���X�e�B�b�N���E�ɓ����Ă��邩�m�F
      // ���X�e�B�b�N���E�ɓ����Ă��ĕ����̓��͔͈͂��傫���ꍇ
      const auto DeadZoneRange = _IntParam("walk_dead_zone_range");
      if (input.GetXJoypad().LeftStickX() > DeadZoneRange) {
         // �ړ��ʂ̃x�N�g���ɃJ�������璍���_�ւ̕����P�ʃx�N�g����90�x��]�������x�N�g���𑫂�
         _owner._moved = _owner._moved + (_owner._direction * _owner._rightRotation);
         // �ړ����Ă��邩�̃t���O���ړ����Ă���Ɛݒ�
         moved = true;
      }
      // �����̓��͔͈͓��Ńf�b�h�]�[���ȏ�̏ꍇ
      else if (input.GetXJoypad().LeftStickX() >= deadZone) {
         // �ړ��ʂ̃x�N�g���ɃJ�������璍���_�ւ̕����P�ʃx�N�g����90�x��]�������x�N�g���𑫂�
         _owner._moved = _owner._moved + (_owner._direction * _owner._rightRotation);
         // �ړ����Ă��邩�̃t���O���ړ����Ă���Ɛݒ�
         moved = true;
         // ���X�e�B�b�N�̏c��������͈̔͊O�������ꍇ
         if (!(input.GetXJoypad().LeftStickY() > DeadZoneRange || input.GetXJoypad().LeftStickY() < -DeadZoneRange)) {
            // �ҋ@��Ԃ�
            _owner._stateServer->GoToState("Idle");
         }
      }
      // ���X�e�B�b�N�����ɓ����Ă��邩�m�F
      // ���X�e�B�b�N�����ɓ����Ă��ĕ����̓��͔͈͂�菬�����ꍇ
      if (input.GetXJoypad().LeftStickX() < -DeadZoneRange) {
         // �ړ��ʂ̃x�N�g���ɃJ�������璍���_�ւ̕����P�ʃx�N�g����-90�x��]�������x�N�g���𑫂�
         _owner._moved = _owner._moved + (_owner._direction * _owner._leftRotation);
         // �ړ����Ă��邩�̃t���O���ړ����Ă���Ɛݒ�
         moved = true;
      }
      // �����̓��͔͈͓��Ńf�b�h�]�[���ȉ��̏ꍇ
      else if (input.GetXJoypad().LeftStickX() <= -deadZone) {
         // �ړ��ʂ̃x�N�g���ɃJ�������璍���_�ւ̕����P�ʃx�N�g����-90�x��]�������x�N�g���𑫂�
         _owner._moved = _owner._moved + (_owner._direction * _owner._leftRotation);
         // �ړ����Ă��邩�̃t���O���ړ����Ă���Ɛݒ�
         moved = true;
         // ���X�e�B�b�N�̏c��������͈̔͊O�������ꍇ
         if (!(input.GetXJoypad().LeftStickY() > DeadZoneRange || input.GetXJoypad().LeftStickY() < -DeadZoneRange)) {
            // �ҋ@��Ԃ�
            _owner._stateServer->GoToState("Idle");
         }
      }
      // ���X�e�B�b�N����ɓ����Ă��邩�m�F
      // ���X�e�B�b�N����ɓ����Ă��ĕ����̓��͔͈͂��傫���ꍇ
      if (input.GetXJoypad().LeftStickY() > DeadZoneRange) {
         // �ړ��ʂ̃x�N�g���ɃJ�������璍���_�ւ̕����P�ʃx�N�g���𑫂�
         _owner._moved = _owner._moved + _owner._direction;
         // �ړ����Ă��邩�̃t���O���ړ����Ă���Ɛݒ�
         moved = true;
      }
      // �����̓��͔͈͓��Ńf�b�h�]�[���ȏ�̏ꍇ
      else if (input.GetXJoypad().LeftStickY() >= deadZone) {
         // �ړ��ʂ̃x�N�g���ɃJ�������璍���_�ւ̕����P�ʃx�N�g���𑫂�
         _owner._moved = _owner._moved + _owner._direction;
         // �ړ����Ă��邩�̃t���O���ړ����Ă���Ɛݒ�
         moved = true;
         // ���X�e�B�b�N�̉���������͈̔͊O�������ꍇ
         if (!(input.GetXJoypad().LeftStickX() > DeadZoneRange || input.GetXJoypad().LeftStickX() < -DeadZoneRange)) {
            // �ҋ@��Ԃ�
            _owner._stateServer->GoToState("Idle");
         }
      }
      // ���X�e�B�b�N�����ɓ����Ă��邩�m�F
      // ���X�e�B�b�N�����ɓ����Ă��ĕ����̓��͔͈͂�菬�����ꍇ
      if (input.GetXJoypad().LeftStickY() < -DeadZoneRange) {
         // �ړ��ʂ̃x�N�g���ɃJ�������璍���_�ւ̕����P�ʃx�N�g����180�x��]�������x�N�g���𑫂�
         _owner._moved = _owner._moved + (_owner._direction * _owner._backRotation);
         // �ړ����Ă��邩�̃t���O���ړ����Ă���Ɛݒ�
         moved = true;
      }
      // �����̓��͔͈͓��Ńf�b�h�]�[���ȉ��̏ꍇ
      else if (input.GetXJoypad().LeftStickY() <= -deadZone) {
         // �ړ��ʂ̃x�N�g���ɃJ�������璍���_�ւ̕����P�ʃx�N�g����180�x��]�������x�N�g���𑫂�
         _owner._moved = _owner._moved + (_owner._direction * _owner._backRotation);
         // �ړ����Ă��邩�̃t���O���ړ����Ă���Ɛݒ�
         moved = true;
         // ���X�e�B�b�N�̉���������͈̔͊O�������ꍇ
         if (!(input.GetXJoypad().LeftStickX() > DeadZoneRange || input.GetXJoypad().LeftStickX() < -DeadZoneRange)) {
            // �ҋ@��Ԃ�
            _owner._stateServer->GoToState("Idle");
         }
      }
      // �ړ����Ă��邩�̃t���O���ړ����Ă��Ȃ��ƂȂ��Ă��邩�m�F
      if (!moved) {
         // �ړ����Ă��Ȃ��ƂȂ��Ă����炱�̏�Ԃ���Ԃ̃��X�g���珜�O����
         _owner._stateServer->GoToState("Idle");
      }
      // �ړ����Ă���ꍇ
      else {
         // �ړ��ʂ̃x�N�g����P�ʉ�����
         _owner._moved.Normalized();
         // �ړ�������������݌����Ă�������Ɣ�ׂĐ^�t�ɋ߂����m�F
         if (_owner.GetForward().Dot(_owner._moved) < -0.5) {
            // �ړ�������������݌����Ă�������Ɣ�ׂĐ^�t�ɋ߂��ꍇ
            // 1�x��]������}�g���N�X���쐬
            Matrix44 mat;
            mat.RotateY(1.0, true);
            // ���������������̃x�N�g�����ړ������̃x�N�g����1�x��]�����Ċ���̐^�t�̂Ƃ��̑傫�����|���Đݒ�
            _owner._rotateDir = _owner._moved * mat * 20.0;
         }
         // �^�t�ɋ߂��Ȃ��ꍇ
         else {
            // ���������������̃x�N�g�����ړ������̃x�N�g������]�����߂�Ƃ��̊���̑傫�����|���Đݒ�
            _owner._rotateDir = _owner._moved * _DoubleParam("rotate_rate");
         }
         // �ړ��ʂ̃x�N�g����ݒ�
         _owner._moved = _owner._moved * _DoubleParam("run_speed");
      }
   }
   // ���g���K�[��������Ă��邩�m�F
   if (input.GetXJoypad().LeftTrigger() >= 20) {
      // ���g���K�[��������Ă�����I�u�W�F�N�g�������グ��͈͂ɂ��邩�m�F
      _owner.HitCheckFromFallObjectRange();
   }
   // ���g���K�[��������Ă��Ȃ��Ƃ���LB�{�^����������Ă��邩�m�F
   else if (input.GetXJoypad().LBClick()) {
      // ���g���K�[��������Ă��Ȃ��Ƃ���LB�{�^����������Ă����牓�u��U���ˌ�������Ԃ�
      _owner._stateServer->GoToState("WeakShootReady");
      // �J�����̃Y�[����������
      _owner._cameraComponent->SetZoom(true);
   }
   // X�{�^����������Ă��āA���u��U���̎c��e�������u��U���̍ő�e�������������瑕�U��Ԃ�
   if (input.GetXJoypad().XClick() && _owner._gameMain.playerBullet() < _IntParam("max_bullet")) {
      _owner._stateServer->GoToState("Reload");
   }
   // Y�{�^����������Ă��āA�|�[�V�����̐���0���傫���q�b�g�|�C���g���ő�q�b�g�|�C���g������������񕜏�Ԃ�
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
   // �����蔻�菈�����s���N���X�Ńv���C���[���K�g�����O�Ɠ������Ă��邩�m�F
   _owner._collisionComponent->GatlingFromPlayer();
   // �����蔻�菈�����s���N���X�Ńv���C���[�����[�U�[�Ɠ������Ă��邩�m�F
   _owner._collisionComponent->PlayerFromLaser();
   // �������̗����I�u�W�F�N�g�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromFallObject();
   // ���[�W�G�l�~�[�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromLargeEnemy();
   // �G���G�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromPoorEnemy();
   // �K�g�����O�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromGatling();
   // ���[�U�[�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromLaser();
   // �N�[���^�C���̍X�V
   --_owner._coolTime;
   // ���G���ԍX�V
   --_owner._invincibleCnt;
}

void Player::StateShootReady::Enter() {
   _owner._stateCnt = _owner.gameMain().modeServer().frameCount();
   // ���f���̃A�j���[�V�����̐ݒ�
   _owner._modelAnimeComponent->ChangeAnime("H_attack_pose_move", true,
      _owner._param->GetDoubleParam("shootready_animespeed"));
   // �炷�T�E���h�̐ݒ�
   _owner.GetSoundComponent().Play("PlayerShootReady");
   _owner.GetSoundComponent().Play("PlayerObjectUpVoice");
   // �A�j���[�V�����͕ς��Ă��Ȃ��Ɛݒ�
   _changeAnim = false;
   // �G�C�����Ɛݒ�
   _owner._isAim = true;
   // �I�u�W�F�N�g�������グ���Ȃ��Ɛݒ�
   _owner._isLift = false;
   // �I�u�W�F�N�g��łĂȂ��Ɛݒ�
   _owner._objectShoot = false;
}

void Player::StateShootReady::Input(InputManager& input) {
   // RB�{�^����������ăI�u�W�F�N�g��łĂ�ꍇ�ҋ@��Ԃ�
   if (input.GetXJoypad().RBClick() && _owner._objectShoot) {
      // �ҋ@��Ԃ�
      _owner._stateServer->GoToState("Idle");
      // �炷�T�E���h�̐ݒ�
      _owner.GetSoundComponent().Play("PlayerShoot");
      _owner.GetSoundComponent().Play("PlayerObjectShootVoice");
      // ���f���̃A�j���[�V�����̐ݒ�
      _owner._modelAnimeComponent->ChangeAnime("H_attack_attack",false, 
         _owner._param->GetDoubleParam("shoot_animespeed"));
      // �J�����̃Y�[�������Ȃ��Ɛݒ�
      _owner._cameraComponent->SetZoom(false);
   }
}

void Player::StateShootReady::Update() {
   StateBase::Update();
   // �ŏ��̃A�j���[�V������1��Đ����Ă��ăA�j���[�V������ς��Ă��Ȃ��ꍇ���̃A�j���[�V������ݒ�
   if (_owner._modelAnimeComponent->repeatedCount() >= 1 && !_changeAnim) {
      // ���f���̃A�j���[�V�����̐ݒ�
      _owner._modelAnimeComponent->ChangeAnime("H_attack_pose_loop", true,
         _owner._param->GetDoubleParam("shootready_animespeed"));
      // �A�j���[�V������ς����Ɛݒ�
      _changeAnim = true;
   }
   _owner.ShootRotate();
   // �����蔻�菈�����s���N���X�Ńv���C���[���K�g�����O�Ɠ������Ă��邩�m�F
   _owner._collisionComponent->GatlingFromPlayer();
   // �����蔻�菈�����s���N���X�Ńv���C���[�����[�U�[�Ɠ������Ă��邩�m�F
   _owner._collisionComponent->PlayerFromLaser();
   // ���[�W�G�l�~�[�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromLargeEnemy();
   // �G���G�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromPoorEnemy();
   // �������̗����I�u�W�F�N�g�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromFallObject();
   // �K�g�����O�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromGatling();
   // ���[�U�[�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromLaser();
   // �N�[���^�C���̍X�V
   --_owner._coolTime;
   // ���G���ԍX�V
   --_owner._invincibleCnt;
}

void Player::StateShootReady::Exit() {
   // �����蔻��̌��ʂ𓖂����Ă��Ȃ��Ɛݒ�
   _owner._collisionComponent->report().id(Collision::CollisionComponent::ReportId::None);
   // �G�C��������Ȃ��Ɛݒ�
   _owner._isAim = false;
}

void Player::StateKnockBack::Enter() {
   _owner._stateCnt = _owner.gameMain().modeServer().frameCount();
   // ���f���̃A�j���[�V�����̐ݒ�
   _owner.modelAnimeComponent().ChangeAnime("damaged", false,
      _owner._param->GetDoubleParam("knockback_animespeed"));
   _owner.GetSoundComponent().Play("PlayerDamageVoice");
   // �m�b�N�o�b�N���鎞�Ԃ̐ݒ�
   _owner._freezeTime = 30;
   // �I�u�W�F�N�g�������グ���Ȃ��Ɛݒ�
   _owner._isLift = false;
}

void Player::StateKnockBack::Update() {
   // �ҋ@��Ԃ̗����I�u�W�F�N�g�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromIdleFallObject("KnockBack");
   // �m�b�N�o�b�N���鎞�Ԓ����m�F
   if (_owner._freezeTime > 0) {
      // �m�b�N�o�b�N����
      _owner.Move(_owner._knockBack);
      // �m�b�N�o�b�N�̌����̔��΂̌����ւ̊p�x���擾���A�����ɐݒ肷��
      auto [x, y, z] = _owner._knockBack.GetVec3();
      auto radius = std::atan2(-x, -z);
      _owner._rotation.SetY(AppFrame::Math::Utility::RadianToDegree(radius));
      // �m�b�N�o�b�N���Ԃ̍X�V
      --_owner._freezeTime;
      return;
   }
   // �q�b�g�|�C���g��0�ȉ��������ꍇ���S��Ԃ�
   if (_owner._gameMain.playerHp() <= 0) {
      _owner._stateServer->GoToState("Die");
   }
   // �q�b�g�|�C���g��0�ȉ�����Ȃ������ꍇ
   else {
      // ���G���Ԃ̐ݒ�
      _owner._invincibleCnt = _owner._param->GetIntParam("invincible_frame");
      // ���G��Ԃɓ��������̃��[�h�T�[�o�[�̃t���[���J�E���g���̐ݒ�
      _owner._invincibleModeCnt = _owner.gameMain().modeServer().frameCount();
      // �m�b�N�o�b�N���Ă��Ȃ��Ɛݒ�
      _owner.collisionComponent().knockBack(false);
      // �����蔻��̌��ʂ𓖂����Ă��Ȃ��Ɛݒ�
      _owner.collisionComponent().report().id(Collision::CollisionComponent::ReportId::None);
      // �ҋ@��Ԃ�
      _owner._stateServer->GoToState("Idle");
   }
   // �N�[���^�C���̍X�V
   --_owner._coolTime;
}

void Player::StateDie::Enter() {
   // ���f���̃A�j���[�V�����̐ݒ�
   _owner.modelAnimeComponent().ChangeAnime("dawn", false);
   // ���S��ԂɂȂ��Ă���Q�[���I�[�o�[�܂ł̃t���[������ݒ�
   _timeOver = 60 * 2;
   // ���S���[�V�������Ɛݒ�
   _owner._isDeadMotion = true;
   // �I�u�W�F�N�g�������グ���Ȃ��Ɛݒ�
   _owner._isLift = false;
}

void Player::StateDie::Update() {
   // �Q�[���I�[�o�[�܂ł̃t���[�������c���Ă����猸�炷
   if (_timeOver > 0) {
      --_timeOver;
   }
   // �Q�[���I�[�o�[�܂ł̃t���[�������������ꍇ
   else {
      auto& soundComponent = _owner.GetSoundComponent();
      soundComponent.Stop("PoorBattleBgm");
      soundComponent.Stop("BossBattleBgm");
      soundComponent.Stop("TutorialBgm");
      // ���[�h�T�[�o�[�ɃQ�[���I�[�o�[���[�h��}��
      _owner.gameMain().modeServer().PushBack("MissionFailed");
   }
}

void Player::StateWeakShootReady::Enter() {
   _owner._stateCnt = _owner.gameMain().modeServer().frameCount();
   // ���f���̃A�j���[�V�����̐ݒ�
   _owner._modelAnimeComponent->ChangeAnime("L_attack_pose_loop", true);
   // �炷�T�E���h�̐ݒ�
   _owner.GetSoundComponent().Play("WeakShootReady");
   // �G�C�����Ɛݒ�
   _owner._isAim = true;
   // �I�u�W�F�N�g�������グ���Ȃ��Ɛݒ�
   _owner._isLift = false;
}

void Player::StateWeakShootReady::Input(InputManager& input) {
   // RB�{�^����������Ă��ăN�[���^�C�����Ȃ��A���u��U���̎c��e�������邩�m�F
   if (input.GetXJoypad().RBClick() && _owner._coolTime <= 0 && _owner._gameMain.playerBullet() > 0) {
      // RB�{�^����������Ă��ăN�[���^�C�����Ȃ��A���u��U���̎c��e�����������ꍇ
      // ���u��U������
      _owner.WeakAttack();
      // ���f���̃A�j���[�V�����̐ݒ�
      _owner._modelAnimeComponent->ChangeAnime("L_attack_attack", false, 
         _owner._param->GetDoubleParam("shoot_animespeed"));
      // �炷�T�E���h�̐ݒ�
      _owner.GetSoundComponent().Play("PlayerShoot");
      // ���u��U���̎c��e�������炷
      _owner._gameMain.playerBullet(_owner._gameMain.playerBullet() - 1);
      // �N�[���^�C���̐ݒ�
      _owner._coolTime = _owner._param->GetIntParam("cooltime");
   }
   // LB�{�^���������ꂽ��ҋ@��Ԃ�
   if (input.GetXJoypad().LBClick()) {
      _owner._stateServer->GoToState("Idle");
      // �J�����̃Y�[�������Ȃ��Ɛݒ�
      _owner._cameraComponent->SetZoom(false);
   }
}

void Player::StateWeakShootReady::Update() {
   StateBase::Update();
   _owner.ShootRotate();
   // �����蔻�菈�����s���N���X�Ńv���C���[���K�g�����O�Ɠ������Ă��邩�m�F
   _owner._collisionComponent->GatlingFromPlayer();
   // �����蔻�菈�����s���N���X�Ńv���C���[�����[�U�[�Ɠ������Ă��邩�m�F
   _owner._collisionComponent->PlayerFromLaser();
   // ���[�W�G�l�~�[�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromLargeEnemy();
   // �G���G�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromPoorEnemy();
   // �������̗����I�u�W�F�N�g�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromFallObject();
   // �K�g�����O�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromGatling();
   // ���[�U�[�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromLaser();
   // �N�[���^�C���̍X�V
   --_owner._coolTime;
   // ���G���ԍX�V
   --_owner._invincibleCnt;
}

void Player::StateWeakShootReady::Exit() {
   // �G�C��������Ȃ��Ɛݒ�
   _owner._isAim = false;
}

void Player::StateReload::Enter() {
   _owner._stateCnt = _owner.gameMain().modeServer().frameCount();
   // ���f���̃A�j���[�V�����̐ݒ�
   _owner._modelAnimeComponent->ChangeAnime("reload", true,0.5);
   // �����[�h��Ԃ̃J�E���g��0�ɐݒ�
   _reloadCnt = 0;
   // �I�u�W�F�N�g�������グ���Ȃ��Ɛݒ�
   _owner._isLift = false;
   _owner.GetSoundComponent().Play("Reload");
}

void Player::StateReload::Update() {
   StateBase::Update();
   // �����[�h��Ԃ̃J�E���g������̒l�����傫��������ҋ@��Ԃ�
   if (_reloadCnt > 60 * 2) {
      _owner._gameMain.playerBullet(5);
      _owner._stateServer->GoToState("Idle");
   }
   // �����蔻�菈�����s���N���X�Ńv���C���[���K�g�����O�Ɠ������Ă��邩�m�F
   _owner._collisionComponent->GatlingFromPlayer();
   // �����蔻�菈�����s���N���X�Ńv���C���[�����[�U�[�Ɠ������Ă��邩�m�F
   _owner._collisionComponent->PlayerFromLaser();
   // ���[�W�G�l�~�[�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromLargeEnemy();
   // �G���G�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromPoorEnemy();
   // �������̗����I�u�W�F�N�g�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromFallObject();
   // �K�g�����O�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromGatling();
   // ���[�U�[�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromLaser();
   // �����[�h��Ԃ̃J�E���g�̍X�V
   ++_reloadCnt;
   // �N�[���^�C���̍X�V
   --_owner._coolTime;
   // ���G���ԍX�V
   --_owner._invincibleCnt;
}

void Player::StateRecovery::Enter() {
   _owner._stateCnt = _owner.gameMain().modeServer().frameCount();
   // ���f���̃A�j���[�V�����̐ݒ�
   _owner._modelAnimeComponent->ChangeAnime("heal", false);
   // �񕜏�Ԃ̃J�E���g��0�ɐݒ�
   _recoveryCnt = 0;
   auto efcHeal = std::make_unique<Effect::EffectHeal>(_owner._gameMain, "Heal");
   auto hipsFramePos = _owner._modelAnimeComponent->GetFrameChildPosion("Kamilla_kari_Reference", "Kamilla_kari_Hips");
   efcHeal->position(hipsFramePos);
   _owner.GetEfcServer().Add(std::move(efcHeal));
   // �I�u�W�F�N�g�������グ���Ȃ��Ɛݒ�
   _owner._isLift = false;
}

void Player::StateRecovery::Update() {
   /**
    * \brief double�^�̒l�𕶎���Ŏw�肵�A�l�Ǘ��N���X����擾����
    * \param paramName �l���w�肷�镶����
    * \return ������ɂ��w�肳�ꂽ�l
    */
   const auto _DoubleParam = [&](std::string paramName) {
      return _owner._param->GetDoubleParam(paramName);
   };
   // ��Ԃ̊��N���X�̍X�V����
   StateBase::Update();
   // �񕜏�Ԃ̃J�E���g������̒l�����傫�����m�F
   if (_recoveryCnt > 60 * 1) {
      const auto MaxHp = _DoubleParam("max_hp");
      // �񕜏�Ԃ̃J�E���g������̒l�����傫���ꍇ
      // �񕜗ʂ̐ݒ�
      auto recovery = MaxHp * _DoubleParam("recovery_rate");
      // �q�b�g�|�C���g���񕜗ʕ����₷
      _owner._gameMain.playerHp(_owner._gameMain.playerHp() + recovery);
      // �q�b�g�|�C���g���ő�l�����傫���Ȃ����ꍇ�q�b�g�|�C���g���ő�l�ɂ���
      if (_owner._gameMain.playerHp() >= MaxHp) {
         _owner._gameMain.playerHp(MaxHp);
      }
      // �|�[�V�����̐�������炷
      _owner._gameMain.playerPortion(_owner._gameMain.playerPortion() - 1);
      // �ҋ@��Ԃ�
      _owner._stateServer->GoToState("Idle");
   }
   // �����蔻�菈�����s���N���X�Ńv���C���[���K�g�����O�Ɠ������Ă��邩�m�F
   _owner._collisionComponent->GatlingFromPlayer();
   // �����蔻�菈�����s���N���X�Ńv���C���[�����[�U�[�Ɠ������Ă��邩�m�F
   _owner._collisionComponent->PlayerFromLaser();
   // ���[�W�G�l�~�[�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromLargeEnemy();
   // �G���G�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromPoorEnemy();
   // �������̗����I�u�W�F�N�g�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromFallObject();
   // �K�g�����O�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromGatling();
   // ���[�U�[�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromLaser();
   // �񕜏�Ԃ̃J�E���g�̍X�V
   ++_recoveryCnt;
   // �N�[���^�C���̍X�V
   --_owner._coolTime;
   // ���G���ԍX�V
   --_owner._invincibleCnt;
}

void Player::StateRun::FootStepSound() {
   // �t���[���J�E���g�̎擾
   auto count = _owner.gameMain().modeServer().frameCount();
   // �J�E���g�����ȏ�o�߂��Ă��邩
   if (count - _footCnt >= FootStepStart) {
      // �v���C���[�̗����t���[���̎擾
      auto rightFootFramePos = _owner.modelAnimeComponent().GetFrameChildPosion("Kamilla_kari_Reference", "Kamilla_kari_RightToeBase");
      auto leftFootFramePos = _owner.modelAnimeComponent().GetFrameChildPosion("Kamilla_kari_Reference", "Kamilla_kari_LeftToeBase");
      // �v���C���[�̗����t���[���̍������擾
      auto rightFootY = rightFootFramePos.GetY();
      auto leftFootY = leftFootFramePos.GetY();
      // �v���C���[�̉E���t���[���͈��ȏ�̍�����
      if (rightFootY >= FootStepHeight) {
         _footRightStep = true;    // ��������t���O��true
      }
      else {
         // �E��������t���O��true��
         if (_footRightStep) {
            _owner.GetSoundComponent().Play("PlayerRightFootStep");  // �����̍Đ�
            _footRightStep = false;                                  // ��������t���O��false
         }
      }
      // �v���C���[�̍����t���[���͈��ȏ�̍�����
      if (leftFootY >= FootStepHeight) {
         _footLeftStep = true;    // ��������t���O��true
      }
      else {
         // ����������t���O��true��
         if (_footLeftStep) {
            auto& soundServer = _owner.GetSoundComponent();
            soundServer.Play("PlayerLeftFootStep");  // �����̍Đ�
            _footLeftStep = false;                   // ��������t���O��false
         }
      }
   }
}
