
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

namespace {
    auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("player",{
       "idle_animespeed","walk_animespeed","run_animespeed","shootready_animespeed","shoot_animespeed",
       "move_speed", "capsule_pos1", "capsule_pos2", "capsule_radius",
        "rocovery_rate", "max_hp", "rotate_rate"});

    const double IdleAnimeSpeed = paramMap["idle_animespeed"];                //!< �ҋ@��Ԃ̃A�j���[�V�����X�s�[�h
    const double WalkAnimeSpeed = paramMap["walk_animespeed"];                //!< ������Ԃ̃A�j���[�V�����X�s�[�h
    const double RunAnimeSpeed = paramMap["run_animespeed"];                  //!< �����Ԃ̃A�j���[�V�����X�s�[�h
    const double ShootReadyAnimeSpeed = paramMap["shootready_animespeed"];    //!< �ˌ��������̃A�j���[�V�����X�s�[�h
    const double ShootAnimeSpeed = paramMap["shoot_animespeed"];              //!< �ˌ��̃A�j���[�V�����X�s�[�h
    const double MoveSpeed = paramMap["move_speed"];                          //!< �ړ��̑���
    const double CapsulePos1 = paramMap["capsule_pos1"];                      //!< �J�v�Z���̈�ڂ̍��W�܂ł̈ʒu����̍���
    const double CapsulePos2 = paramMap["capsule_pos2"];                      //!< �J�v�Z���̓�ڂ̍��W�܂ł̈ʒu����̍���
    const double CapsuleRadius = paramMap["capsule_radius"];                  //!< �J�v�Z���̔��a
    const double MaxHp = paramMap["max_hp"];                                  //!< �q�b�g�|�C���g�̍ő�l
    const double RecoveryRate = paramMap["rocovery_rate"];                    //!< �q�b�g�|�C���g�̍ő�l����̉񕜂��銄��
    const double RotateRate = paramMap["rotate_rate"];                        //!< ��]��������Ƃ��̃x�N�g���̖ʐς����߂�Ƃ��̃x�N�g���̑傫��

    constexpr auto FootStepHeight = 3.0;                                      //!< �����Ԏ��̑�����������(���̍b����̈ʒu)
    constexpr auto FootStepStart = 10;                                        //!< �����ԑJ�ڎ�����̑����������t���[��
}

using namespace FragmentValkyria::Player;

Player::Player(Game::GameMain& gameMain) : ObjectBase{ gameMain } {
}

