
/*****************************************************************//**
 * \file   CollisionServer.cpp
 * \brief  �����蔻�菈�����s���N���X
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "CollisionComponent.h"
#include "ModelAnimeComponent.h"
#include "FallObject.h"
#include "Gatling.h"
#include "Game.h"
#include "Player.h"
#include "Stage.h"
#include "StageModelComponent.h"
#include "LoadStageFromJson.h"
#include "LargeEnemy.h"
#include "Laser.h"
#include "ModeBoss.h"
#include "ModePoor.h"
#include "ObjectBase.h"
#include "ObjectServer.h"
#include "Player.h"
#ifdef _DEBUG
#include <stdexcept>
#include <windows.h>
#endif

using namespace FragmentValkyria::Collision;

CollisionComponent::CollisionComponent(Object::ObjectBase& owner) : _owner{ owner } {
   _param = std::make_unique<Param::ParamCollision>("collision");
   //�����蔻��̌��ʂ��Ǘ�����N���X�̃|�C���^�쐬
   _report = std::make_unique<Report>();
}

void CollisionComponent::PlayerFromObjectRange() {
   // �I�u�W�F�N�g�T�[�o�[�̊e�I�u�W�F�N�g���擾
   auto& runObjects = Game::Game::GetObjServer().runObjects();
   for (auto&& object : runObjects) {
      // �v���C���[����Ȃ���Ή������Ȃ�
      if (object->GetObjType() != Object::ObjectBase::ObjectType::Player) {
         continue;
      }
      // ���̃I�u�W�F�N�g�ɓ������Ă����ꍇ�͏I������
      if (object->collisionComponent().report().id() == ReportId::HitFromIdleFallObject) {
         break;
      }
      if (object->collisionComponent().report().id() == ReportId::HitFromFallObject) {
         break;
      }
      if (object->collisionComponent().report().id() == ReportId::HitFromGatling) {
         break;
      }
      if (object->collisionComponent().report().id() == ReportId::HitFromLargeEnemy) {
         break;
      }
      if (object->collisionComponent().report().id() == ReportId::HitFromPoorEnemy) {
         break;
      }
      // �v���C���[�̎Q�ƌ^�ɃL���X�g����
      auto& player = dynamic_cast<Player::Player&>(*object);
      // ��������I�u�W�F�N�g�������グ����͈͂̋��̔��a
      const auto FallObjectRange = _param->GetDoubleParam("fallobject_range");
      // �I�u�W�F�N�g�T�[�o�[�̊e�I�u�W�F�N�g���擾
      auto& runObjects = Game::Game::GetObjServer().runObjects();
      for (auto&& object : runObjects) {
         // �����蔻��̌��ʂ��v���C���[�Ɠ������Ă��邩�m�F
         if (object->collisionComponent().report().id() == ReportId::HitFromPlayer) {
            // �v���C���[�Ɠ������Ă���I�u�W�F�N�g���������ꍇ
            // �����I�u�W�F�N�g�łȂ���Ώ������X�L�b�v����for������
            if (object->GetObjType() != Object::ObjectBase::ObjectType::FallObject) {
               continue;
            }
            // �v���C���[���I�u�W�F�N�g�������グ���Ȃ��ꍇ��������I�u�W�F�N�g�̓����蔻��̌��ʂɓ������Ă��Ȃ��Ɛݒ�
            if (!player.isLift()) {
               object->collisionComponent().report().id(ReportId::None);
               // �������X�L�b�v���Ė߂�
               return;
            }
            // ��������I�u�W�F�N�g�̈ʒu���擾
            auto objectPos = object->position();
            // ���O�̋����`
            AppFrame::Math::Sphere objectRange = std::make_tuple(objectPos, FallObjectRange);
            // �v���C���[�̈ʒu�擾
            auto plyPoint = player.position();
            // ���Ɠ_�̓����蔻����Ƃ�
            if (AppFrame::Math::Utility::CollisionSpherePoint(plyPoint, objectRange)) {
               // �������Ă�����v���C���[�̓����蔻��̌��ʂɃI�u�W�F�N�g�������グ����͈͂ɂ���Ɛݒ�
               player.collisionComponent().report().id(ReportId::HitFromObjectRange);
               // ��������I�u�W�F�N�g�̓����蔻��̌��ʂɃv���C���[�ɓ������Ă���Ɛݒ�
               object->collisionComponent().report().id(ReportId::HitFromPlayer);
               // �������X�L�b�v���Ė߂�
               return;
            }
            else {
               // �������Ă��Ȃ�������v���C���[�̓����蔻��̌��ʂɓ������Ă��Ȃ��Ɛݒ�
               player.collisionComponent().report().id(ReportId::None);
               // ��������I�u�W�F�N�g�̓����蔻��̌��ʂɓ������Ă��Ȃ��Ɛݒ�
               object->collisionComponent().report().id(ReportId::None);
            }
         }
      }
      // ��������I�u�W�F�N�g�̈ʒu���擾
      auto objectPos = _owner.position();
      // ���O�̋����`
      AppFrame::Math::Sphere objectRange = std::make_tuple(objectPos, FallObjectRange);
      // �v���C���[�̈ʒu�擾
      auto plyPoint = player.position();
      // �v���C���[���I�u�W�F�N�g�������グ���Ȃ��ꍇ��������I�u�W�F�N�g�̓����蔻��̌��ʂɓ������Ă��Ȃ��Ɛݒ�
      if (!player.isLift()) {
         _owner.collisionComponent().report().id(ReportId::None);
         // �������X�L�b�v���Ė߂�
         return;
      }
      // ���Ɠ_�̓����蔻����Ƃ�
      if (AppFrame::Math::Utility::CollisionSpherePoint(plyPoint, objectRange)) {
         // �������Ă�����v���C���[�̓����蔻��̌��ʂɃI�u�W�F�N�g�������グ����͈͂ɂ���Ɛݒ�
         player.collisionComponent().report().id(ReportId::HitFromObjectRange);
         // ��������I�u�W�F�N�g�̓����蔻��̌��ʂɃv���C���[�ɓ������Ă���Ɛݒ�
         _owner.collisionComponent().report().id(ReportId::HitFromPlayer);
      }
      else {
         // �������Ă��Ȃ�������v���C���[�̓����蔻��̌��ʂɓ������Ă��Ȃ��Ɛݒ�
         player.collisionComponent().report().id(ReportId::None);
         // ��������I�u�W�F�N�g�̓����蔻��̌��ʂɓ������Ă��Ȃ��Ɛݒ�
         _owner.collisionComponent().report().id(ReportId::None);
      }
   }
}

void CollisionComponent::PlayerFromFallObjectModel(bool fall) {
   /**
    * \brief double�^�̒l�𕶎���Ŏw�肵�A�l�Ǘ��N���X����擾����
    * \param paramName �l���w�肷�镶����
    * \return ������ɂ��w�肳�ꂽ�l
    */
   const auto _DoubleParam = [&](std::string paramName) {
      return _param->GetDoubleParam(paramName);
   };
   // �����I�u�W�F�N�g�̎Q�ƌ^�ɃL���X�g
   auto& fallObject = dynamic_cast<Enemy::FallObject&>(_owner);
   // ��������I�u�W�F�N�g�̃��f���̃n���h���擾
   auto objectModel = fallObject.modelAnimeComponent().modelHandle();
   // ���f���̃R���W�����̃t���[���ԍ��擾
   auto collision = MV1SearchFrame(objectModel, fallObject.collisionName().data());
   // �I�u�W�F�N�g�T�[�o�[�̊e�I�u�W�F�N�g���擾
   auto& runObjects = Game::Game::GetObjServer().runObjects();
   for (auto&& object : runObjects) {
      // �v���C���[����Ȃ������牽�����Ȃ�
      if (object->GetObjType() != Object::ObjectBase::ObjectType::Player) {
         continue;
      }
      // �����蔻��̌��ʂ��ҋ@��Ԃ̗�������I�u�W�F�N�g�Ɠ������Ă���ꍇ�������Ȃ�
      if (object->collisionComponent().report().id() == ReportId::HitFromIdleFallObject) {
         continue;
      }
      // �v���C���[�̎Q�ƌ^�̎擾
      auto& player = dynamic_cast<Player::Player&>(*object);
      // ���G���Ԃ̎擾
      auto invincibleCnt = player.invincibleCnt();
      // �v���C���[���̃J�v�Z����ݒ�
      auto plyPos = object->position();
      // �J�v�Z���̈�ڂ̈ʒu
      auto pos1 = plyPos + AppFrame::Math::Vector4(0.0, _DoubleParam("ply_capsule_pos1"), 0.0);
      // �J�v�Z���̓�ڂ̈ʒu
      plyPos.Add(0.0, _DoubleParam("ply_capsule_pos2"), 0.0);
      // �J�v�Z���̔��a
      auto radian = static_cast<float>(_DoubleParam("ply_radius"));
      // �J�v�Z���ƃ��f���̓����蔻����Ƃ�
      auto result = MV1CollCheck_Capsule(objectModel, collision, AppFrame::Math::ToDX(pos1), AppFrame::Math::ToDX(plyPos), radian);
      // �����蔻��̌��ʂ��������Ă��邩�m�F
      if (result.HitNum > 0) {
         // �������Ă����ꍇ�������m�F
         if (fall) {
            // �������������ꍇ���G���Ԃ�������I��
            if (invincibleCnt > 0) {
               return;
            }
            // ���G���Ԃ���Ȃ��ꍇ���������ʒu�ɗ�������I�u�W�F�N�g�̈ʒu��ݒ�
            object->collisionComponent().hitPos(_owner.position());
            // �v���C���[�̓����蔻��̌��ʂɗ�������I�u�W�F�N�g�̃��f���Ɠ��������Ɛݒ�
            object->collisionComponent().report().id(ReportId::HitFromFallObject);
            // �_���[�W��20.0�ɐݒ�
            object->collisionComponent().damage(_DoubleParam("player_object_damage"));
         }
         else {
            // ����������Ȃ��ꍇ���������ʒu�ɓ��������|���S���̖@����ݒ�
            object->collisionComponent().hitPos(_owner.position());
            // �v���C���[�̓����蔻��̌��ʂɑҋ@��Ԃ̗�������I�u�W�F�N�g�Ɠ��������Ɛݒ�
            object->collisionComponent().report().id(ReportId::HitFromIdleFallObject);
         }
      }
   }
}

