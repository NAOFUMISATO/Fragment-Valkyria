
/*****************************************************************//**
 * \file   CollisionServer.cpp
 * \brief  �����蔻�菈�����s���N���X
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "CollisionComponent.h"
#include "ModelAnimeComponent.h"
#include "Gatling.h"
#include "GameMain.h"
#include "Player.h"
#include "Stage.h"
#include "StageModelComponent.h"
#include "LoadStageFromJson.h"
#include "Laser.h"
#include "ModeBoss.h"
#include "ModePoor.h"
#include "ObjectBase.h"
#ifdef _DEBUG
#include <stdexcept>
#include <windows.h>
#endif

using namespace FragmentValkyria::Collision;

namespace {
	auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("collision",
		{ "fallobject_range", "ply_radius", "ply_capsule_pos1",
		 "ply_capsule_pos2", "gatling_radius", "fallobject_capsule_pos1",
		"fallobject_capsule_pos2", "fallobject_radius", "laser_radius"});

	const double FallObjectRange = paramMap["fallobject_range"];                      //!< ��������I�u�W�F�N�g�������グ����͈͂̋��̔��a
	const double PlayerRadius = paramMap["ply_radius"];                               //!< �v���C���[�̃J�v�Z���̔��a
	const double PlayerCapsulePos1 = paramMap["ply_capsule_pos1"];                    //!< �v���C���[�̃J�v�Z�����`������2�_���̈�_�̍��W�܂ł̃v���C���[�̈ʒu����̋���
	const double PlayerCapsulePos2 = paramMap["ply_capsule_pos2"];                    //!< �v���C���[�̃J�v�Z�����`������2�_���̈�_�̍��W�܂ł̃v���C���[�̈ʒu����̋���
	const double GatlingRadius = paramMap["gatling_radius"];                          //!< �K�g�����O�̔��a
	const double FallObjectCapsulePos1 = paramMap["fallobject_capsule_pos1"];         //!< �t�H�[���I�u�W�F�N�g�̃J�v�Z�����`������2�_���̈�_�̍��W�܂ł̃t�H�[���I�u�W�F�N�g�̈ʒu����̋���
	const double FallObjectCapsulePos2 = paramMap["fallobject_capsule_pos2"];         //!< �t�H�[���I�u�W�F�N�g�̃J�v�Z�����`������2�_���̈�_�̍��W�܂ł̃t�H�[���I�u�W�F�N�g�̈ʒu����̋���
	const double FallObjectRadius = paramMap["fallobject_radius"];                    //!< �t�H�[���I�u�W�F�N�g�̃J�v�Z���̔��a
	const double LaserRadius = paramMap["laser_radius"];                              //!<���[�U�[�̃J�v�Z���̔��a
}

CollisionComponent::CollisionComponent(Object::ObjectBase& owner) : _owner{ owner } {
	//�����蔻��̌��ʂ��Ǘ�����N���X�̃|�C���^�쐬
	_report = std::make_unique<Report>();
}

void CollisionComponent::ObjectRangeFromPlayer() {
	//�v���C���[�̈ʒu�擾
	auto plyPoint = _owner.position();
	//�I�u�W�F�N�g�T�[�o�[�̊e�I�u�W�F�N�g���擾
	for (auto&& object : _owner.GetObjServer().runObjects()) {
		//�I�u�W�F�N�g�x�[�X�^�ɃL���X�g
		auto& objectBase = ObjectBaseCast(*object);
		//��������I�u�W�F�N�g����Ȃ��ꍇ�Ȃɂ����Ȃ�
		if (objectBase.GetObjType() != Object::ObjectBase::ObjectType::FallObject) {
			continue;
		}
		//��������I�u�W�F�N�g�̈ʒu�擾
		auto objectPos = objectBase.position();
		//��������I�u�W�F�N�g�������グ���鋅�͈̔͂̔��a
		auto objectRadian = FallObjectRange;
		//���O�̋����`
		AppFrame::Math::Sphere objectRange = std::make_tuple(objectPos, objectRadian);
		//���Ɠ_�̓����蔻����Ƃ�
		if (AppFrame::Math::Utility::CollisionSpherePoint(plyPoint, objectRange)) {
			//�������Ă����痎������I�u�W�F�N�g�̓����蔻��̌��ʂɃv���C���[�ɓ������Ă���Ɛݒ�
			objectBase.collisionComponent().report().id(ReportId::HitFromPlayer);
			break;
		}
		else {
			//�������Ă��Ȃ������痎������I�u�W�F�N�g�̓����蔻��̌��ʂɓ������Ă��Ȃ��Ɛݒ�
			objectBase.collisionComponent().report().id(ReportId::None);
		}
	}
}

void CollisionComponent::PlayerFromObjectRange() {
	//��������I�u�W�F�N�g�̈ʒu���擾
	auto objectPos = _owner.position();
	//��������I�u�W�F�N�g�������グ����͈͂̋��̔��a
	auto objectRadian = FallObjectRange;
	//���O�̋����`
	AppFrame::Math::Sphere objectRange = std::make_tuple(objectPos, objectRadian);
	//�I�u�W�F�N�g�T�[�o�[�̊e�I�u�W�F�N�g���擾
	for (auto&& object : _owner.GetObjServer().runObjects()) {
		//�I�u�W�F�N�g�x�[�X�^�ɃL���X�g
		auto& objectBase = ObjectBaseCast(*object);
		//�����蔻��̌��ʂ��v���C���[�Ɠ������Ă��邩�m�F
		if (objectBase.collisionComponent().report().id() == ReportId::HitFromPlayer) {
			//�I�u�W�F�N�g�T�[�o�[�̊e�I�u�W�F�N�g���擾
			for (auto&& plyObject : _owner.GetObjServer().runObjects()) {
				//�I�u�W�F�N�g�x�[�X�^�ɃL���X�g
				auto& ply = ObjectBaseCast(*plyObject);
				//�v���C���[����Ȃ���Ή������Ȃ�
				if (ply.GetObjType() != Object::ObjectBase::ObjectType::Player) {
					continue;
				}
				//���̃I�u�W�F�N�g�ɓ������Ă����ꍇ�͏I������
				if (objectBase.collisionComponent().report().id() == ReportId::HitFromIdleFallObject) {
					break;
				}
				if (objectBase.collisionComponent().report().id() == ReportId::HitFromFallObject) {
					break;
				}
				if (objectBase.collisionComponent().report().id() == ReportId::HitFromGatling) {
					break;
				}
				if (objectBase.collisionComponent().report().id() == ReportId::HitFromLargeEnemy) {
					break;
				}
				//�v���C���[�����̃I�u�W�F�N�g�Ɠ������Ă��Ȃ������痎������I�u�W�F�N�g�������グ���鋅�͈̔͂ɂ���Ɛݒ�
				ply.collisionComponent().report().id(ReportId::HitFromObjectRange);
			}
			break;
		}
		//�v���C���[����Ȃ���Ή������Ȃ�
		if (objectBase.GetObjType() != Object::ObjectBase::ObjectType::Player) {
			continue;
		}
		//���̃I�u�W�F�N�g�ɓ������Ă����ꍇ�͏I������
		if (objectBase.collisionComponent().report().id() == ReportId::HitFromIdleFallObject) {
			break;
		}
		if (objectBase.collisionComponent().report().id() == ReportId::HitFromFallObject) {
			break;
		}
		if (objectBase.collisionComponent().report().id() == ReportId::HitFromGatling) {
			break;
		}
		if (objectBase.collisionComponent().report().id() == ReportId::HitFromLargeEnemy) {
			break;
		}
		//�v���C���[�̈ʒu�擾
		auto plyPoint = objectBase.position();
		//���Ɠ_�̓����蔻����Ƃ�
		if (AppFrame::Math::Utility::CollisionSpherePoint(plyPoint, objectRange)) {
			//�������Ă�����v���C���[�̓����蔻��̌��ʂɃI�u�W�F�N�g�������グ����͈͂ɂ���Ɛݒ�
			objectBase.collisionComponent().report().id(ReportId::HitFromObjectRange);
		} 
		else {
			//�������Ă��Ȃ�������v���C���[�̓����蔻��̌��ʂɓ������Ă��Ȃ��Ɛݒ�
			objectBase.collisionComponent().report().id(ReportId::None);
		}
	}
}

void CollisionComponent::PlayerFromFallObjectModel(bool fall) {
	//��������I�u�W�F�N�g�̃��f���̃n���h���擾
	auto objectModel = _owner.modelAnimeComponent().modelHandle();
	//���f���̃R���W�����̃t���[���ԍ��擾
	auto collision = MV1SearchFrame(objectModel, "drum_green_c");
	//�I�u�W�F�N�g�T�[�o�[�̊e�I�u�W�F�N�g���擾
	for (auto&& object : _owner.GetObjServer().runObjects()) {
		//�I�u�W�F�N�g�x�[�X�^�ɃL���X�g
		auto& objectBase = ObjectBaseCast(*object);
		//�v���C���[����Ȃ������牽�����Ȃ�
		if (objectBase.GetObjType() != Object::ObjectBase::ObjectType::Player) {
			continue;
		}
		//�����蔻��̌��ʂ��ҋ@��Ԃ̗�������I�u�W�F�N�g�Ɠ������Ă���ꍇ�������Ȃ�
		if (objectBase.collisionComponent().report().id() == ReportId::HitFromIdleFallObject) {
			continue;
		}
		//�v���C���[�̎Q�ƌ^�̎擾
		auto& player = dynamic_cast<Player::Player&>(objectBase);
		//���G���Ԃ̎擾
		auto invincibleCnt = player.invincibleCnt();
		// �v���C���[���̃J�v�Z����ݒ�
		auto plyPos = objectBase.position();
		//�J�v�Z���̈�ڂ̈ʒu
		auto pos1 = plyPos + AppFrame::Math::Vector4(0.0, PlayerCapsulePos1, 0.0);
		//�J�v�Z���̓�ڂ̈ʒu
		plyPos.Add(0.0, PlayerCapsulePos2, 0.0);
		//�J�v�Z���̔��a
		auto radian = static_cast<float>(PlayerRadius);
		//�J�v�Z���ƃ��f���̓����蔻����Ƃ�
		auto result = MV1CollCheck_Capsule(objectModel, collision, AppFrame::Math::ToDX(plyPos), AppFrame::Math::ToDX(plyPos), radian);
		//�����蔻��̌��ʂ��������Ă��邩�m�F
		if (result.HitNum > 0) {
			//�������Ă����ꍇ�������m�F
			if (fall) {
				//�������������ꍇ���G���Ԃ�������I��
				if (invincibleCnt > 0) {
					return;
				}
				//���G���Ԃ���Ȃ��ꍇ���������ʒu�ɗ�������I�u�W�F�N�g�̈ʒu��ݒ�
				objectBase.collisionComponent().hitPos(_owner.position());
				//�v���C���[�̓����蔻��̌��ʂɗ�������I�u�W�F�N�g�̃��f���Ɠ��������Ɛݒ�
				objectBase.collisionComponent().report().id(ReportId::HitFromFallObject);
				//�_���[�W��20.0�ɐݒ�
				objectBase.collisionComponent().damage(20.0);
			}
			else {
				//����������Ȃ��ꍇ���������ʒu�ɓ��������|���S���̖@����ݒ�
				objectBase.collisionComponent().hitPos(AppFrame::Math::ToMath(result.Dim[0].Normal));
				//�v���C���[�̓����蔻��̌��ʂɑҋ@��Ԃ̗�������I�u�W�F�N�g�Ɠ��������Ɛݒ�
				objectBase.collisionComponent().report().id(ReportId::HitFromIdleFallObject);
			}
		}
	}
}

void CollisionComponent::GatlingFromObjectModel() {
	//��������I�u�W�F�N�g�̃��f���̃n���h���̎擾
	auto objectModel = _owner.modelAnimeComponent().modelHandle();
	//���f���̃R���W�����̃t���[���ԍ��擾
	auto collision = MV1SearchFrame(objectModel, "drum_green_c");
	//�I�u�W�F�N�g�T�[�o�[�̊e�I�u�W�F�N�g���擾
	for (auto&& object : _owner.GetObjServer().runObjects()) {
		//�I�u�W�F�N�g�x�[�X�^�ɃL���X�g
		auto& objectBase = ObjectBaseCast(*object);
		//�K�g�����O����Ȃ������牽�����Ȃ�
		if (objectBase.GetObjType() != Object::ObjectBase::ObjectType::Gatling) {
			continue;
		}
		// �K�g�����O���̋���ݒ�
		auto gatlingPos = objectBase.position();
		auto radian = static_cast<float>(GatlingRadius);
		//���ƃ��f���̓����蔻��̌��ʂ��擾
		auto result = MV1CollCheck_Sphere(objectModel, collision, AppFrame::Math::ToDX(gatlingPos), radian);
		//�����蔻��̌��ʂ��������Ă��邩�m�F
		if (result.HitNum > 0) {
			//�K�g�����O�̓����蔻�茋�ʂɑҋ@��Ԃ̗�������I�u�W�F�N�g�Ɠ��������Ɛݒ�
			objectBase.collisionComponent().report().id(ReportId::HitFromIdleFallObject);
		}
	}
}

void CollisionComponent::GatlingFromPlayer() {
	//�v���C���[�̎Q�ƌ^�ɃL���X�g
	auto& player = dynamic_cast<Player::Player&>(_owner);
	//���G���Ԃ̎擾
	auto invincibleCnt = player.invincibleCnt();
	//���G���Ԓ��������牽�����Ȃ�
	if (invincibleCnt > 0) {
		return;
	}
	//�v���C���[���̃J�v�Z����ݒ�
	auto playerPos = _owner.position();
	//�J�v�Z���̈ʈ�ڂ̈ʒu
	auto capsulePos1 = playerPos + Vector4(0.0, 30.0, 0.0);
	//�J�v�Z���̓�ڂ̈ʒu
	auto capsulePos2 = playerPos + Vector4(0.0, 60.0, 0.0);
	//�J�v�Z���̔��a
	auto casuleRadian = PlayerRadius;
	//���O�̃J�v�Z�����`
	AppFrame::Math::Capsule playerCapsule = std::make_tuple(capsulePos1, capsulePos2, casuleRadian);
	//�I�u�W�F�N�g�T�[�o�[�̊e�I�u�W�F�N�g���擾
	for (auto&& object : _owner.GetObjServer().runObjects()) {
		//�I�u�W�F�N�g�x�[�X�^�ɃL���X�g
		auto& objectBase = ObjectBaseCast(*object);
		//�K�g�����O����Ȃ������牽�����Ȃ�
		if (objectBase.GetObjType() != Object::ObjectBase::ObjectType::Gatling) {
			continue;
		}
		//�K�g�����O�̈ʒu�̎擾
		auto gatling = objectBase.position();
		//�K�g�����O�̔��a�̐ݒ�
		auto gatlingRadian = GatlingRadius;
		//���O�̋����`
		AppFrame::Math::Sphere gatlingSphere = std::make_tuple(gatling, gatlingRadian);
		//�J�v�Z���Ƌ��œ����蔻����Ƃ�
		if (AppFrame::Math::Utility::CollisionCapsuleSphere(playerCapsule, gatlingSphere)) {
			//�������Ă�����K�g�����O���̓����蔻��̌��ʂ��v���C���[�Ɠ��������Ɛݒ�
			objectBase.collisionComponent().report().id(ReportId::HitFromPlayer);
			//�v���C���[���̓����蔻��̌��ʂ��K�g�����O�Ɠ��������Ɛݒ�
			_owner.collisionComponent().report().id(ReportId::HitFromGatling);
			//�_���[�W�̐ݒ�
			_owner.collisionComponent().damage(20.0);
			//�v���C���[�̓��������ʒu�ɃK�g�����O�̈ʒu��ݒ�
			_owner.collisionComponent().hitPos(objectBase.position());
		}
	}
}

void CollisionComponent::ObjectModelFromLargeEnemy() {
	//��������I�u�W�F�N�g�̃J�v�Z�����쐬
	auto fallObjectPos = _owner.position();
	//�J�v�Z���̈�ڂ̈ʒu
	auto capsulePos1 = Vector4(0.0, FallObjectCapsulePos1, 0.0) + fallObjectPos;
	//�J�v�Z���̓�ڂ̈ʒu
	auto capsulePos2 = Vector4(0.0, FallObjectCapsulePos2, 0.0) + fallObjectPos;
	//�J�v�Z���̔��a
	auto capsuleRadian = static_cast<float>(FallObjectRadius);
	//�I�u�W�F�N�g�T�[�o�[�̊e�I�u�W�F�N�g���擾
	for (auto&& object : _owner.GetObjServer().runObjects()) {
		//�I�u�W�F�N�g�x�[�X�^�ɃL���X�g
		auto& objectBase = ObjectBaseCast(*object);
		//���[�W�G�l�~�[����Ȃ��ꍇ�������Ȃ�
		if (objectBase.GetObjType() != Object::ObjectBase::ObjectType::LargeEnemy) {
			continue;
		}
		//���[�W�G�l�~�[�̃��f���̃n���h���̎擾
		auto largeEnemyModel = objectBase.modelAnimeComponent().modelHandle();
		//���[�W�G�l�~�[�̃��f���̃R���W�����t���[���ԍ��̎擾
		auto collision = objectBase.modelAnimeComponent().FindFrame("S301_typeCO");
		//���f���ƃJ�v�Z���̓����蔻������
		auto result = MV1CollCheck_Capsule(largeEnemyModel, collision, AppFrame::Math::ToDX(capsulePos1), AppFrame::Math::ToDX(capsulePos2), capsuleRadian);
		//�����蔻��̌��ʂ��������Ă��邩�m�F
		if (result.HitNum > 0) {
			//�������Ă����烉�[�W�G�l�~�[�̓����蔻�茋�ʂ𗎉�����I�u�W�F�N�g�Ɠ������Ă���Ɛݒ�
			objectBase.collisionComponent().report().id(ReportId::HitFromFallObject);
			//�_���[�W�̐ݒ�
			objectBase.collisionComponent().damage(20.0);
			//��������I�u�W�F�N�g�̓����蔻�茋�ʂ����[�W�G�l�~�[�Ɠ��������Ɛݒ�
			_owner.collisionComponent().report().id(ReportId::HitFromLargeEnemy);
		}
    }
}

void CollisionComponent::LargeEnemyFromBullet() {
	//���u��U���̒e�̈ʒu���擾
	auto bulletPos = _owner.position();
	//���u��U���̒e�̔��a��ݒ�
	auto bulletRadius = static_cast<float>(20.0);
	//�I�u�W�F�N�g�T�[�o�[�̊e�I�u�W�F�N�g���擾
	for (auto&& object : _owner.GetObjServer().runObjects()) {
		//�I�u�W�F�N�g�x�[�X�^�ɃL���X�g
		auto& objectBase = ObjectBaseCast(*object);
		//���[�W�G�l�~�[����Ȃ������牽�����Ȃ�
		if (objectBase.GetObjType() != Object::ObjectBase::ObjectType::LargeEnemy) {
			continue;
		}
		//���[�W�G�l�~�[�̃��f���̃n���h�����擾
		auto largeEnemyModel = objectBase.modelAnimeComponent().modelHandle();
		//���[�W�G�l�~�[�̃��f���̃R���W�����t���[���ԍ��̎擾
		auto collision = objectBase.modelAnimeComponent().FindFrame("S301_typeCO");
		//���f���Ƌ��̓����蔻������
		auto result = MV1CollCheck_Sphere(largeEnemyModel, collision, AppFrame::Math::ToDX(bulletPos), bulletRadius);
		//�����蔻��̌��ʂ��瓖�����Ă��邩�m�F
		if (result.HitNum > 0) {
			//�������Ă����烉�[�W�G�l�~�[�̓����蔻�茋�ʂ����u��U���̒e�Ɠ��������Ɛݒ�
			objectBase.collisionComponent().report().id(ReportId::HitFromBullet);
			//�_���[�W�̐ݒ�
			objectBase.collisionComponent().damage(10.0);
			//���u��U���̒e�̓����蔻�茋�ʂ����[�W�G�l�~�[�Ɠ��������Ɛݒ�
			_owner.collisionComponent().report().id(ReportId::HitFromLargeEnemy);
		}
	}
}

void CollisionComponent::FallObjectFromLaser() {
	//��������I�u�W�F�N�g�̃J�v�Z���̍쐬
	auto fallObjectPos = _owner.position();
	//�J�v�Z���̈�ڂ̈ʒu
	auto fallObjectCapsulePos1 = Vector4(0.0, FallObjectCapsulePos1, 0.0) + fallObjectPos;
	//�J�v�Z���̓�ڂ̈ʒu
	auto fallObjectCapsulePos2 = Vector4(0.0, FallObjectCapsulePos2, 0.0) + fallObjectPos;
	//�J�v�Z���̔��a
	auto fallObjectRadius = FallObjectRadius;
	//���O�̃J�v�Z�����`
	auto fallObjectCapsule = std::make_tuple(fallObjectCapsulePos1, fallObjectCapsulePos2, fallObjectRadius);
	//�I�u�W�F�N�g�T�[�o�[�̊e�I�u�W�F�N�g���擾
	for (auto&& object : _owner.GetObjServer().runObjects()) {
		//�I�u�W�F�N�g�x�[�X�^�ɃL���X�g
		auto& objectBase = ObjectBaseCast(*object);
		//���[�U�[����Ȃ������牽�����Ȃ�
		if (objectBase.GetObjType() != Object::ObjectBase::ObjectType::Laser) {
			continue;
		}
		//���[�U�[�̎Q�ƌ^�ɃL���X�g
		auto& laser = dynamic_cast<Enemy::Laser&>(objectBase);
		//���[�U�[�̃J�v�Z�����쐬
		//�J�v�Z���̈�ڂ̈ʒu
		auto laserCapsulePos1 = laser.position();
		//�J�v�Z���̓�ڂ̈ʒu
		auto laserCapsulePos2 = laser.end();
		//�J�v�Z���̔��a
		auto laserRadius = LaserRadius;
		//���O�̃J�v�Z�����`
		auto laserCapsule = std::make_tuple(laserCapsulePos1, laserCapsulePos2, laserRadius);
		//�J�v�Z���ƃJ�v�Z���̓����蔻������
		if (AppFrame::Math::Utility::CollisionCapsuleCapsule(fallObjectCapsule, laserCapsule)) {
			//�������Ă����痎������I�u�W�F�N�g�̓����蔻�茋�ʂ����[�U�[�Ɠ��������Ɛݒ�
			_owner.collisionComponent().report().id(ReportId::HitFromLaser);
		}
		break;
	}
}

void CollisionComponent::PlayerFromLaser() {
	//�v���C���[�̎Q�ƌ^�̎擾
	auto& player = dynamic_cast<Player::Player&>(_owner);
	//���G���Ԃ̎擾
	auto invincibleCnt = player.invincibleCnt();
	//���G���Ԓ��������ꍇ�������Ȃ�
	if (invincibleCnt > 0) {
		return;
	}
	//�v���C���[�̃J�v�Z���̍쐬
	auto playerPos = _owner.position();
	//�J�v�Z���̈�ڂ̈ʒu
	auto plyCapsulePos1 = Vector4(0.0, PlayerCapsulePos1, 0.0) + playerPos;
	//�J�v�Z���̓�ڂ̈ʒu
	auto plyCapsulePos2 = Vector4(0.0, PlayerCapsulePos2, 0.0) + playerPos;
	//�J�v�Z���̔��a
	auto playerCapsuleRadius = PlayerRadius;
	//���O�̃J�v�Z�����`
	AppFrame::Math::Capsule playerCapsule = std::make_tuple(plyCapsulePos1, plyCapsulePos2, playerCapsuleRadius);
	//�I�u�W�F�N�g�T�[�o�[�̊e�I�u�W�F�N�g���擾
	for (auto&& object : _owner.GetObjServer().runObjects()) {
		//�I�u�W�F�N�g�x�[�X�^�ɃL���X�g
		auto& objectBase = ObjectBaseCast(*object);
		//���[�U�[����Ȃ������牽�����Ȃ�
		if (objectBase.GetObjType() != Object::ObjectBase::ObjectType::Laser) {
			continue;
		}
		//���[�U�[�̎Q�ƌ^�ɃL���X�g
		auto& laser = dynamic_cast<Enemy::Laser&>(objectBase);
		//���[�U�[�̃J�v�Z�����쐬
		//�J�v�Z���̈�ڂ̈ʒu
		auto laserCapsulePos1 = laser.position();
		//�J�v�Z���̓�ڂ̈ʒu
		auto laserCapsulePos2 = laser.end();
		//�J�v�Z���̔��a
		auto laserRadius = LaserRadius;
		//���O�̃J�v�Z�����`
		auto laserCapsule = std::make_tuple(laserCapsulePos1, laserCapsulePos2, laserRadius);
		//�J�v�Z���ƃJ�v�Z���̓����蔻������
		if (AppFrame::Math::Utility::CollisionCapsuleCapsule(playerCapsule, laserCapsule)) {
			//�������Ă����ꍇ�v���C���[�̓����蔻��̌��ʂ����[�U�[�Ɠ��������Ɛݒ�
			_owner.collisionComponent().report().id(ReportId::HitFromLaser);
			//�_���[�W�̐ݒ�
			_owner.collisionComponent().damage(20.0);
			//���������ʒu�Ƀ��[�U�[�̈ʒu��ݒ�
			_owner.collisionComponent().hitPos(laser.position());
		}
		break;
	}
}

void CollisionComponent::LargeEnemyFromPlayer() {
	//���[�W�G�l�~�[�̃��f���̃n���h���̎擾
	auto largeEnemyModel = _owner.modelAnimeComponent().modelHandle();
	//���[�W�G�l�~�[�̃��f���̃R���W�����t���[���ԍ��̎擾
	auto collision = _owner.modelAnimeComponent().FindFrame("S301_typeCO");
	//�I�u�W�F�N�g�T�[�o�[�̊e�I�u�W�F�N�g���擾
	for (auto&& object : _owner.GetObjServer().runObjects()) {
		//�I�u�W�F�N�g�x�[�X�^�ɃL���X�g
		auto& objectBase = ObjectBaseCast(*object);
		//�v���C���[����Ȃ������牽�����Ȃ�
		if (objectBase.GetObjType() != Object::ObjectBase::ObjectType::Player) {
			continue;
		}
		//�v���C���[�^�̎Q�Ƃ̎擾
		auto& player = dynamic_cast<Player::Player&>(objectBase);
		//���G���Ԃ̎擾
		auto invincibleCnt = player.invincibleCnt();
		//���G���Ԓ��������牽�����Ȃ�
		if (invincibleCnt > 0) {
			return;
		}
		//�v���C���[�̃J�v�Z���̍쐬
		auto playerPos = objectBase.position();
		//�J�v�Z���̈�ڂ̈ʒu
		auto plyCapsulePos1 = Vector4(0.0, PlayerCapsulePos1, 0.0) + playerPos;
		//�J�v�Z���̓�ڂ̈ʒu
		auto plyCapsulePos2 = Vector4(0.0, PlayerCapsulePos2, 0.0) + playerPos;
		//�J�v�Z���̔��a
		auto playerCapsuleRadius = static_cast<float>(PlayerRadius);
		//���f���ƃJ�v�Z���̓����蔻������
		auto result = MV1CollCheck_Capsule(largeEnemyModel, collision, AppFrame::Math::ToDX(plyCapsulePos1), AppFrame::Math::ToDX(plyCapsulePos2), playerCapsuleRadius);
		//�����蔻��̌��ʂ��m�F
		if (result.HitNum > 0) {
			//�������Ă�����v���C���[�̓����蔻�茋�ʂ����[�W�G�l�~�[�Ɠ��������Ɛݒ�
			objectBase.collisionComponent().report().id(ReportId::HitFromLargeEnemy);
			//���������ʒu�Ƀ��[�W�G�l�~�[�̈ʒu��ݒ�
			objectBase.collisionComponent().hitPos(_owner.position());
			//�_���[�W�̐ݒ�
			objectBase.collisionComponent().damage(20.0);
		}
	}
}

void CollisionComponent::BulletFromPoorEnemyGatling() {
	//�K�g�����O�U�������Ă���G���G�̃��f���n���h���̎擾
	auto poorEnemyGatlingModel = _owner.modelAnimeComponent().modelHandle();
	//���f���̃R���W�����t���[���̎擾
	auto collision = _owner.modelAnimeComponent().FindFrame("Spider");
	//�I�u�W�F�N�g�T�[�o�[�̊e�I�u�W�F�N�g���擾
	for (auto&& object : _owner.GetObjServer().runObjects()) {
		//�I�u�W�F�N�g�x�[�X�^�ɃL���X�g
		auto& objectBase = ObjectBaseCast(*object);
		//���u��U���̒e����Ȃ�������Ȃɂ����Ȃ�
		if (objectBase.GetObjType() != Object::ObjectBase::ObjectType::Bullet) {
			continue;
		}
		//���u��U���̒e�̋��̍쐬
		//���̈ʒu
		auto bullet = objectBase.position();
		//���̔��a
		auto bulletRadius = static_cast<float>(20.0);
		//���f���Ƌ��̓����蔻������
		auto result = MV1CollCheck_Sphere(poorEnemyGatlingModel, collision, AppFrame::Math::ToDX(bullet), bulletRadius);
		//�����蔻��̌��ʂ��m�F
		if (result.HitNum > 0) {
			//�������Ă����牓�u��U���̒e�̓����蔻�茋�ʂ��K�g�����O�U�������Ă���G���G�Ɠ��������Ɛݒ�
			objectBase.collisionComponent().report().id(ReportId::HitFromPoorEnemyGatling);
			//�_���[�W�̐ݒ�
			_owner.collisionComponent().damage(10.0);
			//�K�g�����O�U�������Ă���G���G�̓����蔻�茋�ʂ����u��U���̒e�Ɠ��������Ɛݒ�
			_owner.collisionComponent().report().id(ReportId::HitFromBullet);
		}
	}
}

void CollisionComponent::PoorEnemyGatlingFromObjectModel() {
	//��������I�u�W�F�N�g�̃J�v�Z���̍쐬
	auto fallObjectPos = _owner.position();
	//�J�v�Z���̈�ڂ̈ʒu
	auto capsulePos1 = Vector4(0.0, FallObjectCapsulePos1, 0.0) + fallObjectPos;
	//�J�v�Z���̓�ڂ̈ʒu
	auto capsulePos2 = Vector4(0.0, FallObjectCapsulePos2, 0.0) + fallObjectPos;
	//�J�v�Z���̔��a
	auto capsuleRadian = static_cast<float>(FallObjectRadius);
	//�I�u�W�F�N�g�T�[�o�[�̊e�I�u�W�F�N�g���擾
	for (auto&& object : _owner.GetObjServer().runObjects()) {
		//�I�u�W�F�N�g�x�[�X�^�ɃL���X�g
		auto& objectBase = ObjectBaseCast(*object);
		//�K�g�����O�U�������Ă���G���G����Ȃ���Ή������Ȃ�
		if (objectBase.GetObjType() != Object::ObjectBase::ObjectType::PoorEnemyGatling) {
			continue;
		}
		//�K�g�����O�U�������Ă���G���G�̃��f���n���h�����擾
		auto poorEnemyGatlingModel = objectBase.modelAnimeComponent().modelHandle();
		//���f���̃R���W�����t���[���ԍ����擾
		auto collision = objectBase.modelAnimeComponent().FindFrame("Spider");
		//���f���ƃJ�v�Z���̓����蔻������
		auto result = MV1CollCheck_Capsule(poorEnemyGatlingModel, collision, AppFrame::Math::ToDX(capsulePos1), AppFrame::Math::ToDX(capsulePos2), capsuleRadian);
		//�����蔻��̌��ʂ��m�F
		if (result.HitNum > 0) {
			//�K�g�����O�U�������Ă���G���G�̓����蔻�茋�ʂ𗎉�����I�u�W�F�N�g�Ɠ��������Ɛݒ�
			objectBase.collisionComponent().report().id(ReportId::HitFromFallObject);
			//�_���[�W�̐ݒ�
			objectBase.collisionComponent().damage(20.0);
			//��������I�u�W�F�N�g�̓����蔻�茋�ʂ��K�g�����O�U�������Ă���G���G�Ɠ��������Ɛݒ�
			_owner.collisionComponent().report().id(ReportId::HitFromPoorEnemyGatling);
		}
	}
}

void CollisionComponent::PlayerKnockBack() {
	// �I�u�W�F�N�g�T�[�o�[�̊e�I�u�W�F�N�g���擾
	for (auto&& object : _owner.GetObjServer().runObjects()) {
		// �I�u�W�F�N�g�x�[�X�^�ɃL���X�g
		auto& objectBase = ObjectBaseCast(*object);
		// �v���C���[����Ȃ������牽�����Ȃ�
		if (objectBase.GetObjType() != Object::ObjectBase::ObjectType::Player) {
			continue;
		}
		// �v���C���[���m�b�N�o�b�N���Ă��邩
		if (objectBase.collisionComponent().knockBack()) {
			// �m�b�N�o�b�N���Ă�����v���C���[���m�b�N�o�b�N���Ă���Ɛݒ�
			_owner.collisionComponent().knockBack(true);
		}
	}
}

AppFrame::Math::Vector4 CollisionComponent::PlayerCheckStage(const Vector4& pos, const Vector4& moved) {
	
	auto modeBase = _owner.gameMain().modeServer().GetMode("Poor");
	auto modeBoss = std::dynamic_pointer_cast<Mode::ModePoor>(modeBase);
	auto stageComponent = modeBoss->GetStage().stageComponent();
    
	auto [handle, collision] = stageComponent.GetHandleAndCollNum("stage_character_c");

	auto newPos = pos + moved;
	auto start = newPos + Vector4(0.0, 50.0, 0.0);
	auto end = newPos + Vector4(0.0, -10000.0, 0.0);
	auto result = MV1CollCheck_Line(handle, collision, AppFrame::Math::ToDX(start), AppFrame::Math::ToDX(end));
	if (result.HitFlag != 0) {
		newPos = AppFrame::Math::ToMath(result.HitPosition);
		return newPos;
	}
	else {
		return pos;
	}
}

AppFrame::Math::Vector4 CollisionComponent::LargeEnemyCheckStage(const Vector4& pos, const Vector4& moved) {

	auto modeBase = _owner.gameMain().modeServer().GetMode("Boss");
	auto modeBoss = std::dynamic_pointer_cast<Mode::ModeBoss>(modeBase);
	auto stageComponent = modeBoss->GetStage().stageComponent();

	auto [handle, collision] = stageComponent.GetHandleAndCollNum("stage_boss_c");

	auto newPos = pos + moved;
	auto start = newPos + Vector4(0.0, 50.0, 0.0);
	auto end = newPos + Vector4(0.0, -10000.0, 0.0);
	auto result = MV1CollCheck_Line(handle, collision, AppFrame::Math::ToDX(start), AppFrame::Math::ToDX(end));
	if (result.HitFlag != 0) {
		newPos = AppFrame::Math::ToMath(result.HitPosition);
		return newPos;
	}
	else {
		return pos;
	}
}

void CollisionComponent::OutStage() {
	auto modeBase = _owner.gameMain().modeServer().GetMode("Boss");
	auto modeBoss = std::dynamic_pointer_cast<Mode::ModeBoss>(modeBase);
	auto stageComponent = modeBoss->GetStage().stageComponent();

	auto [handle, collision] = stageComponent.GetHandleAndCollNum("stage_object_c");
	// �ʒu�̎擾
	auto position = _owner.position();
	// �����̍쐬
	// �n�_
	auto start = position + Vector4(0.0, 50.0, 0.0);
	// �I�_
	auto end = position + Vector4(0.0, -10000.0, 0.0);

	auto result = MV1CollCheck_Line(handle, collision, AppFrame::Math::ToDX(start), AppFrame::Math::ToDX(end));

	if (result.HitFlag == 0) {
		_owner.collisionComponent().report().id(ReportId::OutStage);
		return;
	}
	
	return;
}

FragmentValkyria::Object::ObjectBase& CollisionComponent::ObjectBaseCast(AppFrame::Object::ObjectBaseRoot& obj) {
#ifndef _DEBUG
	auto& objectBase = dynamic_cast<Object::ObjectBase&>(obj);

	return objectBase;
	
#else
	try {
		auto& objectBase = dynamic_cast<Object::ObjectBase&>(obj);

		return objectBase;
	}
	catch (std::bad_cast&) {
		OutputDebugString("----------�_�E���L���X�g���s----------");
	}
#endif

	return dynamic_cast<Object::ObjectBase&>(obj);
}
