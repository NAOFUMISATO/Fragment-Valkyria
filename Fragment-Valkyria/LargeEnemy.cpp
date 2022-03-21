
/*****************************************************************//**
 * \file   LargeEnemy.cpp
 * \brief  ���[�W�G�l�~�[�̏������񂷃N���X
 * 
 * \author AHMD2000, NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "LargeEnemy.h"
#include "CameraComponent.h"
#include "GameMain.h"
#include "ObjectFactory.h"
#include "CollisionComponent.h"
#include "ModelAnimeComponent.h"
#include "PoorEnemyGatling.h"
#include "ObjectServer.h"
#include "ObjectFactory.h"
#include "FallObject.h"
#include "EffectServer.h"
#include "EffectBossCharge.h"
#include "EffectBossStan.h"
#include "EffectBossFall.h"
#include "EffectPreliminaryLight.h"
#include "EffectBossCrash.h"

namespace {
   auto largeEnemyParamMap = AppFrame::Resource::LoadParamJson::GetParamMap("largeenemy", {
      "gatling_frame", "max_stun", "stun_decrease", "object_stun_value",
      "bullet_stun_value", "hp", "gravity", "consecutive_fall_object_frame",
      "consecutive_num", "action_cooltime", "object_max_num","max_wave","max_poorenemy" });
   const int GatlingFrame = largeEnemyParamMap["gatling_frame"];                                                   //!< �K�g�����O��łt���[���̊Ԋu
   const double MaxStun = largeEnemyParamMap["max_stun"];                                                          //!< �X�^���l�̍ő�l
   const double StunDecrease = largeEnemyParamMap["stun_decrease"];                                                //!< ���t���[�����炷�X�^���l�̒l
   const double ObjectStunValue = largeEnemyParamMap["object_stun_value"];                                         //!< �I�u�W�F�N�g�U�����󂯂����ɑ�����X�^���l
   const double BulletStunValue = largeEnemyParamMap["bullet_stun_value"];                                         //!< ���u��U�����󂯂����ɑ�����X�^���l
   const double MaxHitPoint = largeEnemyParamMap["hp"];                                                            //!< �q�b�g�|�C���g�̍ő�l
   const double Gravity = largeEnemyParamMap["gravity"];                                                           //!< �d��
   const int ConsecutiveFallObjectFrame = largeEnemyParamMap["consecutive_fall_object_frame"];                     //!< �I�u�W�F�N�g�A�������U���̃t���[����
   const int ConsecutiveNum = largeEnemyParamMap["consecutive_num"];                                               //!< �I�u�W�F�N�g�A�������U���̘A����
   const int ActionCoolTime = largeEnemyParamMap["action_cooltime"];                                               //!< �s�����s���N�[���^�C��
   const int MaxNum = largeEnemyParamMap["object_max_num"];                                                        //!< �����I�u�W�F�N�g�̍ő吔
   const int MaxWave= largeEnemyParamMap["max_wave"];                                                              //!< �ő�E�F�C�u��
   const int MaxPoorEnemy = largeEnemyParamMap["max_poorenemy"];                                                   //!< �G���G�̍ő吔

   constexpr auto FootStepHeight = 40.0;                                                                           //!< �����Ԏ��̑�����������
   constexpr auto LaserDiffPos = 150.0;                                                                            //!< ���[�U�[�����ʒu�Ɉ�ԋ߂��ʒu�̃t���[������̃I�t�Z�b�g
   constexpr auto LaserIrradiationTime = 155.f;                                                                    //!< �A�j���[�V�����n�܂��Ă���̃��[�U�[�̏Ǝˎ���
   constexpr auto MinWave = 2;
}

using namespace FragmentValkyria::Enemy;

LargeEnemy::LargeEnemy(Game::GameMain& gameMain) : ObjectBase{ gameMain } {

}

void LargeEnemy::Init() {
   // ���f���̃n���h���̎擾
   auto modelHandle = _modelAnimeComponent->modelHandle();
   // ���f���̑S�̂̃R���W�����t���[���̔ԍ��̎擾
   _wholeCollision = _modelAnimeComponent->FindFrame("S301_typeCO");
   // �R���W�����t���[�����i�r���b�V���Ƃ��Ďg�p
   MV1SetupCollInfo(modelHandle, _bodyCollision, 7, 6, 5);
   // ���f���̓��̂̃R���W�����t���[���̔ԍ��̎擾
   _bodyCollision = _modelAnimeComponent->FindFrame("pasted__body_collision");
   // �R���W�����t���[�����i�r���b�V���Ƃ��Ďg�p
   MV1SetupCollInfo(modelHandle, _bodyCollision, 6, 6, 5);
   // ���f���̎�_�̃R���W�����t���[���̔ԍ��̎擾
   _weakNessesCollision = _modelAnimeComponent->FindFrame("pasted__weak_collision");
   // �R���W�����t���[�����i�r���b�V���Ƃ��Ďg�p
   MV1SetupCollInfo(modelHandle, _weakNessesCollision, 1, 1, 1);
   // ���f���̊�̃R���W�����t���[���̔ԍ��̎擾
   _faceCollision = _modelAnimeComponent->FindFrame("pasted__face_collision");
   // �R���W�����t���[�����i�r���b�V���Ƃ��Ďg�p
   MV1SetupCollInfo(modelHandle, _faceCollision, 3, 2, 1);
   // �q�b�g�|�C���g�̐ݒ�
   _hp = MaxHitPoint;
   // �s���ɒǉ������Ԃ̕�����̓��I�z����쐬
   _actionList.emplace_back("FallObject");
   _actionList.emplace_back("Gatling");
   _actionList.emplace_back("Move");
   _actionList.emplace_back("Laser");
   _actionList.emplace_back("FanGatling");
}

void LargeEnemy::Input(InputManager& input) {
   // �e��Ԃ̓��͏�������
   _stateServer->Input(input);
}

void LargeEnemy::Update() {
   // �R���W�������̍X�V
   MV1RefreshCollInfo(_modelAnimeComponent->modelHandle(), _wholeCollision);
   MV1RefreshCollInfo(_modelAnimeComponent->modelHandle(), _bodyCollision);
   MV1RefreshCollInfo(_modelAnimeComponent->modelHandle(), _weakNessesCollision);
   MV1RefreshCollInfo(_modelAnimeComponent->modelHandle(), _faceCollision);
   // ��Ԃ̍X�V
   _stateServer->Update();
   // ���[���h�s��̍X�V
   ComputeWorldTransform();
   // ���f���̍X�V
   _modelAnimeComponent->Update();
   // �I�u�W�F�N�g�T�[�o�[�̎Q�Ƃ��擾
   auto& objServer = GetObjServer();
   // �I�u�W�F�N�g�T�[�o�[�Ɉʒu��ʒm
   objServer.RegistVector("LargeEnemyPos", _position);
   // �I�u�W�F�N�g�T�[�o�[�Ƀq�b�g�|�C���g��ʒm
   objServer.RegistDouble("LargeEnemyHP", _hp);
}

void LargeEnemy::Draw() {
   // �e��Ԃ̕`�揈������
   _stateServer->Draw();
}

void LargeEnemy::CreateGatling() {
   auto& objServer = GetObjServer();
   // �K�g�����O�𐶐�����ʒu���擾
   auto gatlingFramePos = _modelAnimeComponent->GetFrameChildPosion("root", "gatling3");
   // �K�g�����O�𐶐�������W��ݒ�
   objServer.RegistVector("GatlingPos", gatlingFramePos);
   // �K�g�����O��ł����̐ݒ�
   auto gatlingDirection = _position + _rotateDir * 100.0 - gatlingFramePos;
   // �I�u�W�F�N�g�T�[�o�[�ɃK�g�����O��ł�����ʒm
   objServer.RegistVector("GatlingMoveDirection", gatlingDirection);
   // �K�g�����O�𐶐����ăI�u�W�F�N�g�T�[�o�[�֒ǉ�
   auto gatling = gameMain().objFactory().Create("Gatling");
   objServer.Add(std::move(gatling));
}

void LargeEnemy::CreateLaser() {
   // ���[�U�[�𐶐����ăI�u�W�F�N�g�T�[�o�[�֒ǉ�
   auto laser = gameMain().objFactory().Create("Laser");
   GetObjServer().Add(std::move(laser));
}

void LargeEnemy::CreateFallObject() {
   using Vector4 = AppFrame::Math::Vector4;
   using Matrix44 = AppFrame::Math::Matrix44;
   // �������鐔�𐶐�����Ă��闎���I�u�W�F�N�g�̐��ɐݒ�
   _createNum = 3;
   // �I�u�W�F�N�g�T�[�o�[�̊e�I�u�W�F�N�g�̎擾
   for (auto&& object : _gameMain.objServer().runObjects()) {
      // �����I�u�W�F�N�g����Ȃ������珈�����X�L�b�v���Ė߂�
      if (object->GetObjType() != Object::ObjectBase::ObjectType::FallObject) {
         continue;
      }
      // �����I�u�W�F�N�g�������琶������Ă��闎���I�u�W�F�N�g�̐��𑝂₷
      ++_createNum;
   }
   // ��������Ă��闎���I�u�W�F�N�g�̐��������I�u�W�F�N�g�̍ő吔���傫�����m�F
   if (_createNum > MaxNum) {
      // ��������Ă��闎���I�u�W�F�N�g�̐��������I�u�W�F�N�g�̍ő吔���傫��������I�u�W�F�N�g�T�[�o�[�̊e�I�u�W�F�N�g���擾
      for (auto&& object : _gameMain.objServer().runObjects()) {
         // �����I�u�W�F�N�g����Ȃ������珈�����X�L�b�v���Ė߂�
         if (object->GetObjType() != Object::ObjectBase::ObjectType::FallObject) {
            continue;
         }
         // �����I�u�W�F�N�g�������痎���I�u�W�F�N�g�̎Q�ƂɃL���X�g
         auto& fallObject = dynamic_cast<Enemy::FallObject&>(*object);
         // �����I�u�W�F�N�g���c���I�u�W�F�N�g���m�F
         if (fallObject.residual()) {
            // �����I�u�W�F�N�g���c���I�u�W�F�N�g�������玀�S��Ԃɐݒ�
            object->SetDead();
            // ��������Ă��闎���I�u�W�F�N�g�̐������炷
            --_createNum;
            // ��������Ă��闎���I�u�W�F�N�g�̐��������I�u�W�F�N�g�̍ő吔�ȉ��ɂȂ����珈�����I��
            if (_createNum <= MaxNum) {
               break;
            }
         }
      }
   }
   // �E�Ɋ���̊p�x��]������}�g���N�X�̐���
   auto rightTransMatrix = Matrix44();
   rightTransMatrix.RotateY(30.0, true);
   // ���Ɋ���̊p�x��]������}�g���N�X�̐���
   auto leftTransMatrix = Matrix44();
   leftTransMatrix.RotateY(-30.0, true);
   // �����_���ɉE�����Ɋ���̊p�x��]������}�g���N�X�̍쐬
   auto randomdegree = AppFrame::Math::Utility::GetRandom(0, 1) ? 60.0 : -60.0;
   auto randomTransMatrix = Matrix44();
   randomTransMatrix.RotateY(randomdegree, true);
   // �v���C���[�̈ʒu�̎擾
   auto playerPos = GetObjServer().GetVecData("PlayerPos");
   // �v���C���[�ւ̃x�N�g���̎擾
   auto MoveVec = playerPos - _position;
   // �v���C���[�ւ̋����̎擾
   auto LargeEnemyToPlyDistance = MoveVec.Lenght();
   // �v���C���[�ւ̃x�N�g����P�ʉ�����
   MoveVec.Normalized();
   // �v���C���[�ւ̌����̃x�N�g�����E�Ɋ���̊p�x��]������
   auto rightMoveVec = MoveVec * rightTransMatrix;
   // �v���C���[�ւ̌����̃x�N�g�������Ɋ���̊p�x��]������
   auto leftMoveVec = MoveVec * leftTransMatrix;
   // �v���C���[�ւ̌����̃x�N�g�����E�����Ƀ����_���Ɋ���̊p�x��]������
   auto randomtMoveVec = MoveVec * randomTransMatrix;
   // ����̊p�x��]�������������痎���I�u�W�F�N�g�𗎂Ƃ������̐ݒ�
   auto distance = LargeEnemyToPlyDistance * 0.5;
   // �����_���ɉ�]�������������痎���I�u�W�F�N�g�𗎂Ƃ������̐ݒ�
   auto randomDistance = LargeEnemyToPlyDistance * 0.3;
   // �����I�u�W�F�N�g�𗎂Ƃ��ʒu�̔z��̍쐬
   std::array<Vector4, 4> startPosition = {
      playerPos + Vector4(0.0, 500.0, 0.0),
      _position + Vector4(0.0, 500.0, 0.0) + (rightMoveVec * distance),
      _position + Vector4(0.0, 500.0, 0.0) + (leftMoveVec * distance),
      _position + Vector4(0.0, 500.0, 0.0) + (randomtMoveVec * randomDistance),
   };
   // �����I�u�W�F�N�g�𐶐����I�u�W�F�N�g�T�[�o�[�ɒǉ�
   for (auto i = 0; i < 4; ++i) {
      auto fallObject = gameMain().objFactory().Create("FallObject");
      // �������ꂽ�񐔖ڂ̈ʒu�̔z��̗v�f�Ɉʒu��ݒ�
      fallObject->position(startPosition[i]);
      GetObjServer().Add(std::move(fallObject));
   }
}

void LargeEnemy::SetLaserPosition() {
   // �I�u�W�F�N�g�T�[�o�[�̊e�I�u�W�F�N�g�̎擾
   for (auto&& objects : _gameMain.objServer().runObjects()) {
      // �����I�u�W�F�N�g���m�F
      if (objects->GetObjType() == Object::ObjectBase::ObjectType::FallObject) {
         // �����I�u�W�F�N�g��������
         // �����I�u�W�F�N�g����v���C���[�ւ̃x�N�g���̎擾
         auto fallObjectToPly = _gameMain.objServer().GetVecData("PlayerPos") - objects->position();
         // �����I�u�W�F�N�g����v���C���[�ւ̃x�N�g���̊e�����𕪉�
         auto [x, y, z] = fallObjectToPly.GetVec3();
         // �����I�u�W�F�N�g����v���C���[�ւ̃x�N�g���̋�����2����擾
         auto checkSize = x * x + y * y + z * z;
         // �����I�u�W�F�N�g����v���C���[�ւ̃x�N�g���̋�����2��Ɨ����I�u�W�F�N�g�̈ʒu�̃y�A���쐬
         auto checkPair = std::make_pair(checkSize, objects->position());
         // �����I�u�W�F�N�g����v���C���[�ւ̃x�N�g���̋�����2��Ɨ����I�u�W�F�N�g�̈ʒu�̃y�A�̓��I�z��֒ǉ�
         _objectDistance.emplace_back(checkPair);
         continue;
      }
   }
   // �����I�u�W�F�N�g����v���C���[�ւ̋�����2��̒l���ׂď����ɕ��ѕς���
   std::sort(_objectDistance.begin(), _objectDistance.end());
   // �����I�u�W�F�N�g���Ȃ������ꍇ�v���C���[�̈ʒu�Ƀ��[�U�[��ł�����ݒ�
   if (_objectDistance.empty()) {
      GetObjServer().RegistVector("LaserDirectionPos", _gameMain.objServer().GetVecData("PlayerPos"));
   }
   // �����I�u�W�F�N�g���������ꍇ
   else {
      // ������2��̒l����ԃv���C���[�Ƌ߂������I�u�W�F�N�g�̈ʒu�̐����𕪉�
      auto [x, y, z] = _objectDistance[0].second.GetVec3();
      // ���[�U�[�̑ł����ɍ������Ȃ����������I�u�W�F�N�g�ւ̌�����ݒ�
      GetObjServer().RegistVector("LaserDirectionPos", Vector4(x, 0.0, z));
   }
   // �e�����I�u�W�F�N�g����v���C���[�ւ̋�����2��Ɗe�����I�u�W�F�N�g�̈ʒu�̃y�A�̓��I�z��̂��ׂĂ̗v�f���폜����
   _objectDistance.clear();
}

void LargeEnemy::HitCheckFromFallObject() {
   // �����蔻�茋�ʃN���X�̎Q�Ƃ̎擾
   auto report = _collisionComponent->report();
   // �����蔻�茋�ʂ̊m�F
   if (report.id() == Collision::CollisionComponent::ReportId::HitFromFallObject) {
      // �����I�u�W�F�N�g�Ɠ������Ă�����
      // �q�b�g�|�C���g���_���[�W�ʕ����炷
      _hp -= _collisionComponent->damage();
      // �_���[�W�ʂ�0����Ȃ������ꍇ
      if (_collisionComponent->damage() != 0.0) {
         // �X�^���l������̒l���₷
         _stunValue += ObjectStunValue;
      }
      // �q�b�g�|�C���g��0�ȉ��������ꍇ���S��Ԃ�
      if (_hp <= 0) {
         _stateServer->GoToState("Die");
         // �X�^���l�̐ݒ�
         _stunValue = 0.0;
      }
      // �����蔻��̌��ʂ𓖂����Ă��Ȃ��Ɛݒ�
      _collisionComponent->report().id(Collision::CollisionComponent::ReportId::None);
   }

}

void LargeEnemy::HitCheckFromBullet() {
   // �����蔻�茋�ʃN���X�̎Q�Ƃ̎擾
   auto report = _collisionComponent->report();
   // �����蔻�茋�ʂ̊m�F
   if (report.id() == Collision::CollisionComponent::ReportId::HitFromBullet) {
      // ���u��U���̒e�Ɠ������Ă�����
      // �q�b�g�|�C���g���_���[�W�ʕ����炷
      _hp -= _collisionComponent->damage();
      // �X�^���l������̒l���₷
      _stunValue += BulletStunValue;
      // �q�b�g�|�C���g��0�ȉ��������ꍇ���S��Ԃ�
      if (_hp <= 0) {
         _stateServer->GoToState("Die");
         // �X�^���l�̐ݒ�
         _stunValue = 0.0;
      }
      // �����蔻��̌��ʂ𓖂����Ă��Ȃ��Ɛݒ�
      _collisionComponent->report().id(Collision::CollisionComponent::ReportId::None);
   }
}

void LargeEnemy::Move(const Vector4& moved) {
   // �ړ��ʂ̃x�N�g���̐����𕪉�
   auto [x, y, z] = moved.GetVec3();
   // �ʒu��ۑ�
   auto position = _position;
   // x�����̈ړ���̈ʒu���擾
   position = _collisionComponent->LargeEnemyCheckStage(position, Vector4(x, y, 0.0));
   // z�����̈ړ���̈ʒu���擾
   position = _collisionComponent->LargeEnemyCheckStage(position, Vector4(0.0, y, z));
   // �ʒu��x��z�̐����̈ړ���̈ʒu��ݒ肷��
   _position = position;
}

void LargeEnemy::Action() {
   // �q�b�g�|�C���g���q�b�g�|�C���g�̍ő�l��50���ȉ��ɂȂ����ŏ��̍s���̏ꍇ
   if (_hp <= MaxHitPoint * 0.5 && _firstAngryAction) {
      // �e�s����Ԃւ̕�����̓��I�z��ɃI�u�W�F�N�g�A��������Ԃ�ǉ�
      _actionList.emplace_back("Consecutive");
      // �I�u�W�F�N�g�A��������Ԃ�
      _stateServer->GoToState("Consecutive");
      // �q�b�g�|�C���g���q�b�g�|�C���g�̍ő�l��50���ȉ��ɂȂ����ŏ��̍s���łȂ��Ɛݒ�
      _firstAngryAction = false;
      // �������X�L�b�v���Ė߂�
      return;
   }
   // �s��������͈͂𐧌������s����Ԃւ̕�����̓��I�z�񂪋󂾂�����e�s����Ԃւ̕�����̓��I�z���������
   if (_action.empty()) {
      _action = _actionList;
   }
   // �s����Ԃւ̕�����̓��I�z��̗v�f�̌����ő傾�����ꍇ
   if (_action.size() >= _actionList.size()) {
      // �ŏ��̍s����Ԃֈړ�
      _stateServer->GoToState(_action[0]);
      // �ŏ��̍s����Ԃւ̕�����𓮓I�z�񂩂�폜
      _action.erase(_action.begin());
   }
   else {
      // 0����s��������͈͂𐧌������s����Ԃւ̕�����̓��I�z��̗v�f�̌���-1�������͈̔͂̒l�������_���Ɏ擾
      auto random = AppFrame::Math::Utility::GetRandom(0, static_cast<int>(_action.size()) - 1);
      // �����_���Ɏ擾�����l�Ԗڂ̍s����Ԃֈړ�
      _stateServer->GoToState(_action[random]);
      // �ړ������s����Ԃւ̕�����𓮓I�z�񂩂�폜
      _action.erase(_action.begin() + random);
   }
}

void LargeEnemy::AugularRotate(bool& rotating) {
   // �e���x����]������
   _rotation.Add(0.0, _addRotate, 0.0);
   // �����Ă�������։�]������}�g���N�X�̍쐬
   Matrix44 rotateY = Matrix44();
   rotateY.RotateY(_rotation.GetY(), true);
   // �����Ō����Ă��������90�x��]�����������̃x�N�g���̍쐬
   Vector4 forward = Vector4(-1.0, 0.0, 0.0);
   // �t�H���[�h�x�N�g����90�x��]�������x�N�g���̍쐬
   forward = forward * rotateY;
   // �P�ʉ�������
   forward.Normalized();
   // ���������������ƃt�H���[�h�x�N�g����90�x��]�������x�N�g���̓��ς̌��ʂ��擾
   auto dot = _moved.Dot(forward);
   // �e���x��0���傫�����m�F
   if (_addRotate > 0.0) {
      // �e���x��0���傫���Ƃ����ς̌��ʂ�0��菬�����Ȃ������]���Ȃ��Ɛݒ�
      if (dot < 0) {
         rotating = false;
      }
   }
   else {
      // �e���x��0��菬�����Ƃ����ς̌��ʂ�0���傫���Ȃ������]���Ȃ��Ɛݒ�
      if (dot >= 0) {
         rotating = false;
      }
   }
}

void LargeEnemy::AreaRotate(bool& rotating) {
   // ���f�����f�t�H���g��Z���W�̃}�C�i�X�����������Ă���̂Ńt�H���[�h�x�N�g���𔽓]�����Ă���
   auto forward = GetForward() * -1.0;
   // ��]�̌����̃x�N�g����P�ʉ�����
   _rotateDir.Normalized();
   // �t�H���[�h�x�N�g���Ɖ�]�̌����̃x�N�g���Ƃ̓��ς̌��ʂ��m�F
   if (forward.Dot(_rotateDir) <= -0.9) {
      // ���ς̌��ʂ���^�t�ɋ߂������ꍇ
      // �x�N�g����3�x��]������}�g���N�X�����
      Matrix44 dirRotate;
      dirRotate.RotateY(3.0, true);
      // ��]�̌����̃x�N�g������]������
      _rotateDir = _rotateDir * dirRotate;
    }
   // ��]�̌����̃x�N�g��������̒l�傫������
   _rotateDir = _rotateDir * _rotateEnlarge;
   // �t�H���[�h�x�N�g���Ɖ�]�̌����̃x�N�g���Ƃ̎O�p�`�̖ʐς̎擾
   auto area = forward.Cross(_rotateDir);
   auto rotateValue = 0.5 * area.GetY();
   // �O�p�`�̖ʐς̒l��0�ɋ߂������ꍇ��]���Ȃ��Ɛݒ�
   if (rotateValue <= 0.0 && rotateValue >= -0.1 || rotateValue >= 0.0 && rotateValue <= 0.1) {
      rotating = false;
   }
   // �ʐς̒l����]������(�v�Z�̏��Ԃ��t�H���[�h�x�N�g��������������������̃x�N�g���ɂ��邱�ƂŌv�Z���ʂ̕��������̂܂ܗ��p���邱�ƂōŒZ�����̉�]���s��)
   _rotation.Add(0.0, rotateValue, 0.0);
}

void LargeEnemy::SetAddRotate() {
   // �����Ă�������։�]������}�g���N�X�̍쐬
   Matrix44 rotateY = Matrix44();
   rotateY.RotateY(_rotation.GetY(), true);
   // �����Ă��������90�x��]�������x�N�g���̍쐬
   Vector4 forward = Vector4(-1.0, 0.0, 0.0) * rotateY;
   // �P�ʉ�����
   forward.Normalized();
   // ���������������ƃt�H���[�h�x�N�g����90�x��]�������x�N�g���̓��ς̌��ʂ��擾
   _rotateDot = _moved.Dot(forward);
   // ���ς̌��ʂ�0���傫���ꍇ�e���x���v���X�ɂ���
   if (_rotateDot >= 0) {
      _addRotate = 1.0;
   }
   // ���ς̌��ʂ�0���傫���Ȃ��ꍇ�e���x���}�C�i�X�ɂ���
   else {
      _addRotate = -1.0;
   }
}

void LargeEnemy::StunCheck() {
   // �X�^���l�̍X�V
   _stunValue -= StunDecrease;
   // �X�^���l������̒l�ȏ�̏ꍇ
   if (_stunValue >= MaxStun) {
      // �C���Ԃ�
      _stateServer->GoToState("Stun");
   }
   // �X�^���l��0�ȉ��̏ꍇ
   else if (_stunValue <= 0.0) {
      // �X�^���l��0��
      _stunValue = 0.0;
   }
}

void LargeEnemy::StateBase::Draw() {
   // ���f���̕`�揈������
   _owner._modelAnimeComponent->Draw();
}

void LargeEnemy::StateFall::Enter() {
   // ���̏�ԂɂȂ������̃Q�[���̃t���[���J�E���g�̕ۑ�
   _stateCnt = _owner._gameMain.modeServer().frameCount();
   // ���f���̃A�j���[�V�����̐ݒ�
   _owner._modelAnimeComponent->ChangeAnime("idle", true);
}

void LargeEnemy::StateFall::Update() {
   // ���̏�Ԃɓ����Ă���̃t���[�����̎擾
   auto cnt = _owner._gameMain.modeServer().frameCount() - _stateCnt;
   // �ʒu��Y�����ɑ����Ă����l�̎擾
   auto addY = -0.5 * Gravity * cnt * cnt;
   // �ʒu�̍X�V
   _owner._position.Add(0.0, addY, 0.0);
   // �ʒu��Y������0�ȉ���������
   if (_owner._position.GetY() <= 0.0) {
      // �ʒu��Y������0�ɂ���
      auto [x, y, z] = _owner._position.GetVec3();
      _owner._position = Vector4(x, 0.0, z);
      // �ҋ@��Ԃ�
      _owner._stateServer->GoToState("Idle");
   }
   // �����蔻�菈�����s���N���X�Ńv���C���[�����[�W�G�l�~�[�Ɠ������Ă��邩�m�F
   _owner._collisionComponent->LargeEnemyFromPlayer();
}

void LargeEnemy::StateFall::Exit() {
   auto efcFall = std::make_unique<Effect::EffectBossFall>(_owner._gameMain,"BossFall");
   efcFall->position(_owner._position);
   _owner.GetEfcServer().Add(std::move(efcFall));
   _owner._cameraComponent->SetVibValue(0.0);
   _owner.GetSoundComponent().PlayLoop("BossBattleBgm");
}

void LargeEnemy::StateIdle::Enter() {
   // ���̏�ԂɂȂ������̃Q�[���̃t���[���J�E���g�̕ۑ�
   _stateCnt = _owner.gameMain().modeServer().frameCount();
   // ���f���̃A�j���[�V�����̐ݒ�
   _owner._modelAnimeComponent->ChangeAnime("idle", true);
}

void LargeEnemy::StateIdle::Update() {
   // �Q�[���̃t���[���J�E���g�̎擾
   auto gameCount = _owner.gameMain().modeServer().frameCount();
   // ���̏�Ԃɓ����Ă���̌o�߃t���[�����̎擾
   auto count = static_cast<int>(gameCount) - _stateCnt;
   // ���t���[������������s��������
   if (count >= ActionCoolTime) {
      _owner.Action();
   }
   // �����蔻�菈�����s���N���X�Ńv���C���[�����[�W�G�l�~�[�Ɠ������Ă��邩�m�F
   _owner._collisionComponent->LargeEnemyFromPlayer();
   // �����I�u�W�F�N�g�Ɠ����������m�F
   _owner.HitCheckFromFallObject();
   // ���u��U���̒e�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromBullet();
   // �X�^���l�̍X�V�Ɗm�F
   _owner.StunCheck();
}

void LargeEnemy::StateFallObject::Enter() {
   // ���f���̃A�j���[�V�����̐ݒ�
   _owner._modelAnimeComponent->ChangeAnime("object_attack", false);
}

void LargeEnemy::StateFallObject::Update() {
   // �A�j���[�V�������J��Ԃ����񐔂̎擾
   auto cnt = _owner._modelAnimeComponent->repeatedCount();
   // �A�j���[�V������1��s������ҋ@��Ԃ�
   if (cnt > 0) {
      // �J������U�������邽�߂ɃJ�����̐U���Ɏg��Y�̈ʒu��0.0�ɐݒ�
      _owner._cameraComponent->SetVibValue(0.0);
      // �����I�u�W�F�N�g�̐���
      _owner.CreateFallObject();
      SetPoorSpawn();
      _owner._stateServer->GoToState("Idle");
   }
   // �����蔻�菈�����s���N���X�Ńv���C���[�����[�W�G�l�~�[�Ɠ������Ă��邩�m�F
   _owner._collisionComponent->LargeEnemyFromPlayer();
   // �����I�u�W�F�N�g�Ɠ����������m�F
   _owner.HitCheckFromFallObject();
   // ���u��U���̒e�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromBullet();
   // �X�^���l�̍X�V�Ɗm�F
   _owner.StunCheck();
}

void LargeEnemy::StateGatling::Enter() {
   // �K�g�����O�U�������邽�߂̉�]�����Ă��Ȃ����Ɍo�߂�����t���[���J�E���g�̐ݒ�
   _gatlingFrameCnt = 0;
   // �K�g�����O�̒e��ł񐔂̐ݒ�
   _owner._gatlingCnt = 10;
   // ���������������̃x�N�g����傫������l��ݒ�
   _owner._rotateEnlarge = 50.0;
   // ���f���̃A�j���[�V�����̐ݒ�
   _owner._modelAnimeComponent->ChangeAnime("gatoring", true);
}

void LargeEnemy::StateGatling::Update() {
   // ��]���Ă��Ȃ����̃t���[���J�E���g������̒l�����傫������̃t���[�����o�߂��U�����Ă��Ȃ��ꍇ
   if (_gatlingFrameCnt >= 100 && _gatlingFrameCnt % GatlingFrame == 0 && _owner._attack == false) {
      // ���������������̐ݒ�
      _owner._rotateDir = _owner.GetObjServer().GetVecData("PlayerPos") - _owner._position;
      // �U�����Ă���Ɛݒ�
      _owner._attack = true;
      // ��]���Ă���Ɛݒ�
      _owner._rotating = true;
      // ��]���Ă��Ȃ����̃t���[���J�E���g�̍X�V
      ++_gatlingFrameCnt;
   }
   // �U�����Ă��邩�m�F
   if (_owner._attack) {
      // �U�����Ă���ꍇ
      // �U������������։�]������
      _owner.AreaRotate(_owner._rotating);
      // ��]���I�����Ă��邩�m�F
      if (!_owner._rotating) {
         // �I�����Ă�����K�g�����O�𐶐�
         _owner.CreateGatling();
         // �K�g�����O�̒e��ł񐔂̍X�V
         --_owner._gatlingCnt;
         // �炷�T�E���h�̐ݒ�
         _owner.GetSoundComponent().Play("BossGatling", _owner._position);
         // �U�����Ă��Ȃ��Ɛݒ�
         _owner._attack = false;
      }
   }
   // �U�����Ă��Ȃ��ꍇ��]���Ă��Ȃ����̃t���[���J�E���g�̍X�V������
   else {
      ++_gatlingFrameCnt;
   }
   // �K�g�����O��ł񐔂�0�ȉ��ɂȂ�����ҋ@��Ԃ�
   if (_owner._gatlingCnt <= 0) {
      _owner._stateServer->GoToState("Idle");
   }
   // �����蔻�菈�����s���N���X�Ńv���C���[�����[�W�G�l�~�[�Ɠ������Ă��邩�m�F
   _owner._collisionComponent->LargeEnemyFromPlayer();
   // �����I�u�W�F�N�g�Ɠ����������m�F
   _owner.HitCheckFromFallObject();
   // ���u��U���̒e�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromBullet();
   // �X�^���l�̍X�V�Ɗm�F
   _owner.StunCheck();
}

void LargeEnemy::StateDie::Enter() {
   // ���f���̃A�j���[�V�����̐ݒ�
   _owner._modelAnimeComponent->ChangeAnime("dead", false, 0.5);
   auto efcCrash = std::make_unique<Effect::EffectBossCrash>(_owner._gameMain, "BossCrash");
   _owner.GetEfcServer().Add(std::move(efcCrash));
}

void LargeEnemy::StateDie::Update() {
   if (_owner._modelAnimeComponent->repeatedCount() > 0) {
      _owner._gameMain.modeServer().PushBack("MissionCompleted");
   }
}

void LargeEnemy::StateMove::Enter() {
   // ���̏�ԂɂȂ������̃Q�[���̃t���[���J�E���g�̕ۑ�
   _stateCnt = _owner.gameMain().modeServer().frameCount();
   // �ړ���̃v���C���[�ւ̌����̃x�N�g�����擾����Ɛݒ�
   _endGetplyPos = true;
   // �ړ����ŏ��Ɉړ������ɉ�]����Ɛݒ�
   _owner._firstRotating = true;
   // �ړ����Ō�Ƀv���C���[�̕����ɉ�]����Ɛݒ�
   _owner._endRotating = true;
   // ���f���̃A�j���[�V�����̐ݒ�
   _owner._modelAnimeComponent->ChangeAnime("walk_0", true);
   // 0��1�������_���Ɏ擾
   auto result = /*AppFrame::Math::Utility::GetRandom(0, 1)*/true;
   // 1�������ꍇ
   if (result) {
      // �v���C���[�ւ̌����̃x�N�g�����擾
      _owner._moved = _owner.GetObjServer().GetVecData("PlayerPos") - _owner._position;
      // �P�ʉ�����
      _owner._moved.Normalized();
      // ��]�̊p���x�̐ݒ�
      _owner.SetAddRotate();
      // �ړ��ʂ̃x�N�g����ݒ�
      _owner._moved = _owner._moved * 30.0;
   }
   // 0�������ꍇ
   else {
      // �����_���Ɋp�x���擾
      auto degree = AppFrame::Math::Utility::GetRandom(0.0, 360.0);
      // �擾�����p�x��]������}�g���N�X�̍쐬
      Matrix44 rotate = Matrix44();
      rotate.RotateY(degree, true);
      // Z���̃v���X�����̃x�N�g�����擾�����}�g���N�X����]�������x�N�g�����ړ������̃x�N�g���ɐݒ�
      _owner._moved = Vector4(0.0, 0.0, 1.0) * rotate;
      // �P�ʉ�����
      _owner._moved.Normalized();
      // ��]�̊p���x�̐ݒ�
      _owner.SetAddRotate();
      // �ړ��ʂ̃x�N�g����ݒ�
      _owner._moved = _owner._moved * 30.0;
   }
   _footCnt = 0;
}