void CollisionComponent::GatlingFromObjectModel() {
   // �����I�u�W�F�N�g�̎Q�ƌ^�ɃL���X�g
   auto& fallObject = dynamic_cast<Enemy::FallObject&>(_owner);
   // ��������I�u�W�F�N�g�̃��f���̃n���h���̎擾
   auto objectModel = fallObject.modelAnimeComponent().modelHandle();
   // ���f���̃R���W�����̃t���[���ԍ��擾
   auto collision = MV1SearchFrame(objectModel, fallObject.collisionName().data());
   // �I�u�W�F�N�g�T�[�o�[�̊e�I�u�W�F�N�g���擾
   auto& runObjects = Game::Game::GetObjServer().runObjects();
   for (auto&& object : runObjects) {
      // �K�g�����O����Ȃ������牽�����Ȃ�
      if (object->GetObjType() != Object::ObjectBase::ObjectType::Gatling) {
         continue;
      }
      // �K�g�����O���̋���ݒ�
      auto gatlingPos = object->position();
      auto radius = static_cast<float>(15.0/*GatlingRadius*/);
      auto gatlingSphere = std::make_tuple(gatlingPos, radius);
      // ���ƃ��f���̓����蔻��̌��ʂ��擾
      auto result = MV1CollCheck_Sphere(objectModel, collision, AppFrame::Math::ToDX(gatlingPos), radius);
      // �����蔻��̌��ʂ��������Ă��邩�m�F
      if (result.HitNum > 0) {
         // �K�g�����O�̓����蔻�茋�ʂɑҋ@��Ԃ̗�������I�u�W�F�N�g�Ɠ��������Ɛݒ�
         object->collisionComponent().report().id(ReportId::HitFromIdleFallObject);
      }
   }
}

