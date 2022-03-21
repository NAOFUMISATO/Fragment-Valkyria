
/*****************************************************************//**
 * \file   Gatling.cpp
 * \brief  �K�g�����O
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "Gatling.h"
#include "CollisionComponent.h"
#include "ModelAnimeComponent.h"
#include "ObjectServer.h"
#include "EffectGatlingMuzzleFlash.h"
#include "EffectGatlingBullet.h"
#include "EffectServer.h"

namespace {
   auto gatParamMap = AppFrame::Resource::LoadParamJson::GetParamMap("gatling",{ "speed" });
   const double Speed = gatParamMap["speed"];

   auto collParamMap = AppFrame::Resource::LoadParamJson::GetParamMap("collision", { "gatling_radius" });
   const double Radius = collParamMap["gatling_radius"];
}

using namespace FragmentValkyria::Enemy;

Gatling::Gatling(Game::GameMain& gameMain) : ObjectBase{ gameMain } {

}

void Gatling::Init() {
   // �ړ���������̃x�N�g���̐ݒ�
   _moveDirection = GetObjServer().GetVecData("GatlingMoveDirection");
   // �ړ���������̃x�N�g���̊e�����𕪉�
   auto [x, y, z] = _moveDirection.GetVec3();
   // �ړ���������̃x�N�g���̍������Ȃ���
   _moveDirection = Vector4(x, 0.0, z);
   // �P�ʉ�����
   _moveDirection.Normalized();
}

void Gatling::Update() {
   // ��Ԃ̍X�V
   _stateServer->Update();
   // ���[���h�s��̍X�V
   ComputeWorldTransform();
   
}

void Gatling::Draw() {
   // �e��Ԃ̕`�揈������
   _stateServer->Draw();
}

void Gatling::Move() {
   // �ړ��ʂ̃x�N�g���̐ݒ�
   _moved = _moveDirection * Speed;
   // �ʒu�̐ݒ�
   _position = _position + _moved;
}

void Gatling::HitCheckFromObjectModel() {
   // �����蔻�茋�ʃN���X�̎Q�Ƃ̎擾
   auto report = _collisionComponent->report();
   // �����蔻�茋�ʂ������I�u�W�F�N�g�Ɠ������Ă����玀�S��Ԃ�
   if (report.id() == Collision::CollisionComponent::ReportId::HitFromIdleFallObject) {
      _stateServer->PushBack("Die");
   }
}

void Gatling::HitCheckFromPlayer() {
   // �����蔻�茋�ʃN���X�̎Q�Ƃ̎擾
   auto report = _collisionComponent->report();
   // �����蔻�茋�ʂ��v���C���[�Ɠ������Ă����玀�S��Ԃ�
   if (report.id() == Collision::CollisionComponent::ReportId::HitFromPlayer) {
      _stateServer->PushBack("Die");
   }
}

void Gatling::OutStageCheck() {
   // �����蔻�茋�ʃN���X�̎Q�Ƃ̎擾
   auto report = _collisionComponent->report();
   // �����蔻�茋�ʂ��X�e�[�W�O�̏ꍇ�͎��S��Ԃ�
   if (report.id() == Collision::CollisionComponent::ReportId::OutStage) {
      _stateServer->PushBack("Die");
   }
}

void Gatling::StateBase::Draw() {
   _owner._efcGatling->Draw();
#ifdef _DEBUG
   // �ʒu�������Vector4�N���X����DxLib::VECTOR�\���̂ւ̕ϊ�
   auto position = AppFrame::Math::ToDX(_owner._position);
   // ���a��float�^�ɃL���X�g
   auto radian = static_cast<float>(Radius);
   // DxLib�ɂ�鋅�̕`��
   DrawSphere3D(position, radian, 10, GetColor(255, 0, 0), GetColor(0, 0, 0), FALSE);
#endif
}

void Gatling::StateChase::Enter() {
   auto efcMuzzleFlash = std::make_unique<Effect::EffectGatlingMuzzleFlash>(_owner._gameMain,"GatlingMuzzleFlash");
   efcMuzzleFlash->position(_owner._position);
   _owner.GetEfcServer().Add(std::move(efcMuzzleFlash));
   _owner._efcGatling = std::make_unique<Effect::EffectGatlingBullet>(_owner._gameMain, "GatlingBullet");
   _owner._efcGatling->Init();
   Update();
}

void Gatling::StateChase::Update() {
   // �ړ�����
   _owner.Move();
   // �����蔻�菈�����s���N���X�ŃX�e�[�W�O�ɂ��邩�m�F
   _owner._collisionComponent->OutStage();
   // �v���C���[�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromPlayer();
   // �����I�u�W�F�N�g�̃��f���Ɠ������Ă��邩�m�F
   _owner.HitCheckFromObjectModel();
   // �X�e�[�W�O�ɂ��邩�m�F
   _owner.OutStageCheck();
   _owner._efcGatling->position(_owner._position);
   _owner._efcGatling->Update();
}

void Gatling::StateDie::Update() {
   // ���S��Ԃɐݒ�
   _owner.SetDead();
   _owner._efcGatling->StopEffect();
}

void Gatling::StateDie::Draw() {
#ifdef _DEBUG
   // �ʒu�������Vector4�N���X����DxLib::VECTOR�\���̂ւ̕ϊ�
   auto position = AppFrame::Math::ToDX(_owner._position);
   // ���a��float�^�ɃL���X�g
   auto radian = static_cast<float>(Radius);
   // �u�����h���[�h�����u�����h�ɐݒ�
   SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
   // DxLib�ɂ�鋅�̕`��
   DrawSphere3D(position, radian, 10, GetColor(255, 0, 0), GetColor(0, 0, 0), FALSE);
   // �u�����h���[�h���m�[�u�����h�ɐݒ�
   SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
#endif
}