void LargeEnemy::StateMove::Update() {
   // �Q�[���̃t���[���J�E���g�̎擾
   auto gameCount = _owner.gameMain().modeServer().frameCount();
   // ���̏�Ԃɓ����Ă���̌o�߃t���[�����̎擾
   auto count = gameCount - _stateCnt;
   // �����̏���
   FootStepSound();
   // �ړ����ŏ��Ɉړ������ɉ�]����ꍇ�ړ������։�]������
   if (_owner._firstRotating) {
      _owner.AugularRotate(_owner._firstRotating);
   }
   // �ړ������ւ̉�]���I�������ꍇ
   else {
      // ����̃t���[�����o�߂�����
      if (count >= 60 * 3) {
         // �ړ���Ƀv���C���[�ւ̌������擾����ꍇ
         if (_endGetplyPos) {
            // �v���C���[�ւ̌����̃x�N�g�����擾
            _owner._moved = _owner.GetObjServer().GetVecData("PlayerPos") - _owner._position;
            // �P�ʉ�����
            _owner._moved.Normalized();
            // ��]�̊e���x�̐ݒ�
            _owner.SetAddRotate();
            // �ړ���̃v���C���[�ւ̌������擾���Ȃ��Ɛݒ�
            _endGetplyPos = false;
         }
         // �ړ����Ō�Ƀv���C���[�̕����ɉ�]����ꍇ�v���C���[�̕����։�]������
         if (_owner._endRotating) {
            _owner.AugularRotate(_owner._endRotating);
         }
         // ��]���I�������ꍇ�ҋ@��Ԃ�
         else {
            _owner._stateServer->GoToState("Idle");
         }
      }
      // ����̃t���[�����o�߂�����
      else if (count >= 60 * 1) {
         // �ړ�����
         _owner.Move(_owner._moved);
      }
   }
   // �����蔻�菈�����s���N���X�Ńv���C���[�����[�W�G�l�~�[�Ɠ������Ă��邩�m�F
   _owner._collisionComponent->LargeEnemyFromPlayer();
   // �����I�u�W�F�N�g�Ɠ����������m�F
   _owner.HitCheckFromFallObject();
   // ���u��U���̒e�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromBullet();
   // �X�^���l�̍X�V�Ɗm�F
   _owner.StunCheck();
}