void CollisionComponent::GatlingFromPlayer() {
   /**
    * \brief double�^�̒l�𕶎���Ŏw�肵�A�l�Ǘ��N���X����擾����
    * \param paramName �l���w�肷�镶����
    * \return ������ɂ��w�肳�ꂽ�l
    */
   const auto _DoubleParam = [&](std::string paramName) {
      return _param->GetDoubleParam(paramName);
   };
   // �v���C���[�̎Q�ƌ^�ɃL���X�g
   auto& player = dynamic_cast<Player::Player&>(_owner);
   // ���G���Ԃ̎擾
   auto invincibleCnt = player.invincibleCnt();
   // ���G���Ԓ��������牽�����Ȃ�
   if (invincibleCnt > 0) {
      return;
   }
   // �v���C���[���̃J�v�Z����ݒ�
   auto playerPos = _owner.position();
   // �J�v�Z���̈ʈ�ڂ̈ʒu
   auto capsulePos1 = playerPos + Vector4(0.0, 30.0, 0.0);
   // �J�v�Z���̓�ڂ̈ʒu
   auto capsulePos2 = playerPos + Vector4(0.0, 60.0, 0.0);
   // �J�v�Z���̔��a
   auto casuleRadius = _DoubleParam("ply_radius");
   // ���O�̃J�v�Z�����`
   AppFrame::Math::Capsule playerCapsule = std::make_tuple(capsulePos1, capsulePos2, casuleRadius);
   // �I�u�W�F�N�g�T�[�o�[�̊e�I�u�W�F�N�g���擾
   auto& runObjects = Game::Game::GetObjServer().runObjects();
   for (auto&& object : runObjects) {
      // �K�g�����O����Ȃ������牽�����Ȃ�
      if (object->GetObjType() != Object::ObjectBase::ObjectType::Gatling) {
         continue;
      }
      // �K�g�����O�̎Q�ƌ^�ɃL���X�g
      auto& gatling = dynamic_cast<Enemy::Gatling&>(*object);
      // �K�g�����O���ҋ@��Ԃ̗����I�u�W�F�N�g�Ɠ������Ă�����
      if (gatling.collisionComponent().report().id() == ReportId::HitFromIdleFallObject) {
         // �������X�L�b�v����for������
         continue;
      }
      // �K�g�����O�̈ʒu�̎擾
      auto gatlingPos = gatling.position();
      // �K�g�����O�̔��a�̐ݒ�
      auto gatlingRadius = _DoubleParam("gatling_radius");
      // ���O�̋����`
      AppFrame::Math::Sphere gatlingSphere = std::make_tuple(gatlingPos, gatlingRadius);
      // �J�v�Z���Ƌ��œ����蔻����Ƃ�
      if (AppFrame::Math::Utility::CollisionCapsuleSphere(playerCapsule, gatlingSphere)) {
         // �������Ă�����K�g�����O���̓����蔻��̌��ʂ��v���C���[�Ɠ��������Ɛݒ�
         object->collisionComponent().report().id(ReportId::HitFromPlayer);
         // �v���C���[���̓����蔻��̌��ʂ��K�g�����O�Ɠ��������Ɛݒ�
         _owner.collisionComponent().report().id(ReportId::HitFromGatling);
         // �_���[�W�̐ݒ�
         _owner.collisionComponent().damage(_DoubleParam("player_gatling_damage"));
         // �v���C���[�̓��������ʒu�ɃK�g�����O�̈ʒu��ݒ�
         _owner.collisionComponent().hitPos(object->position());
      }
   }
}

void CollisionComponent::LargeEnemyFromObjectModel() {
   /**
    * \brief double�^�̒l�𕶎���Ŏw�肵�A�l�Ǘ��N���X����擾����
    * \param paramName �l���w�肷�镶����
    * \return ������ɂ��w�肳�ꂽ�l
    */
   const auto _DoubleParam = [&](std::string paramName) {
      return _param->GetDoubleParam(paramName);
   };
   // �����I�u�W�F�N�g�̎Q�ƌ^�ɃL���X�g
   auto& fallObject = dynamic_cast<Enemy::FallObject&>(_owner);
   // ��������I�u�W�F�N�g�̃J�v�Z�����쐬
   auto fallObjectPos = fallObject.position();
   // �J�v�Z���̈�ڂ̈ʒu
   auto capsulePos1 = fallObject.capsulePos1();
   // �J�v�Z���̓�ڂ̈ʒu
   auto capsulePos2 = fallObject.capsulePos2();
   // �J�v�Z���̔��a
   auto capsuleRadian = static_cast<float>(_DoubleParam("fallobject_drum_radius"));
   // �I�u�W�F�N�g�T�[�o�[�̊e�I�u�W�F�N�g���擾
   auto& runObjects = Game::Game::GetObjServer().runObjects();
   for (auto&& object : runObjects) {
      // ���[�W�G�l�~�[����Ȃ��ꍇ�������Ȃ�
      if (object->GetObjType() != Object::ObjectBase::ObjectType::LargeEnemy) {
         continue;
      }
      // ���[�W�G�l�~�[�̃��f���̃n���h���̎擾
      auto largeEnemyModel = object->modelAnimeComponent().modelHandle();
      // ���[�W�G�l�~�[�̃��f���̎�_�̃R���W�����t���[���ԍ��̎擾
      auto collision = object->modelAnimeComponent().FindFrame("pasted__weak_collision");
      // ���f���̎�_�ƃJ�v�Z���̓����蔻������
      auto result = MV1CollCheck_Capsule(largeEnemyModel, collision, AppFrame::Math::ToDX(capsulePos1), AppFrame::Math::ToDX(capsulePos2), capsuleRadian);
      // �����蔻��̌��ʂ��������Ă��邩�m�F
      if (result.HitNum > 0) {
         // �������Ă����烉�[�W�G�l�~�[�̓����蔻�茋�ʂ𗎉�����I�u�W�F�N�g�Ɠ������Ă���Ɛݒ�
         object->collisionComponent().report().id(ReportId::HitFromFallObject);
         // �_���[�W�̐ݒ�
         object->collisionComponent().damage(_DoubleParam("weak_object_damage"));
         // ��������I�u�W�F�N�g�̓����蔻�茋�ʂ����[�W�G�l�~�[�Ɠ��������Ɛݒ�
         _owner.collisionComponent().report().id(ReportId::HitFromLargeEnemy);
         // for���𔲂���
         break;
      }
      // ���f���̎�_�Ɠ������Ă��Ȃ��ꍇ
      else {
         // ���[�W�G�l�~�[�̃��f���̊�̃R���W�����t���[���ԍ��̎擾
         collision = object->modelAnimeComponent().FindFrame("pasted__face_collision");
         // ���f���̊�ƃJ�v�Z���̓����蔻������
         result = MV1CollCheck_Capsule(largeEnemyModel, collision, AppFrame::Math::ToDX(capsulePos1), AppFrame::Math::ToDX(capsulePos2), capsuleRadian);
         // �����蔻��̌��ʂ��������Ă��邩�m�F
         if (result.HitNum > 0) {
            // �������Ă����烉�[�W�G�l�~�[�̓����蔻�茋�ʂ𗎉�����I�u�W�F�N�g�Ɠ������Ă���Ɛݒ�
            object->collisionComponent().report().id(ReportId::HitFromFallObject);
            // �_���[�W�̐ݒ�
            object->collisionComponent().damage(_DoubleParam("normal_object_damage"));
            // ��������I�u�W�F�N�g�̓����蔻�茋�ʂ����[�W�G�l�~�[�Ɠ��������Ɛݒ�
            _owner.collisionComponent().report().id(ReportId::HitFromLargeEnemy);
            // for���𔲂���
            break;
         }
         // ���f���̊�Ɠ������Ă��Ȃ��ꍇ
         else {
            // ���[�W�G�l�~�[�̃��f���̓��̂̃R���W�����t���[���ԍ��̎擾
            collision = object->modelAnimeComponent().FindFrame("pasted__body_collision");
            // ���f���̓��̂ƃJ�v�Z���̓����蔻������
            result = MV1CollCheck_Capsule(largeEnemyModel, collision, AppFrame::Math::ToDX(capsulePos1), AppFrame::Math::ToDX(capsulePos2), capsuleRadian);
            // �����蔻��̌��ʂ��������Ă��邩�m�F
            if (result.HitNum > 0) {
               // �������Ă����烉�[�W�G�l�~�[�̓����蔻�茋�ʂ𗎉�����I�u�W�F�N�g�Ɠ������Ă���Ɛݒ�
               object->collisionComponent().report().id(ReportId::HitFromFallObject);
               // �_���[�W�̐ݒ�
               object->collisionComponent().damage(0.0);
               // ��������I�u�W�F�N�g�̓����蔻�茋�ʂ����[�W�G�l�~�[�Ɠ��������Ɛݒ�
               _owner.collisionComponent().report().id(ReportId::HitFromLargeEnemy);
               // for���𔲂���
               break;
            }
         }
      }
    }
}

