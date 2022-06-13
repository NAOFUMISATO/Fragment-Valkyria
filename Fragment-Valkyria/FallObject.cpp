
/*****************************************************************//**
 * \file   FallObject.cpp
 * \brief  �����I�u�W�F�N�g�̏������񂷃N���X
 * 
 * \author YUSUKEAHAMEDO
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
#include "EffectObjectHit.h"
#include "ParamFallObject.h"

using namespace FragmentValkyria::Enemy;

namespace {
   constexpr auto DefaultPointAngle = 0.0;                                       //!< ������ꏊ�̃r���{�[�h�̉�]�p�x
   constexpr auto UpEffectDiffY = 20.0;                                          //!< �G�t�F�N�g�̍���
}

FallObject::FallObject(Game::GameMain& gameMain) : ObjectBase{ gameMain } {
   _param = std::make_unique<Param::ParamFallObject>(_gameMain,"fallobject");
   _collParam = std::make_unique<Param::ParamCollision>(_gameMain, "collision");
}

void FallObject::Init() {
   auto modelHandle = _modelAnimeComponent->modelHandle();
   _collision = MV1SearchFrame(modelHandle, _collisionName.data());
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
   // �����蔻�茋�ʃN���X�̎Q�Ƃ̎擾
   auto report = _collisionComponent->report();
   // �����蔻�茋�ʂ̊m�F
   if (report.id() == Collision::CollisionComponent::ReportId::HitFromPlayer) {
      // �v���C���[�Ɠ������Ă�����㏸��Ԃ�
      _stateServer->GoToState("Up");
   }
}

void FallObject::HitCheckFromLargeEnemy() {
   // �����蔻�茋�ʃN���X�̎Q�Ƃ̎擾
   auto report = _collisionComponent->report();
   // �����蔻�茋�ʂ̊m�F
   if (report.id() == Collision::CollisionComponent::ReportId::HitFromLargeEnemy) {
      // ���[�W�G�l�~�[�Ɠ������Ă����玀�S��Ԃ�
      auto efcHit = std::make_unique<Effect::EffectObjectHit>(_gameMain, "ObjectHit");
      efcHit->position(_position);
      GetEfcServer().Add(std::move(efcHit));
      GetSoundComponent().Play("ObjectHit", _position);
      _stateServer->GoToState("Die");
   }
}

void FallObject::HitCheckFromLaser() {
   // �����蔻�茋�ʃN���X�̎Q�Ƃ̎擾
   auto report = _collisionComponent->report();
   // �����蔻�茋�ʂ̊m�F
   if (report.id() == Collision::CollisionComponent::ReportId::HitFromLaser) {
      // ���[�U�[�Ɠ������Ă����玀�S��Ԃ�
      _stateServer->GoToState("Die");
   }
}

void FallObject::HitCheckFromPoorEnemy() {
   // �����蔻�茋�ʃN���X�̎Q�Ƃ̎擾
   auto report = _collisionComponent->report();
   // �����蔻�茋�ʂ̊m�F
   if (report.id() == Collision::CollisionComponent::ReportId::HitFromPoorEnemy) {
      auto efcHit = std::make_unique<Effect::EffectObjectHit>(_gameMain, "ObjectHit");
      efcHit->position(_position);
      GetEfcServer().Add(std::move(efcHit));
      GetSoundComponent().Play("ObjectHit",_position);
      // �G���G�Ɠ��������玀�S��Ԃ�
      _stateServer->GoToState("Die");
   }
}

void FallObject::OutStageCheck() {
   // �����蔻�茋�ʃN���X�̎Q�Ƃ̎擾
   auto report = _collisionComponent->report();
   // �����蔻�茋�ʂ̊m�F
   if (report.id() == Collision::CollisionComponent::ReportId::OutStage) {
      // �X�e�[�W�̊O�ɂ����玀�S��Ԃ�
      _stateServer->GoToState("Die");
   }
}

void FallObject::CheckPlayerKnockBack() {
   // �����蔻�茋�ʃN���X����v���C���[���m�b�N�o�b�N���Ă��邩�擾
   auto result = _collisionComponent->knockBack();
   // �m�b�N�o�b�N���Ă����玀�S��Ԃ�
   if (result) {
      _stateServer->GoToState("Die");
   }
}

void FallObject::Save() {
   /**
    * \brief double�^�̒l�𕶎���Ŏw�肵�A�l�Ǘ��N���X����擾����
    * \param paramName �l���w�肷�镶����
    * \return ������ɂ��w�肳�ꂽ�l
    */
   const auto _DoubleParam = [&](std::string paramName) {
      return _param->GetDoubleParam(paramName);
   };
   // �s�K���ȉ�]�������邽�߂Ɏg���T�C���̒l���擾���邽�߂Ɏg���p�x�̍X�V
   _rotateAngle += 1.0;
   // �p�x�����W�A���ɂ���
   auto radian = AppFrame::Math::Utility::DegreeToRadian(_rotateAngle);
   // �T�C���̒l���擾
   auto sinValue = std::sin(radian);
   // ����̊p�x��-1�{����1�{�͈̔͂Ŋp�x�����߂�
   auto xyz = _DoubleParam("rotate_angle") * sinValue;
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
      _position = _position + moveVec * _DoubleParam("up_speed");
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
   auto y = _DoubleParam("updown_range") * upDownSinValue;
   // �����̍X�V
   _position.Add(0.0, y, 0.0);
}