void LargeEnemy::StateLaser::Enter() {
   // ���̏�ԂɂȂ������̃Q�[���̃t���[���J�E���g�̕ۑ�
   _stateCnt = _owner.gameMain().modeServer().frameCount();
   // ���[�U�[�𐶐�����ʒu��ݒ�
   _owner.SetLaserPosition();
   // �U�����������ݒ�
   _owner._rotateDir = _owner._gameMain.objServer().GetVecData("LaserDirectionPos") - _owner._position;
   // ���������������̃x�N�g����傫������l��ݒ�
   _owner._rotateEnlarge = 10.0;
   // ��]����������Ɛݒ�
   _owner._rotating = true;
   // �U�������Ă��Ȃ��Ɛݒ�
   _owner._attack = false;
   // ���[�U�[�U���t���O��ݒ�
   _owner._isLaser = true;
   // ���[�U�[�𐶐����Ȃ��Ɛݒ�
   _createLaser = false;
   // ���f���̃A�j���[�V�����̐ݒ�
   _owner._modelAnimeComponent->ChangeAnime("beem", false);
   // �炷�T�E���h�̐ݒ�
   _owner.GetSoundComponent().Play("BossCharge",_owner._position);
   // ���[�U�[�����ʒu���擾
   auto laserPos = GetLaserPos();
   // ���[�U�[�����ʒu���I�u�W�F�N�g�T�[�o�[�ɓo�^
   _owner.GetObjServer().RegistVector("LaserPos", laserPos);
   // �`���[�W�G�t�F�N�g�̃C���X�^���X�𐶐�
   auto efcCharge = std::make_unique<Effect::EffectBossCharge>(_owner._gameMain, "BossCharge");
   // �G�t�F�N�g�̈ʒu�ݒ�
   efcCharge->position(laserPos);
   // �G�t�F�N�g�T�[�o�[�ɓo�^
   _owner.GetEfcServer().Add(std::move(efcCharge));

}

