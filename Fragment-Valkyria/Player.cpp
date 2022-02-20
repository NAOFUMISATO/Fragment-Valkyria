
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
        "rocovery_rate", "max_hp"});

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
    const double RecoveryRate = paramMap["rocovery_rate"];                     //!< �q�b�g�|�C���g�̍ő�l����̉񕜂��銄��

    constexpr auto FootStepHeight = 3.0;                                      //!< �����Ԏ��̑�����������(���̍b����̈ʒu)
    constexpr auto FootStepStart = 10;                                        //!< �����ԑJ�ڎ�����̑����������t���[��
}

using namespace FragmentValkyria::Player;

Player::Player(Game::GameMain& gameMain) : ObjectBase{ gameMain } {
}

void Player::Init(){
    // �x�N�g����90�x��]������}�g���N�X�̍쐬
    _rightRotation.RotateY(90.0, true);
    // �x�N�g����-90�x��]������}�g���N�X�̍쐬
    _leftRotation.RotateY(-90.0, true);
    // �x�N�g����180�x��]������}�g���N�X�̍쐬
    _backRotation.RotateY(180.0, true);
    _isAim = false;        // �G�C�������̃t���O��false
    _isDeadMotion = false; // ���S���[�V���������̃t���O��false
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
   // �J�����̍X�V
   _cameraComponent->SetPlyPos(_position);
   _cameraComponent->Update();
   // �I�u�W�F�N�g�T�[�o�[�Ɉʒu��ʒm
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
    // �e��Ԃ̕`�揈��
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

void Player::HitCheckFromFallObjectRange() {
    // �����蔻�茋�ʃN���X�̎Q�Ƃ̎擾
    auto report = _collisionComponent->report();
    // �����蔻�茋�ʂ̊m�F
    if (report.id() == Collision::CollisionComponent::ReportId::HitFromObjectRange) {
        // �����蔻�茋�ʂ��I�u�W�F�N�g�������グ����͈͂ɂ���ƂȂ��Ă�����ˌ�������Ԃ�
        _stateServer->PushBack("ShootReady");
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

        //// �ړ��ʂ̃x�N�g���̎擾
        //auto moveVec = _moved * MoveSpeed;
        //// �ړ��ʂ̃x�N�g���Ɩ@���̊O�ς��Ƃ�
        //auto slideVec = moveVec.Cross(normal);
        //// �@���ƈړ��ʂ̃x�N�g���Ɩ@���̊O�ς̌��ʂƂ̊O�ς̌��ʂ���X���C�h������ׂ������̃x�N�g�����擾
        //slideVec = normal.Cross(slideVec);
   
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
            vec.Normalized();
            auto knockBackDelta = vec * mat;
            // �m�b�N�o�b�N�ʂ̃x�N�g����ݒ�
            _knockBack = knockBackDelta * 10.0;
        }
        else {
            // �����I�u�W�F�N�g���^��ɂȂ��ꍇ
            // �m�b�N�o�b�N�ʂ̃x�N�g����ݒ�
            auto knockBackVec = _position - hitPos;
            // �������Ȃ���
            auto [x, y, z] = knockBackVec.GetVec3();
            auto knockBackDelta = Vector4(x, 0.0, z);
            knockBackDelta.Normalized();
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
        // �m�b�N�o�b�N�ʂ̃x�N�g����ݒ�
        auto knockBackVec = _position - hitPos;
        // �������Ȃ���
        auto [x, y, z] = knockBackVec.GetVec3();
        auto knockBackDelta = Vector4(x, 0.0, z);
        knockBackDelta.Normalized();
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
        // �m�b�N�o�b�N�ʂ̃x�N�g����ݒ�
        auto knockBackVec = _position - hitPos;
        knockBackVec.Normalized();
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
      // �m�b�N�o�b�N�ʂ̃x�N�g����ݒ�
      auto knockBackVec = _position - hitPos;
      knockBackVec.Normalized();
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
/// �ҋ@
void Player::StateIdle::Enter() {
   /*_owner._forwardSpeed = 0.0;*/
   _owner._modelAnimeComponent->ChangeAnime("stay", true, IdleAnimeSpeed);
}
void Player::StateIdle::Input(InputManager& input) {

   if (input.GetKeyboard().SpaceClick()) {
      _owner._stateServer->PushBack("Attack");
   }
   //���X�e�B�b�N�������Ă����瑖���Ԃ�
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
    
    //���G���ԍX�V
    --_owner._invincibleCnt;
}

/// ����
void Player::StateRun::Enter() {
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

   // ���G���ԍX�V
   --_owner._invincibleCnt;
}

/// �U��
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

    // ���G���ԍX�V
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
    // ���G���ԍX�V
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

    // ���G���ԍX�V
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

    // ���G���ԍX�V
    --_owner._invincibleCnt;
}