void Player::Init(){
    _rightRotation.RotateY(90.0, true);     // �x�N�g����90�x��]������}�g���N�X�̍쐬
    _leftRotation.RotateY(-90.0, true);     // �x�N�g����-90�x��]������}�g���N�X�̍쐬
    _backRotation.RotateY(180.0, true);     // �x�N�g����180�x��]������}�g���N�X�̍쐬
    _isAim = false;                         // �G�C�������̃t���O��false
    _isDeadMotion = false;                  // ���S���[�V���������̃t���O��false
    _rotateDir = GetForward() * RotateRate; // ��]�Ō��������������ɍŏ��Ɍ����Ă��������ݒ�
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
   GetObjServer().RegistDouble("PlayerHP",_hp);
   GetObjServer().RegistDouble("PlayerBulletStock",static_cast<double>(_bulletStock));
   GetObjServer().RegistDouble("PlayerPortionStock", static_cast<double>(_portionStock));
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
    // x��z�̈ړ���̐������ʒu�ɐݒ肷��
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
   forward = forward * RotateRate;
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

void Player::HitCheckFromIdleFallObject() {
    // �����蔻�茋�ʃN���X�̎Q�Ƃ̎擾
    auto report = _collisionComponent->report();
    // �����蔻�茋�ʂ̊m�F
    if (report.id() == Collision::CollisionComponent::ReportId::HitFromIdleFallObject) {
        // �ҋ@��Ԃ̗����I�u�W�F�N�g�Ƃ������Ă�����
        // ���������|���S���̖@�����擾
        auto normal = _collisionComponent->hitPos();
        // �@���Ɉړ��̑������������x�N�g�����ʒu�����炷
        _position = _position + normal * MoveSpeed;
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
        _hp -= _collisionComponent->damage();
        // �m�b�N�o�b�N���Ă���Ɛݒ�
        _collisionComponent->knockBack(true);
        // �����蔻��̌��ʂ𓖂����Ă��Ȃ��Ɛݒ�
        _collisionComponent->report().id(Collision::CollisionComponent::ReportId::None);
        // �J�����̃Y�[�������Ȃ��Ɛݒ�
        _cameraComponent->SetZoom(false);
        // �m�b�N�o�b�N��Ԃ�
        _stateServer->PushBack("KnockBack");
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
        _hp -= _collisionComponent->damage();
        // �m�b�N�o�b�N���Ă���Ɛݒ�
        _collisionComponent->knockBack(true);
        // �����蔻��̌��ʂ𓖂����Ă��Ȃ��Ɛݒ�
        _collisionComponent->report().id(Collision::CollisionComponent::ReportId::None);
        // �J�����̃Y�[�������Ȃ��Ɛݒ�
        _cameraComponent->SetZoom(false);
        // �m�b�N�o�b�N��Ԃ�
        _stateServer->PushBack("KnockBack");
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
        _knockBack = knockBackDelta * 20.0;
        // �_���[�W�ʕ��q�b�g�|�C���g�����炷
        _hp -= _collisionComponent->damage();
        // �m�b�N�o�b�N���Ă���Ɛݒ�
        _collisionComponent->knockBack(true);
        // �����蔻��̌��ʂ𓖂����Ă��Ȃ��Ɛݒ�
        _collisionComponent->report().id(Collision::CollisionComponent::ReportId::None);
        // �J�����̃Y�[�������Ȃ��Ɛݒ�
        _cameraComponent->SetZoom(false);
        // �m�b�N�o�b�N��Ԃ�
        _stateServer->PushBack("KnockBack");
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
        // �P�ʉ�����
        knockBackVec.Normalized();
        // �m�b�N�o�b�N�ʂ̃x�N�g����ݒ�
        _knockBack = knockBackVec * 10.0;
        // �_���[�W�ʕ��q�b�g�|�C���g�����炷
        _hp -= _collisionComponent->damage();
        // �m�b�N�o�b�N���Ă���Ɛݒ�
        _collisionComponent->knockBack(true);
        // �J�����̃Y�[�������Ȃ��Ɛݒ�
        _cameraComponent->SetZoom(false);
        // �m�b�N�o�b�N��Ԃ�
        _stateServer->PushBack("KnockBack");
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
      // �P�ʉ�����
      knockBackVec.Normalized();
      // �m�b�N�o�b�N�ʂ̃x�N�g����ݒ�
      _knockBack = knockBackVec * 10.0;
      // �_���[�W�ʕ��q�b�g�|�C���g�����炷
      _hp -= _collisionComponent->damage();
      // �m�b�N�o�b�N���Ă���Ɛݒ�
      _collisionComponent->knockBack(true);
      // �J�����̃Y�[�������Ȃ��Ɛݒ�
      _cameraComponent->SetZoom(false);
      // �m�b�N�o�b�N��Ԃ�
      _stateServer->PushBack("KnockBack");
   }
}

void Player::WeakAttack() {
    // ���u��U���̒e�𐶐����ăI�u�W�F�N�g�T�[�o�[�֒ǉ�
    auto bullet = gameMain().objFactory().Create("Bullet");
    gameMain().objServer().Add(std::move(bullet));
}

void Player::StateBase::Draw() {
   // ���f���̕`�揈������
   _owner._modelAnimeComponent->Draw();
   // ���G���Ԓ����m�F
   if (_owner._invincibleCnt > 0) {
      // ���G���Ԓ��������烂�f���𓧖��ɋ߂�����
       MV1SetOpacityRate(_owner._modelAnimeComponent->modelHandle(), 0.1f);
   }
   else {
      // ���G���Ԓ�����Ȃ������烂�f����s�����ɂ���
       MV1SetOpacityRate(_owner._modelAnimeComponent->modelHandle(), 1.0f);
   }
#ifdef _DEBUG
   // �v���C���[�̃J�v�Z���̈�ڂ̍��W�̐ݒ�
   auto pos1 = _owner._position + Vector4(0.0, CapsulePos1, 0.0);
   // �v���C���[�̃J�v�Z���̓�ڂ̍��W�̐ݒ�
   auto pos2 = _owner._position + Vector4(0.0, CapsulePos2, 0.0);
   // �v���C���[�̃J�v�Z���̔��a�̐ݒ�
   auto radian = static_cast<float>(CapsuleRadius);
   // �v���C���[�̃X�e�[�W�Ƃ̔���p�̐����̎n�_�̐ݒ�
   auto start = _owner._position + Vector4(0.0, 50.0, 0.0);
   // �v���C���[�̃X�e�[�W�Ƃ̔���p�̐����̏I�_�̐ݒ�
   auto end = _owner._position + Vector4(0.0, -10000.0, 0.0);
  // �v���C���[�̃J�v�Z���̕`��
   DrawCapsule3D(AppFrame::Math::ToDX(pos1), AppFrame::Math::ToDX(pos2), radian, 20, GetColor(0, 255, 0), GetColor(0, 0, 0), FALSE);
   // �v���C���[�̃X�e�[�W�Ƃ̔���p�̐����̕`��
   DrawLine3D(AppFrame::Math::ToDX(start), AppFrame::Math::ToDX(end), AppFrame::Math::Utility::GetColorCode(0, 255, 255));
#endif
}

void Player::StateIdle::Enter() {
   // ���f���̃A�j���[�V�����̐ݒ�
   _owner._modelAnimeComponent->ChangeAnime("stay", true, IdleAnimeSpeed);
}

void Player::StateIdle::Input(InputManager& input) {
   // ���X�e�B�b�N�������Ă������Ԃ̃��X�g�ɑ����Ԃ�ǉ�����
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
   if (input.GetXJoypad().XClick() && _owner._bulletStock < 5) {
      _owner._stateServer->GoToState("Reload");
   }
   // Y�{�^����������Ă��āA�|�[�V�����̐���0���傫���q�b�g�|�C���g���ő�q�b�g�|�C���g������������񕜏�Ԃ�
   if (input.GetXJoypad().YClick() && _owner._portionStock > 0 && _owner._hp < 100.0) {
      // �|�[�V�����̐�������炷
      --_owner._portionStock;
      _owner._stateServer->GoToState("Recovery");
   }
}

void Player::StateIdle::Update() {
   // ��]����
   _owner.Rotate();
   // �R���W�����R���|�[�l���g�Ńv���C���[���I�u�W�F�N�g�������グ����͈͂ɂ��邩�m�F
   _owner._collisionComponent->ObjectRangeFromPlayer();
   // �ҋ@��Ԃ̗����I�u�W�F�N�g�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromIdleFallObject();
   // �R���W�����R���|�[�l���g�Ńv���C���[���K�g�����O�Ɠ������Ă��邩�m�F
   _owner._collisionComponent->GatlingFromPlayer();
   // �R���W�����R���|�[�l���g�Ńv���C���[�����[�U�[�Ɠ������Ă��邩�m�F
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
   // ���G���ԍX�V
   --_owner._invincibleCnt;
}

void Player::StateRun::Enter() {
   // ���f���̃A�j���[�V�����̐ݒ�
   _owner._modelAnimeComponent->ChangeAnime("run", true, RunAnimeSpeed);
   auto count = _owner.gameMain().modeServer().frameCount();
   _footCnt = count;
}
void Player::StateRun::Input(InputManager& input) {
   // �ҋ@��Ԃ̗����I�u�W�F�N�g�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromIdleFallObject();
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
   if (input.GetXJoypad().LeftStickX() >= 3000) {
      // ���X�e�B�b�N���E�ɓ����Ă�����ړ��ʂ̃x�N�g���ɃJ�������璍���_�ւ̕����P�ʃx�N�g����90�x��]�������x�N�g���𑫂�
      _owner._moved = _owner._moved + (_owner._direction * _owner._rightRotation);
      // �ړ����Ă��邩�̃t���O���ړ����Ă���Ɛݒ�
      moved = true;
   }
   // ���X�e�B�b�N�����ɓ����Ă��邩�m�F
   if (input.GetXJoypad().LeftStickX() <= -3000) {
      // ���X�e�B�b�N�����ɓ����Ă�����ړ��ʂ̃x�N�g���ɃJ�������璍���_�ւ̕����P�ʃx�N�g����-90�x��]�������x�N�g���𑫂�
      _owner._moved = _owner._moved + (_owner._direction * _owner._leftRotation);
      // �ړ����Ă��邩�̃t���O���ړ����Ă���Ɛݒ�
      moved = true;
   }
   // ���X�e�B�b�N����ɓ����Ă��邩�m�F
   if (input.GetXJoypad().LeftStickY() >= 3000) {
      // ���X�e�B�b�N����ɓ����Ă�����ړ��ʂ̃x�N�g���ɃJ�������璍���_�ւ̕����P�ʃx�N�g���𑫂�
      _owner._moved = _owner._moved + _owner._direction;
      // �ړ����Ă��邩�̃t���O���ړ����Ă���Ɛݒ�
      moved = true;
   }
   // ���X�e�B�b�N�����ɓ����Ă��邩�m�F
   if (input.GetXJoypad().LeftStickY() <= -3000) {
      // ���X�e�B�b�N�����ɓ����Ă�����ړ��ʂ̃x�N�g���ɃJ�������璍���_�ւ̕����P�ʃx�N�g����180�x��]�������x�N�g���𑫂�
      _owner._moved = _owner._moved + (_owner._direction * _owner._backRotation);
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
      _owner._stateServer->PushBack("WeakShootReady");
      // �J�����̃Y�[����������
      _owner._cameraComponent->SetZoom(true);
   }
   // X�{�^����������Ă��āA���u��U���̎c��e�������u��U���̍ő�e�������������瑕�U��Ԃ�
   if (input.GetXJoypad().XClick() && _owner._bulletStock < 5) {
      _owner._stateServer->GoToState("Reload");
   }
   // Y�{�^����������Ă��āA�|�[�V�����̐���0���傫���q�b�g�|�C���g���ő�q�b�g�|�C���g������������񕜏�Ԃ�
   if (input.GetXJoypad().YClick() && _owner._portionStock > 0 && _owner._hp < 100.0) {
      // �|�[�V�����̐�������炷
      --_owner._portionStock;
      _owner._stateServer->GoToState("Recovery");
   }
   // �ړ����Ă��邩�̃t���O���ړ����Ă��Ȃ��ƂȂ��Ă��邩�m�F
   if (!moved) {
      // �ړ����Ă��Ȃ��ƂȂ��Ă����炱�̏�Ԃ���Ԃ̃��X�g���珜�O����
      _owner._stateServer->PopBack();
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
         _owner._rotateDir = _owner._moved * RotateRate;
      }
      // �ړ��ʂ̃x�N�g����ݒ�
      _owner._moved = _owner._moved * MoveSpeed;
   }
}