void LargeEnemy::StateLaser::Update() {
   // �Q�[���̃t���[���J�E���g�̎擾
   auto gameCount = _owner.gameMain().modeServer().frameCount();
   // ���̏�Ԃɓ����Ă���̌o�߃t���[�����̎擾
   auto count = gameCount - _stateCnt;
   // �A�j���[�V�����Đ����I��������ҋ@��Ԃ�
   auto repeated = _owner._modelAnimeComponent->repeatedCount();
   auto playTime = _owner._modelAnimeComponent->playTime();
   // �U����������։�]������
   _owner.AreaRotate(_owner._rotating);
   if (repeated >= 1) {
      _owner._stateServer->GoToState("Idle");
   }
   // ����̃t���[�����o�߂��Ă��ă��[�U�[�𐶐����Ȃ��ƂȂ��Ă�����
   else if (playTime > LaserIrradiationTime && !_createLaser) {
      // �U�����Ă���Ɛݒ�
      _owner._attack = true;
      // ���[�U�[�𐶐�����Ɛݒ�
      _createLaser = true;
   }
   // �U�����Ă��邩�m�F
   if (_owner._attack) {
      // �U�����Ă����ꍇ
      // ��]���I��������
      if (!_owner._rotating) {
         // ���[�U�[�𐶐�����
         _owner.CreateLaser();
         // �炷�T�E���h�̐ݒ�
         _owner.GetSoundComponent().Play("BossBeam", _owner._position);
         // �U�����Ă��Ȃ��Ɛݒ�
         _owner._attack = false;
      }
   }
   // �����蔻�菈�����s���N���X�Ńv���C���[�����[�W�G�l�~�[�Ɠ������Ă��邩�m�F
   _owner._collisionComponent->LargeEnemyFromPlayer();
   // �����I�u�W�F�N�g�Ɠ����������m�F
   _owner.HitCheckFromFallObject();
   // ���u��U���̒e�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromBullet();
   // �X�^���l�̍X�V�Ɗm�F
   _owner.StunCheck();
   // ���[�U�[�����ʒu���I�u�W�F�N�g�T�[�o�[�ɓo�^
   _owner.GetObjServer().RegistVector("LaserPos", GetLaserPos());
}