void FallObject::Up() {
   /**
    * \brief double�^�̒l�𕶎���Ŏw�肵�A�l�Ǘ��N���X����擾����
    * \param paramName �l���w�肷�镶����
    * \return ������ɂ��w�肳�ꂽ�l
    */
   const auto _DoubleParam = [&](std::string paramName) {
      return _param->GetDoubleParam(paramName);
   };
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
   const auto UpSpeed = _DoubleParam("up_speed");
   // �ʒu�̍���������̒l���傫���ꍇ
   if (_position.GetY() >= _DoubleParam("up_value")) {
      // �v���C���[�̉E��̍��W��Y�����Ɋ���̒l����
      _plyRightHandVec.Add(0.0, _DoubleParam("right_hand_up_value"), 0.0);
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
   // �ړ��ʂ̐ݒ�
   auto move = _shootVec * _param->GetDoubleParam("shoot_speed");
   // �ʒu�̍X�V
   _position = _position + move;
}

void FallObject::SetCapsulePos() {
   /**
    * \brief double�^�̒l�𕶎���Ŏw�肵�A�l�Ǘ��N���X����擾����
    * \param paramName �l���w�肷�镶����
    * \return ������ɂ��w�肳�ꂽ�l
    */
   const auto _DoubleParam = [&](std::string paramName) {
      return _collParam->GetDoubleParam(paramName);
   };
   // ���f���̉�]�̃}�g���N�X���쐬
   auto [rx, ry, rz] = _rotation.GetVec3();
   auto world = AppFrame::Math::Matrix44();
   world.RotateZ(rz, false);
   world.RotateX(rx, false);
   world.RotateY(ry, false);
   // �J�v�Z���̈ʒu�̐ݒ�
   _capsulePos1 = _position + Vector4(0.0, _DoubleParam("fallobject_drum_capsule_pos1"), 0.0) * world;
   _capsulePos2 = _position + Vector4(0.0, _DoubleParam("fallobject_drum_capsule_pos2"), 0.0) * world;
}

void FallObject::StateBase::Draw() {
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
   // ����̃x�N�g���N���X�̈ʒu��DxLib��VECTOR�^�ɕϊ�
   auto pos = AppFrame::Math::ToDX(_owner._position);
   // float�^�ɃL���X�g
   auto radian = static_cast<float>(_DoubleParam("fallobject_range"));
   // ���̕`��
   DrawSphere3D(pos, radian, 10, GetColor(0, 0, 0), GetColor(0, 0, 0), FALSE);
   // float�^�ɃL���X�g
   radian = static_cast<float>(_DoubleParam("fallobject_drum_radius"));
   // �J�v�Z���̕`��
   DrawCapsule3D(AppFrame::Math::ToDX(_owner._capsulePos1), AppFrame::Math::ToDX(_owner._capsulePos2), radian, 20, GetColor(0, 255, 0), GetColor(0, 0, 0), FALSE);
#endif
}

void FallObject::StateIdle::Enter() {
   // ������ԂłȂ��Ɛݒ�
   _owner._isFall = false;
}

void FallObject::StateIdle::Input(InputManager& input) {
   // ���g���K�[�����͂���Ă�����
   if (input.GetXJoypad().LeftTrigger() >= 20) {
      // �v���C���[�������͈͓��ɂ��邩�m�F
      _owner.HitCheckFromPlayerPoint();
   }
}

void FallObject::StateIdle::Update() {
   // �J�v�Z���̈ʒu�̐ݒ�
   _owner.SetCapsulePos();
   // �����蔻�菈�����s���N���X�ŃI�u�W�F�N�g�������グ����͈͂Ƀv���C���[�����邩�m�F
   _owner._collisionComponent->PlayerFromObjectRange();
   // �����蔻�菈�����s���N���X�Ńv���C���[�������I�u�W�F�N�g�̃��f���Ɠ������Ă��邩�m�F
   _owner._collisionComponent->PlayerFromFallObjectModel(_owner._isFall);
   // �����蔻�菈�����s���N���X�ŃI�u�W�F�N�g�̃��f���ɃK�g�����O���������Ă��邩�m�F
   _owner._collisionComponent->GatlingFromObjectModel();
   // �����蔻�菈�����s���N���X�Ń��[�U�[����������I�u�W�F�N�g�ɓ������Ă��邩�m�F
   _owner._collisionComponent->FallObjectFromLaser();
   // ���[�U�[�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromLaser();
}

void FallObject::StateFall::Enter() {
   // ������Ԃ̐i���̏����l�̐ݒ�
   _owner._fallTimer = 0.0;
}

void FallObject::StateFall::Update() {
   // Y���W�̈ʒu�̕ω��ʂ̎擾
   auto posY = (0.5 * _owner._param->GetDoubleParam("gravity") * _owner._fallTimer * _owner._fallTimer);
   // �ʒu�̍X�V
   _owner._position.Add(0.0, -posY, 0.0);
   // Y���W���n�ʂ�菬�����Ȃ�����
   if (_owner._position.GetY() <= 0.0) {
      // �ʒu�̕���
      auto [oldPosX, oldPosY, oldPosZ] = _owner._position.GetVec3();
      // �ʒu�̍����𖳂����Đݒ�
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
         _owner.GetSoundComponent().Play("ObjectFall",_owner._position);
      }
   }
   // �J�v�Z���̈ʒu�̐ݒ�
   _owner.SetCapsulePos();
   // �����蔻�菈�����s���N���X�Ńv���C���[�������I�u�W�F�N�g�̃��f���Ɠ������Ă��邩�m�F
   _owner._collisionComponent->PlayerFromFallObjectModel(_owner._isFall);
   // ������Ԃ̐i���̍X�V
   ++_owner._fallTimer;
}