void Player::StateRun::Update() {
   FootStepSound();
   // �ړ�����
   _owner.Move(_owner._moved);
   // ��]����
   _owner.Rotate();
   // �R���W�����R���|�[�l���g�Ńv���C���[���I�u�W�F�N�g�������グ����͈͂ɂ��邩�m�F
   _owner._collisionComponent->ObjectRangeFromPlayer();
   // �R���W�����R���|�[�l���g�Ńv���C���[���K�g�����O�Ɠ������Ă��邩�m�F
   _owner._collisionComponent->GatlingFromPlayer();
   // �R���W�����R���|�[�l���g�Ńv���C���[�����[�U�[�Ɠ������Ă��邩�m�F
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
   // ���G���ԍX�V
   --_owner._invincibleCnt;
}

void Player::StateShootReady::Enter() {
   // ���f���̃A�j���[�V�����̐ݒ�
   _owner._modelAnimeComponent->ChangeAnime("H_attack_pose_loop", true, ShootReadyAnimeSpeed);
   // �炷�T�E���h�̐ݒ�
   _owner.GetSoundComponent().Play("PlayerShootReady");
   // �G�C�����Ɛݒ�
   _owner._isAim = true;
}

void Player::StateShootReady::Input(InputManager& input) {
   // RB�{�^���������ꂽ��ҋ@��Ԃ�
   if (input.GetXJoypad().RBClick()) {
      // �炷�T�E���h�̐ݒ�
      _owner.GetSoundComponent().Play("PlayerShoot");
      // ���f���̃A�j���[�V�����̐ݒ�
      _owner._modelAnimeComponent->ChangeAnime("H_attack_attack", false, ShootAnimeSpeed);
      _owner._stateServer->GoToState("Idle");
      // �J�����̃Y�[�������Ȃ��Ɛݒ�
      _owner._cameraComponent->SetZoom(false);
   }
}