void LargeEnemy::StateLaser::Exit() {
   // �r�[���֘A�G�t�F�N�g���Đ����Ă����Ȃ��~����
   for (auto& effect : _owner.GetEfcServer().runEffects()) {
      // �񋓌^��using�錾
      using enum Effect::EffectBase::EffectType;
      switch (effect->GetEfcType()) {
      // �r�[���֘A�G�t�F�N�g
      case BossCharge:
      case BossBeam:
         effect->StopEffect();   // �G�t�F�N�g�̒�~
         break;
      default:
         break;
      }
   }
   // ���[�U�[�U���t���O��ݒ�
   _owner._isLaser = false;
}

void LargeEnemy::StateFanGatling::Enter() {
   // ���K�g�����O�U�������邽�߂̉�]�����Ă��Ȃ����Ɍo�߂�����t���[���J�E���g�̐ݒ�
   _fanGatlingFrameCnt = 0;
   // �K�g�����O�̒e��ł񐔂�ݒ�
   _owner._gatlingCnt = 10;
   // �ŏ��ɐ��ɃK�g�����O�U��������Ƃ��̌����̃x�N�g�����擾����Ƃ��x�N�g������]������p�x��ݒ�
   _owner._fanAngle = -45.0;
   // �v���C���[�ւ̌������擾
   _owner._rotateDir = _owner.GetObjServer().GetVecData("PlayerPos") - _owner._position;
   // ���������������̃x�N�g����傫������l��ݒ�
   _owner._rotateEnlarge = 50.0;
   // ���f���̃A�j���[�V�����̐ݒ�
   _owner._modelAnimeComponent->ChangeAnime("gatoring", true);
}