void FallObject::StateFall::Draw() {
   /**
    * \brief double�^�̒l�𕶎���Ŏw�肵�A�l�Ǘ��N���X����擾����
    * \param paramName �l���w�肷�镶����
    * \return ������ɂ��w�肳�ꂽ�l
    */
   const auto _DoubleParam = [&](std::string paramName) {
      return _owner._param->GetDoubleParam(paramName);
   };
   // ��Ԃ̊��N���X�̕`�揈��
   FallObject::StateBase::Draw();
   auto pointPosition = _owner._position;
   pointPosition.SetY(_DoubleParam("fallpoint_pos_y"));
   _owner.GetTexComponent().DrawBillBoard(pointPosition, _DoubleParam("fallpoint_scale"),
      DefaultPointAngle, _owner._fallPointHandles, _owner._param->GetIntParam("fallpoint_animespeed"));
}

void FallObject::StateUp::Enter() {
   // �c���I�u�W�F�N�g�łȂ��Ɛݒ�
   _owner.residual(false);
   _owner._efcUp = std::make_unique<Effect::EffectObjectUp>(_owner._gameMain, "ObjectUp");
   auto efcPos = _owner._position;
   efcPos.SetY(_owner._position.GetY() + UpEffectDiffY);
   _owner._efcUp->position(efcPos);
   _owner._efcUp->Init();
   _owner._efcBorn = true;
   // �X�V����
   Update();
}