void Player::StateShootReady::Update() {
   // �ˌ���Ԃ̉�]����
   _owner.ShootRotate();
   // �R���W�����R���|�[�l���g�Ńv���C���[���K�g�����O�Ɠ������Ă��邩�m�F
   _owner._collisionComponent->GatlingFromPlayer();
   // �R���W�����R���|�[�l���g�Ńv���C���[�����[�U�[�Ɠ������Ă��邩�m�F
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
   // ���G���ԍX�V
   --_owner._invincibleCnt;
}

void Player::StateShootReady::Exit() {
   // �G�C��������Ȃ��Ɛݒ�
   _owner._isAim = false;
}

void Player::StateKnockBack::Enter() {
   // ���f���̃A�j���[�V�����̐ݒ�
   _owner.modelAnimeComponent().ChangeAnime("damaged", false, 1.2);
   // �m�b�N�o�b�N���鎞�Ԃ̐ݒ�
   _owner._freezeTime = 30;
}

void Player::StateKnockBack::Update() {
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
   if (_owner._hp <= 0) {
      _owner._stateServer->GoToState("Die");
   }
   // �q�b�g�|�C���g��0�ȉ�����Ȃ������ꍇ
   else {
      // ���G���Ԃ̐ݒ�
      _owner._invincibleCnt = 60 * 2;
      // �m�b�N�o�b�N���Ă��Ȃ��Ɛݒ�
      _owner.collisionComponent().knockBack(false);
      // �����蔻��̌��ʂ𓖂����Ă��Ȃ��Ɛݒ�
      _owner.collisionComponent().report().id(Collision::CollisionComponent::ReportId::None);
      // �ҋ@��Ԃ�
      _owner._stateServer->GoToState("Idle");
   }
}