void LargeEnemy::StateFanGatling::Update() {
   // ��]���Ă��Ȃ����̃t���[���J�E���g������̃t���[�����o�߂��U�����Ă��炸����̃t���[���������傫��������
   if (_fanGatlingFrameCnt % GatlingFrame == 0 && _owner._attack == false && _fanGatlingFrameCnt >= 100) {
      // �v���C���[�ւ̌����̃x�N�g������]������}�g���N�X���쐬
      Matrix44 dirRotate;
      dirRotate.RotateY(_owner._fanAngle, true);
      // �K�g�����O�U����ł����̐ݒ�
      _owner._rotateDir = _owner._rotateDir * dirRotate;
      // �U�����Ă���Ɛݒ�
      _owner._attack = true;
      // ��]���Ă���Ɛݒ�
      _owner._rotating = true;
      // ��]���Ă��Ȃ��Ƃ��̃t���[���J�E���g�̍X�V
      ++_fanGatlingFrameCnt;
   }
   // �U�����Ă�����
   if (_owner._attack) {
      // �U������������։�]������
      _owner.AreaRotate(_owner._rotating);
      // ��]���I��������
      if (!_owner._rotating) {
         // �K�g�����O�𐶐�
         _owner.CreateGatling();
         // �K�g�����O�̒e��ł񐔂̍X�V
         --_owner._gatlingCnt;
         // ���ɃK�g�����O�U��������Ƃ��̌����̃x�N�g�����擾����Ƃ��x�N�g������]������p�x��ݒ�
         _owner._fanAngle = 10.0;
         // �炷�T�E���h�̐ݒ� 
         _owner.GetSoundComponent().Play("BossGatling",_owner._position);
         // �U�����Ă��Ȃ��Ɛݒ�
         _owner._attack = false;
      }
   }
   // �U�����Ă��Ȃ��ꍇ
   else {
      // ��]���Ă��Ȃ��Ƃ��̃t���[���J�E���g�̍X�V
      ++_fanGatlingFrameCnt;
   }
   // �K�g�����O��ł񐔂�0�ȉ��ɂȂ�����ҋ@��Ԃ�
   if (_owner._gatlingCnt <= 0) {
      _owner._stateServer->GoToState("Idle");
   }
   // �����蔻�菈�����s���N���X�Ńv���C���[�����[�W�G�l�~�[�Ɠ������Ă��邩�m�F
   _owner._collisionComponent->LargeEnemyFromPlayer();
   // �����I�u�W�F�N�g�Ɠ����������m�F
   _owner.HitCheckFromFallObject();
   // ���u��U���̒e�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromBullet();
   // �X�^���l�̍X�V�Ɗm�F
   _owner.StunCheck();
}