void CollisionComponent::LargeEnemyFromBullet() {
   /**
    * \brief double�^�̒l�𕶎���Ŏw�肵�A�l�Ǘ��N���X����擾����
    * \param paramName �l���w�肷�镶����
    * \return ������ɂ��w�肳�ꂽ�l
    */
   const auto _DoubleParam = [&](std::string paramName) {
      return _param->GetDoubleParam(paramName);
   };
   // ���u��U���̒e�̈ʒu���擾
   auto bulletPos = _owner.position();
   // ���u��U���̒e�̔��a��ݒ�
   auto bulletRadius = static_cast<float>(_DoubleParam("bullet_radius"));
   // �I�u�W�F�N�g�T�[�o�[�̊e�I�u�W�F�N�g���擾
   auto& runObjects = Game::Game::GetObjServer().runObjects();
   for (auto&& object : runObjects){
      // ���[�W�G�l�~�[����Ȃ������牽�����Ȃ�
      if (object->GetObjType() != Object::ObjectBase::ObjectType::LargeEnemy) {
         continue;
      }
      // ���[�W�G�l�~�[�̃��f���̃n���h�����擾
      auto largeEnemyModel = object->modelAnimeComponent().modelHandle();
      // ���[�W�G�l�~�[�̃��f���̎�_�̃R���W�����t���[���ԍ��̎擾
      auto collision = object->modelAnimeComponent().FindFrame("pasted__weak_collision");
      // ���f���̎�_�Ƌ��̓����蔻������
      auto result = MV1CollCheck_Sphere(largeEnemyModel, collision, AppFrame::Math::ToDX(bulletPos), bulletRadius);
      // �����蔻��̌��ʂ��瓖�����Ă��邩�m�F
      if (result.HitNum > 0) {
         // �������Ă����烉�[�W�G�l�~�[�̓����蔻�茋�ʂ����u��U���̒e�Ɠ��������Ɛݒ�
         object->collisionComponent().report().id(ReportId::HitFromBullet);
         // �_���[�W�ʂ̐ݒ�
         object->collisionComponent().damage(_DoubleParam("weak_bullet_damage"));
         // ���u��U���̒e�̓����蔻�茋�ʂ����[�W�G�l�~�[�Ɠ��������Ɛݒ�
         _owner.collisionComponent().report().id(ReportId::HitFromLargeEnemy);
         // for���𔲂���
         break;
      }
      // ���f���̎�_�Ɠ������Ă��Ȃ��ꍇ
      else {
         // ���[�W�G�l�~�[�̃��f���̊�̃R���W�����t���[���ԍ��̎擾
         collision = object->modelAnimeComponent().FindFrame("pasted__face_collision");
         // ���f���̊�Ƌ��̓����蔻������
         result = MV1CollCheck_Sphere(largeEnemyModel, collision, AppFrame::Math::ToDX(bulletPos), bulletRadius);
         // �����蔻��̌��ʂ��瓖�����Ă��邩�m�F
         if (result.HitNum > 0) {
            // �������Ă����烉�[�W�G�l�~�[�̓����蔻�茋�ʂ����u��U���̒e�Ɠ��������Ɛݒ�
            object->collisionComponent().report().id(ReportId::HitFromBullet);
            // �_���[�W�ʂ̐ݒ�
            object->collisionComponent().damage(_DoubleParam("normal_bullet_damage"));
            // ���u��U���̒e�̓����蔻�茋�ʂ����[�W�G�l�~�[�Ɠ��������Ɛݒ�
            _owner.collisionComponent().report().id(ReportId::HitFromLargeEnemy);
            // for���𔲂���
            break;
         }
         // ���f���̊�ɓ������Ă��Ȃ������ꍇ
         else {
            // ���[�W�G�l�~�[�̃��f���̓��̂̃R���W�����t���[���ԍ��̎擾
            collision = object->modelAnimeComponent().FindFrame("pasted__body_collision");
            // ���f���̓��̂Ƌ��̓����蔻������
            result = MV1CollCheck_Sphere(largeEnemyModel, collision, AppFrame::Math::ToDX(bulletPos), bulletRadius);
            // �����蔻��̌��ʂ��瓖�����Ă��邩�m�F
            if (result.HitNum > 0) {
               // �������Ă����烉�[�W�G�l�~�[�̓����蔻�茋�ʂ����u��U���̒e�Ɠ��������Ɛݒ�
               object->collisionComponent().report().id(ReportId::HitFromBullet);
               // �_���[�W�ʂ̐ݒ�
               object->collisionComponent().damage(0.0);
               // ���u��U���̒e�̓����蔻�茋�ʂ����[�W�G�l�~�[�Ɠ��������Ɛݒ�
               _owner.collisionComponent().report().id(ReportId::HitFromLargeEnemy);
               // for���𔲂���
               break;
            }
         }
      }
   }
}