void FallObject::StateUp::Update() {
   // �㏸������
   _owner.Up();
   // �J�v�Z���̈ʒu�̐ݒ�
   _owner.SetCapsulePos();
   // �����蔻��Ǘ��N���X�Ńv���C���[���m�b�N�o�b�N���Ă��邩�m�F
   _owner._collisionComponent->PlayerKnockBack();
   // �v���C���[���m�b�N�o�b�N���Ă��邩�m�F
   _owner.CheckPlayerKnockBack();
   auto efcPos = _owner._position;
   efcPos.SetY(_owner._position.GetY() + UpEffectDiffY);
   _owner._efcUp->position(efcPos);
   _owner._efcUp->Update();
}

void FallObject::StateSave::Enter() {
   // �I�u�W�F�N�g�ꊇ�Ǘ��N���X�̊e�I�u�W�F�N�g�̎擾
   for (auto&& object : _owner.GetObjServer().runObjects()) {
      // �v���C���[�łȂ���Ώ������X�L�b�v����for������
      if (object->GetObjType() != Object::ObjectBase::ObjectType::Player) {
         continue;
      }
      // �v���C���[�̎Q�ƌ^�ɃL���X�g
      auto& player = dynamic_cast<Player::Player&>(*object);
      // �v���C���[���I�u�W�F�N�g��łĂ�Ɛݒ�
      player.objectShoot(true);
      break;
   }
}

void FallObject::StateSave::Input(InputManager& input) {
   if (input.GetXJoypad().RBClick()) {
      _owner._stateServer->GoToState("Shoot");
   }
}

void FallObject::StateSave::Update() {
   // �������Ȃ����]������
   _owner.Save();
   // �J�v�Z���̈ʒu�̐ݒ�
   _owner.SetCapsulePos();
   // �����蔻��Ǘ��N���X�Ńv���C���[���m�b�N�o�b�N���Ă��邩�m�F
   _owner._collisionComponent->PlayerKnockBack();
   // �v���C���[���m�b�N�o�b�N���Ă��邩�m�F
   _owner.CheckPlayerKnockBack();
   auto efcPos = _owner._position;
   efcPos.SetY(_owner._position.GetY() + UpEffectDiffY);
   _owner._efcUp->position(efcPos);
   _owner._efcUp->Update();
}

void FallObject::StateShoot::Enter() {
   // �J�����̒����_�ւ̃x�N�g����ݒ�
   _owner._shootVec = _owner.GetObjServer().GetVecData("CamTarget") - _owner._position;
   // �P�ʉ�����
   _owner._shootVec.Normalized();
}

void FallObject::StateShoot::Update() {
   // �J�����̒����_�֐i�ޏ���
   _owner.Shoot();
   // �J�v�Z���̈ʒu�̐ݒ�
   _owner.SetCapsulePos();
   // �����蔻�菈�����s���N���X�Ń��[�W�G�l�~�[�̃��f���ɗ����I�u�W�F�N�g���������Ă��邩�m�F
   _owner._collisionComponent->LargeEnemyFromObjectModel();
   // �����蔻�菈�����s���N���X�ŎG���G�̃��f���ɗ����I�u�W�F�N�g���������Ă��邩�m�F
   _owner._collisionComponent->PoorEnemyGatlingFromObjectModel();
   // �����蔻�菈�����s���N���X�Ń��[�U�[�������I�u�W�F�N�g�ɓ������Ă��邩�m�F
   _owner._collisionComponent->FallObjectFromLaser();
   // �����蔻�菈�����s���N���X�ŃX�e�[�W�O�ɂ��邩�m�F
   _owner._collisionComponent->OutStage();
   // ���[�W�G�l�~�[�̃��f���Ɠ������Ă��邩�m�F
   _owner.HitCheckFromLargeEnemy();
   // ���[�U�[�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromLaser();
   // �G���G�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromPoorEnemy();
   // �X�e�[�W�O�ɂ��邩�m�F
   _owner.OutStageCheck();
   auto efcPos = _owner._position;
   efcPos.SetY(_owner._position.GetY() + UpEffectDiffY);
   _owner._efcUp->position(efcPos);
   _owner._efcUp->Update();
}

void FallObject::StateDie::Update() {
   if (_owner._efcBorn) {
      _owner._efcUp->StopEffect();
   }
   // ���S��Ԃɐݒ�
   _owner.SetDead();
}