void LargeEnemy::StateStun::Enter() {
   // ���f���̃A�j���[�V�����̐ݒ�
   _owner._modelAnimeComponent->ChangeAnime("stan", true, 0.5);
   auto efcStan = std::make_unique<Effect::EffectBossStan>(_owner._gameMain, "BossStan");
   efcStan->position(_owner._position);
   _owner.GetEfcServer().Add(std::move(efcStan));

}

void LargeEnemy::StateStun::Update() {
   // �A�j���[�V�����̌J��Ԃ����񐔂̎擾
   auto repeated = _owner._modelAnimeComponent->repeatedCount();
   // �A�j���[�V���������Đ�������ҋ@��Ԃ�
   if (repeated >= 1) {
      _owner._stateServer->GoToState("Idle");
   }
   // �����蔻�菈�����s���N���X�Ńv���C���[�����[�W�G�l�~�[�Ɠ������Ă��邩�m�F
   _owner._collisionComponent->LargeEnemyFromPlayer();
   // �����I�u�W�F�N�g�Ɠ����������m�F
   _owner.HitCheckFromFallObject();
   // ���u��U���̒e�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromBullet();
}

void LargeEnemy::StateStun::Exit() {
   // �X�^���l��������
   _owner._stunValue = 0.0;
}

void LargeEnemy::StateConsecutiveFallObject::Enter() {
   // ���f���̃A�j���[�V�����̐ݒ�
   _owner._modelAnimeComponent->ChangeAnime("object_attack", false, 1.0); 
   // �����I�u�W�F�N�g�𐶐����鐔������̒l�ɐݒ�
   _fallObjectNum = ConsecutiveNum;
   auto efcPreliminary = std::make_unique<Effect::EffectPreliminaryLight>(_owner._gameMain, "PreliminaryLight");
   // �{�X�̌����̎擾(�{�X��Z���W���t�ׁ̈A���]�����Ă���)
   auto bossDir = _owner.GetForward() * -1.0;
   // �G�t�F�N�g�����ʒu�ɋ߂��t���[���̎擾
   auto rootFramePos = _owner.modelAnimeComponent().GetFrameChildPosion("root", "root1");
   // �w����W�����炷
   auto efcPos = rootFramePos + bossDir * 300.0;
   efcPreliminary->position(efcPos);
   _owner.GetEfcServer().Add(std::move(efcPreliminary));
   _cntInit = true;
}

