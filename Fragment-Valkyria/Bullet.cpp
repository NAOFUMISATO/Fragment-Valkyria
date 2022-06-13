
/*****************************************************************//**
 * \file   Bullet.cpp
 * \brief  �v���C���[�̉��u��U���̒e�̏������񂷃N���X
 * 
 * \author YUSUKEAHAMEDO
 * \date   January 2022
 *********************************************************************/
#include "Bullet.h"
#include "CollisionComponent.h"
#include "ObjectServer.h"
#include "ParamBullet.h"

using namespace FragmentValkyria::Player;

Bullet::Bullet(Game::GameMain& gameMain) : ObjectBase{ gameMain } {
   _param = std::make_unique<Param::ParamBullet>(_gameMain,"weakbullet");
   _collParam = std::make_unique < Param::ParamCollision>(_gameMain, "collision");
}

void Bullet::Init() {
   // �J�����̒����_�ւ̃x�N�g�����擾
   _moved = GetObjServer().GetVecData("CamTarget") - _position;
   // �P�ʉ�����
   _moved.Normalized();
}

void Bullet::Update() {
   // ��Ԃ̍X�V
   _stateServer->Update();
   // ���[���h�s��̍X�V
   ComputeWorldTransform();
}

void Bullet::Draw() {
   // �e��Ԃ̕`�揈������
   _stateServer->Draw();
}

void Bullet::Move() {
   // �ʒu�̍X�V
   _position = _position + _moved * _param->GetDoubleParam("speed");
}

void Bullet::HitCheckFromLargeEnemy() {
   // �����蔻�茋�ʃN���X�̎Q�Ƃ̎擾
   auto report = _collisionComponent->report();
   // �����蔻�茋�ʂ����[�W�G�l�~�[�Ɠ������Ă����玀�S��Ԃ�
   if (report.id() == Collision::CollisionComponent::ReportId::HitFromLargeEnemy) {
      // �����Ŏ��S��Ԃɐݒ肷�邱�ƂŒe�������������Ă��܂��̂𒼂���
      SetDead();
      _efcBullet->StopEffect();
   }
}

void Bullet::HitCheckFromPoorEnemyGatling() {
   // �����蔻�茋�ʃN���X�̎Q�Ƃ̎擾
   auto report = _collisionComponent->report();
   // �����蔻�茋�ʂ��K�g�����O�U�������Ă���G���G�Ɠ������Ă����玀�S��Ԃ�
   if (report.id() == Collision::CollisionComponent::ReportId::HitFromPoorEnemy) {
      // �����Ŏ��S��Ԃɐݒ肷�邱�ƂŒe�������������Ă��܂��̂𒼂���
      SetDead();
      _efcBullet->StopEffect();
   }
}

void Bullet::OutCheckFromStage() {
   // �����蔻�茋�ʃN���X�̎Q�Ƃ̎擾
   auto report = _collisionComponent->report();
   // �����蔻�茋�ʂ��X�e�[�W�O�̏ꍇ���S��Ԃ�
   if (report.id() == Collision::CollisionComponent::ReportId::OutStage) {
      // �����Ŏ��S��Ԃɐݒ肷�邱�ƂŒe�������������Ă��܂��̂𒼂���
      SetDead();
      _efcBullet->StopEffect();
   }
}

void Bullet::StateBase::Draw() {
#ifdef _DEBUG
   // �ʒu�������Vector4�N���X����DxLib::VECTOR�\���̂֕ϊ�
   auto pos = AppFrame::Math::ToDX(_owner._position);
   // ���a��float�^�ɃL���X�g
   auto radius = static_cast<float>(_owner._collParam->GetDoubleParam("bullet_radius"));
   // DxLib�ɂ�鋅�̕`��
   DrawSphere3D(pos, radius, 20, GetColor(0, 0, 255), GetColor(0, 0, 0), FALSE);
#endif
}

void Bullet::StateShoot::Enter() {
   _owner._efcBullet = std::make_unique<Effect::EffectWeakBullet>(_owner._gameMain,"WeakBullet");
   _owner._efcBullet->Init();
   Update();
}

void Bullet::StateShoot::Update() {
   // �ړ�����
   _owner.Move();
   // �����蔻�菈�����s���N���X�Ń��[�W�G�l�~�[�Ɠ������Ă��邩�m�F
   _owner._collisionComponent->LargeEnemyFromBullet();
   // �����蔻�菈�����s���N���X�ŃX�e�[�W�O�ɂ��邩�m�F
   _owner._collisionComponent->OutStage();
   // ���[�W�G�l�~�[�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromLargeEnemy();
   // �K�g�����O�U�������Ă���G���G�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromPoorEnemyGatling();
   // �X�e�[�W�O�ɂ��邩�m�F
   _owner.OutCheckFromStage();
   _owner._efcBullet->position(_owner._position);
   _owner._efcBullet->Update();
}