void CollisionComponent::FallObjectFromLaser() {
   /**
    * \brief double�^�̒l�𕶎���Ŏw�肵�A�l�Ǘ��N���X����擾����
    * \param paramName �l���w�肷�镶����
    * \return ������ɂ��w�肳�ꂽ�l
    */
   const auto _DoubleParam = [&](std::string paramName) {
      return _param->GetDoubleParam(paramName);
   };
   // �����I�u�W�F�N�g�̎Q�ƌ^�ɃL���X�g
   auto& fallObject = dynamic_cast<Enemy::FallObject&>(_owner);
   // ��������I�u�W�F�N�g�̃J�v�Z���̍쐬
   auto fallObjectPos = fallObject.position();
   // �J�v�Z���̈�ڂ̈ʒu
   auto fallObjectCapsulePos1 = fallObject.capsulePos1();
   // �J�v�Z���̓�ڂ̈ʒu
   auto fallObjectCapsulePos2 = fallObject.capsulePos2();
   // �J�v�Z���̔��a
   auto fallObjectRadius = _DoubleParam("fallobject_drum_radius");
   // ���O�̃J�v�Z�����`
   auto fallObjectCapsule = std::make_tuple(fallObjectCapsulePos1, fallObjectCapsulePos2, fallObjectRadius);
   // �I�u�W�F�N�g�T�[�o�[�̊e�I�u�W�F�N�g���擾
   auto& runObjects = Game::Game::GetObjServer().runObjects();
   for (auto&& object : runObjects) {
      // ���[�U�[����Ȃ������牽�����Ȃ�
      if (object->GetObjType() != Object::ObjectBase::ObjectType::Laser) {
         continue;
      }
      // ���[�U�[�̎Q�ƌ^�ɃL���X�g
      auto& laser = dynamic_cast<Enemy::Laser&>(*object);
      // ���[�U�[�̃J�v�Z�����쐬
      // �J�v�Z���̈�ڂ̈ʒu
      auto laserCapsulePos1 = laser.position();
      // �J�v�Z���̓�ڂ̈ʒu
      auto laserCapsulePos2 = laser.end();
      // �J�v�Z���̔��a
      auto laserRadius = _DoubleParam("laser_radius");
      // ���O�̃J�v�Z�����`
      auto laserCapsule = std::make_tuple(laserCapsulePos1, laserCapsulePos2, laserRadius);
      // �J�v�Z���ƃJ�v�Z���̓����蔻������
      if (AppFrame::Math::Utility::CollisionCapsuleCapsule(fallObjectCapsule, laserCapsule)) {
         // �������Ă����痎������I�u�W�F�N�g�̓����蔻�茋�ʂ����[�U�[�Ɠ��������Ɛݒ�
         _owner.collisionComponent().report().id(ReportId::HitFromLaser);
      }
      break;
   }
}

void CollisionComponent::PlayerFromLaser() {
   /**
    * \brief double�^�̒l�𕶎���Ŏw�肵�A�l�Ǘ��N���X����擾����
    * \param paramName �l���w�肷�镶����
    * \return ������ɂ��w�肳�ꂽ�l
    */
   const auto _DoubleParam = [&](std::string paramName) {
      return _param->GetDoubleParam(paramName);
   };
   // �v���C���[�̎Q�ƌ^�̎擾
   auto& player = dynamic_cast<Player::Player&>(_owner);
   // ���G���Ԃ̎擾
   auto invincibleCnt = player.invincibleCnt();
   // ���G���Ԓ��������ꍇ�������Ȃ�
   if (invincibleCnt > 0) {
      return;
   }
   // �v���C���[�̃J�v�Z���̍쐬
   auto playerPos = _owner.position();
   // �J�v�Z���̈�ڂ̈ʒu
   auto plyCapsulePos1 = Vector4(0.0, _DoubleParam("ply_capsule_pos1"), 0.0) + playerPos;
   // �J�v�Z���̓�ڂ̈ʒu
   auto plyCapsulePos2 = Vector4(0.0, _DoubleParam("ply_capsule_pos2"), 0.0) + playerPos;
   // �J�v�Z���̔��a
   auto playerCapsuleRadius = _DoubleParam("ply_radius");
   // ���O�̃J�v�Z�����`
   AppFrame::Math::Capsule playerCapsule = std::make_tuple(plyCapsulePos1, plyCapsulePos2, playerCapsuleRadius);
   // �I�u�W�F�N�g�T�[�o�[�̊e�I�u�W�F�N�g���擾
   auto& runObjects = Game::Game::GetObjServer().runObjects();
   for (auto&& object : runObjects) {
      // ���[�U�[����Ȃ������牽�����Ȃ�
      if (object->GetObjType() != Object::ObjectBase::ObjectType::Laser) {
         continue;
      }
      // ���[�U�[�̎Q�ƌ^�ɃL���X�g
      auto& laser = dynamic_cast<Enemy::Laser&>(*object);
      // ���[�U�[�̃J�v�Z�����쐬
      // �J�v�Z���̈�ڂ̈ʒu
      auto laserCapsulePos1 = laser.position();
      // �J�v�Z���̓�ڂ̈ʒu
      auto laserCapsulePos2 = laser.end();
      // �J�v�Z���̔��a
      auto laserRadius = _DoubleParam("laser_radius");
      // ���O�̃J�v�Z�����`
      auto laserCapsule = std::make_tuple(laserCapsulePos1, laserCapsulePos2, laserRadius);
      // �J�v�Z���ƃJ�v�Z���̓����蔻������
      if (AppFrame::Math::Utility::CollisionCapsuleCapsule(playerCapsule, laserCapsule)) {
         // �������Ă����ꍇ�v���C���[�̓����蔻��̌��ʂ����[�U�[�Ɠ��������Ɛݒ�
         _owner.collisionComponent().report().id(ReportId::HitFromLaser);
         // �_���[�W�̐ݒ�
         _owner.collisionComponent().damage(_DoubleParam("player_laser_damage"));
         // ���������ʒu�Ƀ��[�U�[�̈ʒu��ݒ�
         _owner.collisionComponent().hitPos(laser.position());
      }
      break;
   }
}