void Player::StateDie::Enter() {
   // ���f���̃A�j���[�V�����̐ݒ�
   _owner.modelAnimeComponent().ChangeAnime("dawn", false);
   // ���S��ԂɂȂ��Ă���Q�[���I�[�o�[�܂ł̃t���[������ݒ�
   _timeOver = 60 * 2;
   // ���S���[�V�������Ɛݒ�
   _owner._isDeadMotion = true;
}

void Player::StateDie::Update() {
   // �Q�[���I�[�o�[�܂ł̃t���[�������c���Ă����猸�炷
   if (_timeOver > 0) {
      --_timeOver;
   }
   // �Q�[���I�[�o�[�܂ł̃t���[�������������ꍇ
   else {
      // ���[�h�T�[�o�[�ɃQ�[���I�[�o�[���[�h��}��
      _owner.gameMain().modeServer().PushBack("MissionFailed");
   }
}

void Player::StateWeakShootReady::Enter() {
   // ���f���̃A�j���[�V�����̐ݒ�
   _owner._modelAnimeComponent->ChangeAnime("L_attack_pose_loop", true);
   // �炷�T�E���h�̐ݒ�
   _owner.GetSoundComponent().Play("PlayerShootReady");
   // ���u��U���̃N�[���^�C����0�ɐݒ�
   _coolTime = 0;
   // �G�C�����Ɛݒ�
   _owner._isAim = true;
}

