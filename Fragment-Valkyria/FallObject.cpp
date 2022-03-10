
/*****************************************************************//**
 * \file   FallObject.cpp
 * \brief  
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "FallObject.h"
#include "CollisionComponent.h"
#include "GameMain.h"
#include "Player.h"
#include "ModelAnimeComponent.h"
#include "EffectObjectFall.h"
#include "ObjectServer.h"
#include "EffectServer.h"

using namespace FragmentValkyria::Enemy;

namespace {
   auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("fallobject",
      { "range", "gravity", "shoot_speed", "up_speed", "rotate_angle",
      "updown_range", "right_hand_up_value", "up_value", "capsule_pos1", "capsule_pos2", "capsule_radius",
      "fallpoint_pos_y","fallpoint_scale","fallpoint_animespeed" });

   const double Range = paramMap["range"];
   const double Gravity = paramMap["gravity"];
   const double ShootSpeed = paramMap["shoot_speed"];
   const double UpSpeed = paramMap["up_speed"];
   const double RotateAngle = paramMap["rotate_angle"];
   const double UpDownRange = paramMap["updown_range"];
   const double RightHandUpValue = paramMap["right_hand_up_value"];
   const double UpValue = paramMap["up_value"];
   const double CapsulePos1 = paramMap["capsule_pos1"];
   const double CapsulePos2 = paramMap["capsule_pos2"];
   const double CapsuleRadius = paramMap["capsule_radius"];
   const double FallPointPosY = paramMap["fallpoint_pos_y"];
   const double FallPointScale = paramMap["fallpoint_scale"];
   const int FallPointAnimeSpeed = paramMap["fallpoint_animespeed"];
   constexpr auto DefaultPointScale = 1.0;
   constexpr auto DefaultPointAngle = 0.0;
   constexpr auto UpEffectDiffY = 20.0;
}

FallObject::FallObject(Game::GameMain& gameMain) : ObjectBase{ gameMain } {

}

void FallObject::Init() {
   auto modelHandle = _modelAnimeComponent->modelHandle();
   _collision = MV1SearchFrame(modelHandle, "drum_green_c");
   // �i�r���b�V���̕\���ݒ�
#ifndef _DEBUG
   MV1SetFrameVisible(modelHandle, _collision, false);
#else
   MV1SetFrameOpacityRate(modelHandle, _collision, 0.5f);
#endif

   // �t���[��1���i�r���b�V���Ƃ��Ďg�p
   MV1SetupCollInfo(modelHandle, _collision);

   _fallPointHandles = _gameMain.resServer().GetTextures("FallPoint");
}

void FallObject::Input(InputManager& input) {
   _stateServer->Input(input);
}

void FallObject::Update() {
   //�R���W�������̍X�V
   MV1RefreshCollInfo(_modelAnimeComponent->modelHandle(), _collision);
   // ��Ԃ̍X�V
   _stateServer->Update();
   // ���[���h�s��̍X�V
   ComputeWorldTransform();
   // ���f���̍X�V
   _modelAnimeComponent->Update();
}

void FallObject::Draw() {
   _stateServer->Draw();
}

void FallObject::HitCheckFromPlayerPoint() {
   auto report = _collisionComponent->report();
   if (report.id() == Collision::CollisionComponent::ReportId::HitFromPlayer) {
      _stateServer->GoToState("Up");
   }
}

void FallObject::HitCheckFromLargeEnemy() {
   auto report = _collisionComponent->report();

   if (report.id() == Collision::CollisionComponent::ReportId::HitFromLargeEnemy) {
      _stateServer->GoToState("Die");
   }
}

void FallObject::HitCheckFromLaser() {
   auto report = _collisionComponent->report();

   if (report.id() == Collision::CollisionComponent::ReportId::HitFromLaser) {
      _stateServer->GoToState("Die");
   }
}

void FallObject::HitCheckFromPoorEnemyGatling() {
   auto report = _collisionComponent->report();

   if (report.id() == Collision::CollisionComponent::ReportId::HitFromPoorEnemyGatling) {
      _stateServer->GoToState("Die");
   }
}

void FallObject::OutStageCheck() {
   auto report = _collisionComponent->report();

   if (report.id() == Collision::CollisionComponent::ReportId::OutStage) {
      _stateServer->GoToState("Die");
   }
}

void FallObject::CheckPlayerKnockBack() {
   auto result = _collisionComponent->knockBack();
   if (result) {
      _stateServer->GoToState("Die");
   }
}

void FallObject::Save() {
   // �s�K���ȉ�]�������邽�߂Ɏg���T�C���̒l���擾���邽�߂Ɏg���p�x�̍X�V
   _rotateAngle += 1.0;
   // �p�x�����W�A���ɂ���
   auto radian = AppFrame::Math::Utility::DegreeToRadian(_rotateAngle);
   // �T�C���̒l���擾
   auto sinValue = std::sin(radian);
   // ����̊p�x��-1�{����1�{�͈̔͂Ŋp�x�����߂�
   auto xyz = RotateAngle * sinValue;
   // ��]�̃x�N�g����ݒ�
   _rotation = Vector4(xyz, xyz, xyz);
   // �I�u�W�F�N�g�ꊇ�Ǘ��N���X�̃I�u�W�F�N�g�̎擾
   for (auto&& object : GetObjServer().runObjects()) {
      // �v���C���[����Ȃ��ꍇ�������X�L�b�v����for������
      if (object->GetObjType() != Object::ObjectBase::ObjectType::Player) {
         continue;
      }
      // �v���C���[�̎Q�ƌ^�ɃL���X�g
      auto& player = dynamic_cast<Player::Player&>(*object);
      // �v���C���[�̉E��̍��W�̎擾
      _plyRightHandVec = player.modelAnimeComponent().GetFrameChildPosion("Kamilla_kari_Reference", "Kamilla_kari_RightFingerBase");
      // �v���C���[�̉E��̍��W�ւ̃x�N�g���̎擾
      auto toRightHand = _plyRightHandVec - _position;
      // �P�ʉ�����
      toRightHand.Normalized();
      // �����𖳂���
      auto [x, y, z] = toRightHand.GetVec3();
      auto moveVec = Vector4(x, 0.0, z);
      // �ʒu�������𖳂������v���C���[�̉E��̍��W�֋߂Â���
      _position = _position + moveVec * UpSpeed;
      // for���𔲂���
      break;
   }
   // �㉺�ɂӂ�ӂ킳���邽�߂Ɏg�p����T�C���̒l���擾���邽�߂̊p�x�̍X�V
   _upDownAngle += 2.0;
   // �p�x�����W�A���ɂ���
   auto upDownRadian = AppFrame::Math::Utility::DegreeToRadian(_upDownAngle);
   // �T�C���̒l���擾
   auto upDownSinValue = std::sin(upDownRadian);
   // ����̒l��-1�{����1�{�͈̔͂ő����Ă������������߂�
   auto y = UpDownRange * upDownSinValue;
   // �����̍X�V
   _position.Add(0.0, y, 0.0);
}

void FallObject::Up() {
   // �I�u�W�F�N�g�ꊇ�Ǘ��N���X�̃I�u�W�F�N�g�̎擾
   for (auto&& object : GetObjServer().runObjects()) {
      // �v���C���[����Ȃ��ꍇ�������X�L�b�v����for������
      if (object->GetObjType() != Object::ObjectBase::ObjectType::Player) {
         continue;
      }
      // �v���C���[�̎Q�ƌ^�ɃL���X�g
      auto& player = dynamic_cast<Player::Player&>(*object);
      // �v���C���[�̉E��̍��W�̎擾
      _plyRightHandVec = player.modelAnimeComponent().GetFrameChildPosion("Kamilla_kari_Reference", "Kamilla_kari_RightFingerBase");
      // for���𔲂���
      break;
   }
   // �ʒu�̍���������̒l���傫���ꍇ
   if (_position.GetY() >= UpValue) {
      // �v���C���[�̉E��̍��W��Y�����Ɋ���̒l����
      _plyRightHandVec.Add(0.0, RightHandUpValue, 0.0);
      // �v���C���[�̉E��̍��W�Ɋ���̒l�����𑫂����ʒu�ւ̃x�N�g���̎擾
      auto moveVec = _plyRightHandVec - _position;
      // �P�ʉ�����
      moveVec.Normalized();
      // �ʒu���v���C���[�̉E��̍��W�Ɋ���̒l�����𑫂����ʒu�֋߂Â���
      _position = _position + moveVec * UpSpeed;
      // �������v���C���[�̉E��̍��W�Ɋ���̒l�����𑫂����ʒu�̍����ȏ�̏ꍇ
      if (_position.GetY() >= _plyRightHandVec.GetY()) {
         // ������Ԃ�
         _stateServer->GoToState("Save");
      }
   }
   // �ʒu�̍���������̒l��菬�����ꍇ
   else {
      // ����̒l�����𑝂₷
      _position.Add(0.0, UpSpeed, 0.0);
   }
}

void FallObject::Shoot() {
   auto move = _shootVec * ShootSpeed;
   _position = _position + move;
}

void FallObject::StateBase::Draw() {
   _owner._modelAnimeComponent->Draw();
#ifdef _DEBUG
   auto pos = AppFrame::Math::ToDX(_owner._position);
   auto radian = static_cast<float>(Range);
   DrawSphere3D(pos, radian, 10, GetColor(0, 0, 0), GetColor(0, 0, 0), FALSE);

   auto pos1 = _owner._position + Vector4(0.0, CapsulePos1, 0.0);
   auto pos2 = _owner._position + Vector4(0.0, CapsulePos2, 0.0);
   radian = static_cast<float>(CapsuleRadius);

   DrawCapsule3D(AppFrame::Math::ToDX(pos1), AppFrame::Math::ToDX(pos2), radian, 20, GetColor(0, 255, 0), GetColor(0, 0, 0), FALSE);
#endif
}

void FallObject::StateIdle::Enter() {
   _owner._isFall = false;
}

void FallObject::StateIdle::Input(InputManager& input) {
   if (input.GetXJoypad().LeftTrigger() >= 20) {
      _owner.HitCheckFromPlayerPoint();
   }
}

void FallObject::StateIdle::Update() {
   _owner._collisionComponent->PlayerFromFallObjectModel(_owner._isFall);
   _owner._collisionComponent->PlayerFromObjectRange();
   _owner._collisionComponent->GatlingFromObjectModel();
   _owner._collisionComponent->FallObjectFromLaser();
   _owner.HitCheckFromLaser();
}

void FallObject::StateFall::Enter() {
   _owner._fallTimer = 0.0;
}

void FallObject::StateFall::Input(InputManager& input) {
   
}

void FallObject::StateFall::Update() {
   auto posY = (0.5 * Gravity * _owner._fallTimer * _owner._fallTimer);

   _owner._position.Add(0.0, -posY, 0.0);

   if (_owner._position.GetY() <= 0.0) {
      auto [oldPosX, oldPosY, oldPosZ] = _owner._position.GetVec3();
      _owner._position = AppFrame::Math::Vector4(oldPosX, 0.0, oldPosZ);
      auto efcFall = std::make_unique<Effect::EffectObjectFall>(_owner._gameMain, "ObjectFall");
      efcFall->position(_owner._position);
      efcFall->speed(2.0);
      _owner.GetEfcServer().Add(std::move(efcFall));
      // �c���I�u�W�F�N�g�łȂ��ꍇ���S��Ԃ�
      if (!_owner._residual) {
         _owner._stateServer->GoToState("Die");
      }
      // �c���I�u�W�F�N�g�̏ꍇ�ҋ@��Ԃ�
      else {
         _owner._stateServer->GoToState("Idle");
      }
   }

   _owner._collisionComponent->PlayerFromFallObjectModel(_owner._isFall);

   ++_owner._fallTimer;
}

void FallObject::StateFall::Draw() {
   FallObject::StateBase::Draw();
   auto pointPosition = _owner._position;
   pointPosition.SetY(FallPointPosY);
   _owner.GetTexComponent().DrawBillBoard(pointPosition, FallPointScale,DefaultPointAngle, _owner._fallPointHandles, FallPointAnimeSpeed);
}

void FallObject::StateUp::Enter() {
   _owner._vecBeforeSave = _owner._position;
   _owner.residual(false);
}

void FallObject::StateUp::Update() {
   // �㏸������
   _owner.Up();
   // �����蔻��Ǘ��N���X�Ńv���C���[���m�b�N�o�b�N���Ă��邩�m�F
   _owner._collisionComponent->PlayerKnockBack();
   // �v���C���[���m�b�N�o�b�N���Ă��邩�m�F
   _owner.CheckPlayerKnockBack();
}

void FallObject::StateUp::Exit() {

}

void FallObject::StateSave::Enter() {
   for (auto&& object : _owner.GetObjServer().runObjects()) {
      if (object->GetObjType() != Object::ObjectBase::ObjectType::Player) {
         return;
      }
      auto& player = dynamic_cast<Player::Player&>(*object);
      player.objectShoot(true);
      break;
   }
   _owner._efcUp = std::make_unique<Effect::EffectObjectUp>(_owner._gameMain, "ObjectUp");
   auto efcPos = _owner._position;
   efcPos.SetY(_owner._position.GetY() + UpEffectDiffY);
   _owner._efcUp->position(efcPos);
   _owner._efcUp->Init();
   Update();
}

void FallObject::StateSave::Input(InputManager& input) {
   if (input.GetXJoypad().RBClick()) {
      _owner._stateServer->GoToState("Shoot");
   }
}

void FallObject::StateSave::Update() {
    // �������Ȃ����]������
   _owner.Save();
   // �����蔻��Ǘ��N���X�Ńv���C���[���m�b�N�o�b�N���Ă��邩�m�F
   _owner._collisionComponent->PlayerKnockBack();
   // �v���C���[���m�b�N�o�b�N���Ă��邩�m�F
   _owner.CheckPlayerKnockBack();
   auto efcPos = _owner._position;
   efcPos.SetY(_owner._position.GetY() + UpEffectDiffY);
   _owner._efcUp->position(efcPos);
   _owner._efcUp->Update();
}

void FallObject::StateSave::Exit() {
   _owner._efcUp->StopEffect();
}

void FallObject::StateShoot::Enter() {
   _owner._shootVec = _owner.GetObjServer().GetVecData("CamTarget") - _owner._position;
   _owner._shootVec.Normalized();
}

void FallObject::StateShoot::Input(InputManager& input) {

}

void FallObject::StateShoot::Update() {
   _owner.Shoot();

   _owner._collisionComponent->LargeEnemyFromObjectModel();
   _owner._collisionComponent->PoorEnemyGatlingFromObjectModel();
   _owner._collisionComponent->FallObjectFromLaser();
   _owner._collisionComponent->OutStage();
   _owner.HitCheckFromLargeEnemy();
   _owner.HitCheckFromLaser();
   _owner.HitCheckFromPoorEnemyGatling();
   _owner.OutStageCheck();
}

void FallObject::StateDie::Enter() {

}

void FallObject::StateDie::Input(InputManager& input) {

}

void FallObject::StateDie::Update() {
   _owner.SetDead();
}