void CollisionComponent::LargeEnemyFromPlayer() {
   /**
    * \brief double�^�̒l�𕶎���Ŏw�肵�A�l�Ǘ��N���X����擾����
    * \param paramName �l���w�肷�镶����
    * \return ������ɂ��w�肳�ꂽ�l
    */
   const auto _DoubleParam = [&](std::string paramName) {
      return _param->GetDoubleParam(paramName);
   };
   //���[�W�G�l�~�[�̃��f���̃n���h���̎擾
   auto largeEnemyModel = _owner.modelAnimeComponent().modelHandle();
   //���[�W�G�l�~�[�̃��f���̃R���W�����t���[���ԍ��̎擾
   auto collision = _owner.modelAnimeComponent().FindFrame("S301_typeCO");
   //�I�u�W�F�N�g�T�[�o�[�̊e�I�u�W�F�N�g���擾
   auto& runObjects = Game::Game::GetObjServer().runObjects();
   for (auto&& object : runObjects) {
      //�v���C���[����Ȃ������牽�����Ȃ�
      if (object->GetObjType() != Object::ObjectBase::ObjectType::Player) {
         continue;
      }
      //�v���C���[�^�̎Q�Ƃ̎擾
      auto& player = dynamic_cast<Player::Player&>(*object);
      //���G���Ԃ̎擾
      auto invincibleCnt = player.invincibleCnt();
      //���G���Ԓ��������牽�����Ȃ�
      if (invincibleCnt > 0) {
         return;
      }
      //�v���C���[�̃J�v�Z���̍쐬
      auto playerPos = player.position();
      //�J�v�Z���̈�ڂ̈ʒu
      auto plyCapsulePos1 = Vector4(0.0, _DoubleParam("ply_capsule_pos1"), 0.0) + playerPos;
      //�J�v�Z���̓�ڂ̈ʒu
      auto plyCapsulePos2 = Vector4(0.0, _DoubleParam("ply_capsule_pos2"), 0.0) + playerPos;
      //�J�v�Z���̔��a
      auto playerCapsuleRadius = static_cast<float>(_DoubleParam("ply_radius"));
      //���f���ƃJ�v�Z���̓����蔻������
      auto result = MV1CollCheck_Capsule(largeEnemyModel, collision, AppFrame::Math::ToDX(plyCapsulePos1), AppFrame::Math::ToDX(plyCapsulePos2), playerCapsuleRadius);
      //�����蔻��̌��ʂ��m�F
      if (result.HitNum > 0) {
         //�������Ă�����v���C���[�̓����蔻�茋�ʂ��{�X�Ɠ��������Ɛݒ�
         player.collisionComponent().report().id(ReportId::HitFromLargeEnemy);
         //���������ʒu�Ƀ��[�W�G�l�~�[�̈ʒu��ݒ�
         player.collisionComponent().hitPos(_owner.position());
         //�_���[�W�̐ݒ�
         player.collisionComponent().damage(_DoubleParam("player_largeenemy_damage"));
      }
   }
}

void CollisionComponent::PoorEnemyFromPlayer() {
   /**
    * \brief double�^�̒l�𕶎���Ŏw�肵�A�l�Ǘ��N���X����擾����
    * \param paramName �l���w�肷�镶����
    * \return ������ɂ��w�肳�ꂽ�l
    */
   const auto _DoubleParam = [&](std::string paramName) {
      return _param->GetDoubleParam(paramName);
   };
   //���[�W�G�l�~�[�̃��f���̃n���h���̎擾
   auto poorEnemyModel = _owner.modelAnimeComponent().modelHandle();
   //���[�W�G�l�~�[�̃��f���̃R���W�����t���[���ԍ��̎擾
   auto collision = _owner.modelAnimeComponent().FindFrame("mob");
   //�I�u�W�F�N�g�T�[�o�[�̊e�I�u�W�F�N�g���擾
   auto& runObjects = Game::Game::GetObjServer().runObjects();
   for (auto&& object : runObjects) {
      //�v���C���[����Ȃ������牽�����Ȃ�
      if (object->GetObjType() != Object::ObjectBase::ObjectType::Player) {
         continue;
      }
      //�v���C���[�^�̎Q�Ƃ̎擾
      auto& player = dynamic_cast<Player::Player&>(*object);
      //���G���Ԃ̎擾
      auto invincibleCnt = player.invincibleCnt();
      //���G���Ԓ��������牽�����Ȃ�
      if (invincibleCnt > 0) {
         return;
      }
      //�v���C���[�̃J�v�Z���̍쐬
      auto playerPos = player.position();
      //�J�v�Z���̈�ڂ̈ʒu
      auto plyCapsulePos1 = Vector4(0.0, _DoubleParam("ply_capsule_pos1"), 0.0) + playerPos;
      //�J�v�Z���̓�ڂ̈ʒu
      auto plyCapsulePos2 = Vector4(0.0, _DoubleParam("ply_capsule_pos2"), 0.0) + playerPos;
      //�J�v�Z���̔��a
      auto playerCapsuleRadius = static_cast<float>(_DoubleParam("ply_radius"));
      //���f���ƃJ�v�Z���̓����蔻������
      auto result = MV1CollCheck_Capsule(poorEnemyModel, collision, AppFrame::Math::ToDX(plyCapsulePos1), AppFrame::Math::ToDX(plyCapsulePos2), playerCapsuleRadius);
      //�����蔻��̌��ʂ��m�F
      if (result.HitNum > 0) {
         //�������Ă�����v���C���[�̓����蔻�茋�ʂ��G���G�Ɠ��������Ɛݒ�
         player.collisionComponent().report().id(ReportId::HitFromPoorEnemy);
         //���������ʒu�Ƀ��[�W�G�l�~�[�̈ʒu��ݒ�
         player.collisionComponent().hitPos(_owner.position());
         //�_���[�W�̐ݒ�
         player.collisionComponent().damage(_DoubleParam("player_poorenemy_damage"));
      }
   }
}