void Player::StateWeakShootReady::Input(InputManager& input) {
   // RB�{�^����������Ă��ăN�[���^�C�����Ȃ��A���u��U���̎c��e�������邩�m�F
   if (input.GetXJoypad().RBClick() && _coolTime <= 0 && _owner._bulletStock > 0) {
      // RB�{�^����������Ă��ăN�[���^�C�����Ȃ��A���u��U���̎c��e�����������ꍇ
      // ���u��U������
      _owner.WeakAttack();
      // ���f���̃A�j���[�V�����̐ݒ�
      _owner._modelAnimeComponent->ChangeAnime("L_attack_attack", false, ShootAnimeSpeed);
      // �炷�T�E���h�̐ݒ�
      _owner.GetSoundComponent().Play("PlayerShoot");
      // ���u��U���̎c��e�������炷
      --_owner._bulletStock;
      // �N�[���^�C���̐ݒ�
      _coolTime = 60 * 1;
   }
   // LB�{�^���������ꂽ��ҋ@��Ԃ�
   if (input.GetXJoypad().LBClick()) {
      _owner._stateServer->GoToState("Idle");
      // �J�����̃Y�[�������Ȃ��Ɛݒ�
      _owner._cameraComponent->SetZoom(false);
   }
}

void Player::StateWeakShootReady::Update() {
   // �ˌ���Ԃ̉�]����
   _owner.ShootRotate();
   // �N�[���^�C���̍X�V
   --_coolTime;
   // ���G���ԍX�V
   --_owner._invincibleCnt;
}

void Player::StateWeakShootReady::Exit() {
   // �G�C��������Ȃ��Ɛݒ�
   _owner._isAim = false;
}

void Player::StateReload::Enter() {
   // ���f���̃A�j���[�V�����̐ݒ�
   _owner._modelAnimeComponent->ChangeAnime("stealth_sit", true, 0.8);
   // �����[�h��Ԃ̃J�E���g��0�ɐݒ�
   _reloadCnt = 0;
}

void Player::StateReload::Update() {
   // �����[�h��Ԃ̃J�E���g������̒l�����傫��������ҋ@��Ԃ�
   if (_reloadCnt > 60 * 2) {
      _owner._bulletStock = 5;
      _owner._stateServer->GoToState("Idle");
   }
   // �����[�h��Ԃ̃J�E���g�̍X�V
   ++_reloadCnt;
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

void Player::StateRecovery::Enter() {
   // ���f���̃A�j���[�V�����̐ݒ�
   _owner._modelAnimeComponent->ChangeAnime("heal", false);
   // �񕜏�Ԃ̃J�E���g��0�ɐݒ�
   _recoveryCnt = 0;
}

void Player::StateRecovery::Update() {
   // �񕜏�Ԃ̃J�E���g������̒l�����傫�����m�F
   if (_recoveryCnt > 60 * 2) {
      // �񕜏�Ԃ̃J�E���g������̒l�����傫���ꍇ
      // �񕜗ʂ̐ݒ�
      auto recovery = MaxHp * RecoveryRate;
      // �q�b�g�|�C���g���񕜗ʕ����₷
      _owner._hp += recovery;
      // �q�b�g�|�C���g���ő�l�����傫���Ȃ����ꍇ�q�b�g�|�C���g���ő�l�ɂ���
      if (_owner._hp >= MaxHp) {
         _owner._hp = MaxHp;
      }
      // �ҋ@��Ԃ�
      _owner._stateServer->GoToState("Idle");
   }
   // �񕜏�Ԃ̃J�E���g�̍X�V
   ++_recoveryCnt;
   // ���G���ԍX�V
   --_owner._invincibleCnt;
}