void LargeEnemy::StateConsecutiveFallObject::Update() {
   // ���̏�Ԃɓ����Ă���̌o�߃t���[�����̎擾
   if (_owner._modelAnimeComponent->repeatedCount() > 0&& _cntInit) {
      _stateCnt = _owner.gameMain().modeServer().frameCount();
      _cntInit = false;
   }
   auto count = _owner.gameMain().modeServer().frameCount() - _stateCnt;
   if (!_cntInit && count % ConsecutiveFallObjectFrame == 0) {
      // �����I�u�W�F�N�g�̐���
      CreateFallObject();
      // �J������U�������邽�߂ɃJ�����̐U���Ɏg��Y�̈ʒu��0.0�ɐݒ�
      _owner._cameraComponent->SetVibValue(0.0);
      --_fallObjectNum;
   }
   if (_fallObjectNum <= 0) {
      _owner._stateServer->GoToState("Idle");
   }
   // �����蔻�菈�����s���N���X�Ńv���C���[�����[�W�G�l�~�[�Ɠ������Ă��邩�m�F
   _owner._collisionComponent->LargeEnemyFromPlayer();
   // �����I�u�W�F�N�g�Ɠ����������m�F
   _owner.HitCheckFromFallObject();
   // ���u��U���̒e�Ɠ������Ă��邩�m�F
   _owner.HitCheckFromBullet();
   // �X�^���l�̍X�V�Ɗm�F
   _owner.StunCheck();
}

void LargeEnemy::StateFallObject::SetPoorSpawn() {
   auto&& runObjects = _owner.GetObjServer().runObjects();
   auto activePoorEnemyCount = std::count_if(runObjects.begin(), runObjects.end(),
      [](std::unique_ptr<Object::ObjectBase>& obj) {
         // ������Ԃ̎G���G�͉��̂��邩
         return (obj->GetObjType() == Object::ObjectBase::ObjectType::PoorEnemy) && obj->IsActive(); });
   // ������Ԃ̎G���G����薢���Ȃ�
   if (activePoorEnemyCount <= MaxPoorEnemy) {
      // �����_���ȃX�|�[���e�[�u�������蓖�Ă�
      auto random = AppFrame::Math::Utility::GetRandom(MinWave, MaxWave);
      _owner._gameMain.objFactory().SetSpawnTable("bosswave" + std::to_string(random));
   }
}

void LargeEnemy::StateMove::FootStepSound() {
   // �t���[���J�E���g�̎擾
   auto count = _owner.gameMain().modeServer().frameCount();
   // �{�X�̗��O���̐ڒn�����̃t���[�����擾
   auto rightFootFramePos = _owner._modelAnimeComponent->GetFrameChildPosion("root", "front_right_hand");
   auto leftFootFramePos = _owner._modelAnimeComponent->GetFrameChildPosion("root", "front_left_hand");
   // �{�X�̗��O���̐ڒn�����̃t���[���̍������擾
   auto rightFootY = rightFootFramePos.GetY();
   auto leftFootY = leftFootFramePos.GetY();
   // �E�O���̐ڒn�����t���[���͈��ȏ㍂����
   if (rightFootY >= FootStepHeight) {
      _footRightStep = true;    // ��������t���O��true
   }
   else {
      // �E��������t���O��true��
      if (_footRightStep) {
         _owner.GetSoundComponent().Play("BossFootStep", _owner._position);   // �����̍Đ�
         _footRightStep = false;                                              // ��������t���O��false
      }
   }
   // ���O���̐ڒn�����t���[���͈��ȏ㍂����
   if (leftFootY >= FootStepHeight) {
      _footLeftStep = true;    // ��������t���O��true
   }
   else {
      // ����������t���O��true��
      if (_footLeftStep) {
         _owner.GetSoundComponent().Play("BossFootStep", _owner._position);   // �����̍Đ�
         _footLeftStep = false;                                               // ��������t���O��false
      }
   }
}

AppFrame::Math::Vector4 LargeEnemy::StateLaser::GetLaserPos() {
   // �{�X�̌����̎擾(�{�X��Z���W���t�ׁ̈A���]�����Ă���)
   auto bossDir = _owner.GetForward() * -1.0;
   // ���[�U�[�����ʒu�ɋ߂��t���[���̎擾
   auto laserFramePos = _owner.modelAnimeComponent().GetFramePosion("pasted__laser_collision");
   // �w����W�����炷
   auto laserPos = laserFramePos + bossDir * LaserDiffPos;
   return laserPos;
}

void LargeEnemy::StateConsecutiveFallObject::CreateFallObject() {
   // �v���C���[�̈ʒu�̎擾
   auto plyPos = _owner.GetObjServer().GetVecData("PlayerPos");
   // �v���C���[�̈ʒu�Ɉ��̍����𑫂�
   plyPos.Add(0.0, 500.0, 0.0);
   // �����I�u�W�F�N�g�̐���
   auto fallObjectBase = _owner.gameMain().objFactory().Create("FallObject");
   // �����I�u�W�F�N�g�̎Q�ƌ^�ɃL���X�g
   auto& fallObject = dynamic_cast<Enemy::FallObject&>(*fallObjectBase);
   // �v���C���[�̈ʒu�Ɉ��̍����𑫂����l���ʒu�ɐݒ�
   fallObject.position(plyPos);
   // �c�����Ȃ��Ɛݒ�
   fallObject.residual(false);
   // �I�u�W�F�N�g�ꊇ�Ǘ��N���X�̓��I�z��ɒǉ�
   _owner.GetObjServer().Add(std::move(fallObjectBase));
}