void CollisionComponent::BulletFromPoorEnemy() {
   /**
    * \brief double�^�̒l�𕶎���Ŏw�肵�A�l�Ǘ��N���X����擾����
    * \param paramName �l���w�肷�镶����
    * \return ������ɂ��w�肳�ꂽ�l
    */
   const auto _DoubleParam = [&](std::string paramName) {
      return _param->GetDoubleParam(paramName);
   };
   //�K�g�����O�U�������Ă���G���G�̃��f���n���h���̎擾
   auto poorEnemyGatlingModel = _owner.modelAnimeComponent().modelHandle();
   //���f���̃R���W�����t���[���̎擾
   auto collision = _owner.modelAnimeComponent().FindFrame("mob");
   //�I�u�W�F�N�g�T�[�o�[�̊e�I�u�W�F�N�g���擾
   auto& runObjects = Game::Game::GetObjServer().runObjects();
   for (auto&& object : runObjects) {
      //���u��U���̒e����Ȃ�������Ȃɂ����Ȃ�
      if (object->GetObjType() != Object::ObjectBase::ObjectType::Bullet) {
         continue;
      }
      //���u��U���̒e�̋��̍쐬
      //���̈ʒu
      auto bullet = object->position();
      //���̔��a
      auto bulletRadius = static_cast<float>(_DoubleParam("bullet_radius"));
      //���f���Ƌ��̓����蔻������
      auto result = MV1CollCheck_Sphere(poorEnemyGatlingModel, collision, AppFrame::Math::ToDX(bullet), bulletRadius);
      //�����蔻��̌��ʂ��m�F
      if (result.HitNum > 0) {
         //�������Ă����牓�u��U���̒e�̓����蔻�茋�ʂ��K�g�����O�U�������Ă���G���G�Ɠ��������Ɛݒ�
         object->collisionComponent().report().id(ReportId::HitFromPoorEnemy);
         //�_���[�W�̐ݒ�
         _owner.collisionComponent().damage(_DoubleParam("poorenemy_bullet_damage"));
         //�K�g�����O�U�������Ă���G���G�̓����蔻�茋�ʂ����u��U���̒e�Ɠ��������Ɛݒ�
         _owner.collisionComponent().report().id(ReportId::HitFromBullet);
      }
   }
}

void CollisionComponent::PoorEnemyGatlingFromObjectModel() {
   /**
    * \brief double�^�̒l�𕶎���Ŏw�肵�A�l�Ǘ��N���X����擾����
    * \param paramName �l���w�肷�镶����
    * \return ������ɂ��w�肳�ꂽ�l
    */
   const auto _DoubleParam = [&](std::string paramName) {
      return _param->GetDoubleParam(paramName);
   };
   // �����I�u�W�F�N�g�̎Q�ƌ^�ɃL���X�g
   auto& fallObject = dynamic_cast<Enemy::FallObject&>(_owner);
   //��������I�u�W�F�N�g�̃J�v�Z���̍쐬
   auto fallObjectPos = fallObject.position();
   //�J�v�Z���̈�ڂ̈ʒu
   auto capsulePos1 = fallObject.capsulePos1();
   //�J�v�Z���̓�ڂ̈ʒu
   auto capsulePos2 = fallObject.capsulePos2();
   //�J�v�Z���̔��a
   auto capsuleRadian = static_cast<float>(_DoubleParam("fallobject_drum_radius"));
   //�I�u�W�F�N�g�T�[�o�[�̊e�I�u�W�F�N�g���擾
   auto& runObjects = Game::Game::GetObjServer().runObjects();
   for (auto&& object : runObjects) {
      //�K�g�����O�U�������Ă���G���G����Ȃ���Ή������Ȃ�
      if (object->GetObjType() != Object::ObjectBase::ObjectType::PoorEnemy) {
         continue;
      }
      //�K�g�����O�U�������Ă���G���G�̃��f���n���h�����擾
      auto poorEnemyGatlingModel = object->modelAnimeComponent().modelHandle();
      //���f���̃R���W�����t���[���ԍ����擾
      auto collision = object->modelAnimeComponent().FindFrame("mob");
      //���f���ƃJ�v�Z���̓����蔻������
      auto result = MV1CollCheck_Capsule(poorEnemyGatlingModel, collision, AppFrame::Math::ToDX(capsulePos1), AppFrame::Math::ToDX(capsulePos2), capsuleRadian);
      //�����蔻��̌��ʂ��m�F
      if (result.HitNum > 0) {
         //�K�g�����O�U�������Ă���G���G�̓����蔻�茋�ʂ𗎉�����I�u�W�F�N�g�Ɠ��������Ɛݒ�
         object->collisionComponent().report().id(ReportId::HitFromFallObject);
         //�_���[�W�̐ݒ�
         object->collisionComponent().damage(_DoubleParam("poorenemy_object_damage"));
         //��������I�u�W�F�N�g�̓����蔻�茋�ʂ��K�g�����O�U�������Ă���G���G�Ɠ��������Ɛݒ�
         _owner.collisionComponent().report().id(ReportId::HitFromPoorEnemy);
      }
   }
}

void CollisionComponent::PlayerKnockBack() {
   // �I�u�W�F�N�g�T�[�o�[�̊e�I�u�W�F�N�g���擾
   auto& runObjects = Game::Game::GetObjServer().runObjects();
   for (auto&& object : runObjects) {
      // �v���C���[����Ȃ������牽�����Ȃ�
      if (object->GetObjType() != Object::ObjectBase::ObjectType::Player) {
         continue;
      }
      // �v���C���[���m�b�N�o�b�N���Ă��邩
      if (object->collisionComponent().knockBack()) {
         // �m�b�N�o�b�N���Ă�����v���C���[���m�b�N�o�b�N���Ă���Ɛݒ�
         _owner.collisionComponent().knockBack(true);
      }
   }
}

AppFrame::Math::Vector4 CollisionComponent::PlayerCheckStage(const Vector4& pos, const Vector4& moved) {
   // �I�u�W�F�N�g�T�[�o�[�̊e�I�u�W�F�N�g���擾
   auto& runObjects = Game::Game::GetObjServer().runObjects();
   for (auto&& object : runObjects) {
      // �v���C���[�������ꍇ
      if (object->GetObjType() == Object::ObjectBase::ObjectType::Player) {
         auto& player = dynamic_cast<Player::Player&>(*object);
         // �v���C���[�����S���[�V�����Ȃ�ΕԂ�
         if (player.isDeadMotion()) {
            return pos;
         };
      }
   }
   // �X�e�[�W�̃��f���n���h���Ɠ����蔻��̃t���[���ԍ��̎擾
   auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
   auto modeBase = modeServer.GetNowMode();
   auto modeIngame = std::dynamic_pointer_cast<Mode::ModeInGameBase>(modeBase);
   auto stageComponent = modeIngame->GetStage().stageComponent();
   auto [handle, collision] = stageComponent.GetHandleAndFrameNum("stage_character_c");
   // �ړ���̈ʒu�̎擾
   auto newPos = pos + moved;
   // �ړ���̈ʒu���X�e�[�W�Ɠ������Ă��邩�m�F
   auto start = newPos + Vector4(0.0, 50.0, 0.0);
   auto end = newPos + Vector4(0.0, -10000.0, 0.0);
   auto result = MV1CollCheck_Line(handle, collision, AppFrame::Math::ToDX(start), AppFrame::Math::ToDX(end));
   if (result.HitFlag != 0) {
      // �ړ���̈ʒu���X�e�[�W�Ɠ������Ă���ꍇ�ړ���̈ʒu��Ԃ�
      newPos = AppFrame::Math::ToMath(result.HitPosition);
      return newPos;
   }
   else {
      // �ړ���̈ʒu���X�e�[�W�Ɠ������Ă��Ȃ��ꍇ�ړ��O�̈ʒu��Ԃ�
      return pos;
   }
}

AppFrame::Math::Vector4 CollisionComponent::LargeEnemyCheckStage(const Vector4& pos, const Vector4& moved) {
   // �I�u�W�F�N�g�T�[�o�[�̊e�I�u�W�F�N�g���擾
   auto& runObjects = Game::Game::GetObjServer().runObjects();
   for (auto&& object : runObjects) {
      // �v���C���[�������ꍇ
      if (object->GetObjType() == Object::ObjectBase::ObjectType::Player) {
         // �v���C���[�����S���[�V�����Ȃ�ΕԂ�
         auto& player = dynamic_cast<Player::Player&>(*object);
         if (player.isDeadMotion()) {
            return pos;
         };
      }
   }
   // �X�e�[�W�̃��f���n���h���Ɠ����蔻��̃t���[���ԍ��̎擾
   auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
   auto modeBase = modeServer.GetNowMode();
   auto modeIngame = std::dynamic_pointer_cast<Mode::ModeInGameBase>(modeBase);
   auto stageComponent = modeIngame->GetStage().stageComponent();
   auto [handle, collision] = stageComponent.GetHandleAndFrameNum("stage_boss_c");
   // �ړ���̈ʒu�̎擾
   auto newPos = pos + moved;
   // �ړ���̈ʒu���X�e�[�W�Ɠ������Ă��邩�m�F
   auto start = newPos + Vector4(0.0, 50.0, 0.0);
   auto end = newPos + Vector4(0.0, -10000.0, 0.0);
   auto result = MV1CollCheck_Line(handle, collision, AppFrame::Math::ToDX(start), AppFrame::Math::ToDX(end));
   if (result.HitFlag != 0) {
      // �ړ���̈ʒu���X�e�[�W�Ɠ������Ă���ꍇ�ړ���̈ʒu��Ԃ�
      newPos = AppFrame::Math::ToMath(result.HitPosition);
      return newPos;
   }
   else {
      // �ړ���̈ʒu���X�e�[�W�Ɠ������Ă��Ȃ��ꍇ�ړ��O�̈ʒu��Ԃ�
      return pos;
   }
}

AppFrame::Math::Vector4 CollisionComponent::PoorEnemyCheckStage(const Vector4& pos, const Vector4& moved) {
   auto& runObjects = Game::Game::GetObjServer().runObjects();
   for (auto&& object : runObjects) {
      if (object->GetObjType() == Object::ObjectBase::ObjectType::Player) {
         // �v���C���[�����S���[�V�����Ȃ�ΕԂ�
         auto& player = dynamic_cast<Player::Player&>(*object);
         if (player.isDeadMotion()) {
            return pos;
         };
      }
   }
   auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
   auto modeBase = modeServer.GetNowMode();
   auto modeIngame = std::dynamic_pointer_cast<Mode::ModeInGameBase>(modeBase);
   auto stageComponent = modeIngame->GetStage().stageComponent();

   auto [handle, collision] = stageComponent.GetHandleAndFrameNum("stage_character_c");

   auto newPos = pos + moved;
   auto start = newPos + Vector4(0.0, 50.0, 0.0);
   auto end = newPos + Vector4(0.0, -10000.0, 0.0);
   auto result = MV1CollCheck_Line(handle, collision, AppFrame::Math::ToDX(start), AppFrame::Math::ToDX(end));
   if (result.HitFlag) {
      newPos = AppFrame::Math::ToMath(result.HitPosition);
      return newPos;
   }
   else {
      return pos;
   }
}

void CollisionComponent::OutStage() {
   auto& runObjects = Game::Game::GetObjServer().runObjects();
   for (auto&& object : runObjects) {
      if (object->GetObjType() == Object::ObjectBase::ObjectType::Player) {
         auto& player = dynamic_cast<Player::Player&>(*object);
         // �v���C���[�����S���[�V�����Ȃ�ΕԂ�
         if (player.isDeadMotion()) {
            return;
         };
      }
   }
   auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
   auto modeBase = modeServer.GetNowMode();
   auto modeInGameBase = std::dynamic_pointer_cast<Mode::ModeInGameBase>(modeBase);
   auto stageComponent = modeInGameBase->GetStage().stageComponent();

   auto [handle, collision] = stageComponent.GetHandleAndFrameNum("stage_object_c");
   // �ʒu�̎擾
   auto position = _owner.position();
   // �����̍쐬
   // �n�_
   auto start = position + Vector4(0.0, 50.0, 0.0);
   // �I�_
   auto end = position + Vector4(0.0, -10000.0, 0.0);

   auto result = MV1CollCheck_Line(handle, collision, AppFrame::Math::ToDX(start), AppFrame::Math::ToDX(end));

   if (!result.HitFlag) {
      _owner.collisionComponent().report().id(ReportId::OutStage);
      return;
   }
   return;
}

bool CollisionComponent::IsLineFromStage(const Vector4& pos) {
   auto& runObjects = Game::Game::GetObjServer().runObjects();
   for (auto&& object : runObjects) {
      if (object->GetObjType() == Object::ObjectBase::ObjectType::Player) {
         auto& player = dynamic_cast<Player::Player&>(*object);
         // �v���C���[�����S���[�V�����Ȃ�ΕԂ�
         if (player.isDeadMotion()) {
            return false;
         };
      }
   }
   auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
   auto modeBase = modeServer.GetNowMode();
   auto modeInGameBase = std::dynamic_pointer_cast<Mode::ModeInGameBase>(modeBase);
   auto stageComponent = modeInGameBase->GetStage().stageComponent();

   auto [handle, collision] = stageComponent.GetHandleAndFrameNum("stage_character_c");
   // �ʒu�̎擾
   auto [x, y, z] = pos.GetVec3();
   // �����̍쐬
   auto start = Vector4(x, 50.0, z);   // �n�_
   auto end = Vector4(x, -10000.0, z); // �I�_

   auto result = MV1CollCheck_Line(handle, collision, AppFrame::Math::ToDX(start), AppFrame::Math::ToDX(end));

   return result.HitFlag;
}
