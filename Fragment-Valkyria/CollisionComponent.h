#pragma once
/*****************************************************************//**
 * \file   CollisionServer.h
 * \brief  �����蔻�菈�����s���N���X
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "AppFrame.h"
#include "ParamCollision.h"
/**
 * \brief �v���W�F�N�g��
 */
namespace FragmentValkyria {
   //��d�C���N���[�h�h�~
   namespace Object {
      class ObjectBase;
   }
   /**
    * \brief �����蔻��֌W
    */
   namespace Collision {
      /**
       * \class �����蔻�菈�����s���N���X
       * \brief �����蔻�菈�����s���N���X���Ǘ�����
       */
      class CollisionComponent {
         using Vector4 = AppFrame::Math::Vector4;
      public:
         /**
          * \brief �����蔻�茋�ʂ̗�
          */
         enum class ReportId {
            None,                      //!< �������Ă��Ȃ�
            HitFromPlayer,             //!< �v���C���[�Ɠ�������
            HitFromObjectRange,        //!< �I�u�W�F�N�g�������グ����͈͂ɂ���
            HitFromIdleFallObject,     //!< �ҋ@��Ԃ̗�������I�u�W�F�N�g�Ɠ�������
            HitFromFallObject,         //!< �������Ă��闎������I�u�W�F�N�g�Ɠ�������
            HitFromGatling,            //!< �K�g�����O�Ɠ�������
            HitFromLargeEnemy,         //!< �{�X�Ɠ�������
            HitFromPoorEnemy,          //!< �G���G�Ɠ�������
            HitFromBullet,             //!< ���u��U���̒e�Ɠ�������
            HitFromLaser,              //!< ���[�U�[�Ɠ�������
            OutStage                   //!< �X�e�[�W�̊O�ɂ���
         };
         /**
          * \brief �����蔻�茋�ʂ��Ǘ�����N���X
          */
         class Report {
         public:
            /**
             * \brief �����蔻�茋�ʂ̐ݒ�
             * \param id �����蔻�茋��
             */
            void id(ReportId id) { _id = id; }
            /**
             * \brief �����蔻�茋�ʂ̎擾
             * \return �����蔻�茋��
             */
            ReportId& id() { return _id; }

         private:
            ReportId _id{ ReportId::None };  //!< �����蔻��̌���
         };
         /**
          * \brief �R���X�g���N�^
          * \param owner �I�u�W�F�N�g�̎Q��
          */
         CollisionComponent(Object::ObjectBase& owner);
         /**
          * \brief �I�u�W�F�N�g�������グ����͈͂Ƀv���C���[�����邩�m�F
          */
         void PlayerFromObjectRange();
         /**
          * \brief ��������I�u�W�F�N�g�̃��f���Ɠ������Ă��邩����
          * \param fall ��������
          */
         void PlayerFromFallObjectModel(bool fall);
         /**
          * \brief �I�u�W�F�N�g�̃��f���ɃK�g�����O���������Ă��邩�m�F
          */
         void GatlingFromObjectModel();
         /**
          * \brief �v���C���[���K�g�����O�ɓ������Ă��邩�m�F
          */
         void GatlingFromPlayer();
         /**
          * \brief ���[�W�G�l�~�[�̃��f���ɗ�������I�u�W�F�N�g�������������m�F
          */
         void LargeEnemyFromObjectModel();
         /**
          * \brief ���u��U���̒e�����[�W�G�l�~�[�ɓ����������m�F
          */
         void LargeEnemyFromBullet();
         /**
          * \brief ���[�U�[����������I�u�W�F�N�g�ɓ������Ă��邩�m�F
          */
         void FallObjectFromLaser();
         /**
          * \brief ���[�U�[���v���C���[�Ɠ������Ă��邩�m�F
          */
         void PlayerFromLaser();
         /**
          * \brief �v���C���[�����[�W�G�l�~�[�̃��f���Ɠ������Ă��邩�m�F
          */
         void LargeEnemyFromPlayer();
         /**
          * \brief �v���C���[���G���G�̃��f���Ɠ������Ă��邩�m�F
          */
         void PoorEnemyFromPlayer();
         /**
          * \brief �K�g�����O�U�������Ă���G���G�����u��U���̒e�Ɠ������Ă��邩�m�F
          */
         void BulletFromPoorEnemy();
         /**
          * \brief ��������I�u�W�F�N�g���K�g�����O�U�������Ă���G���G�Ɠ����������m�F
          */
         void PoorEnemyGatlingFromObjectModel();
         /**
          * \brief �v���C���[���m�b�N�o�b�N���Ă��邩�m�F
          */
         void PlayerKnockBack();
         /**
          * \brief �v���C���[�ƃX�e�[�W���������Ă��邩�m�F
          * \param pos �v���C���[�̈ʒu
          * \param moved �v���C���[�̈ړ��ʂ̃x�N�g��
          * \return �ړ���̈ʒu
          */
         Vector4 PlayerCheckStage(const Vector4& pos, const Vector4& moved);
         /**
          * \brief ���[�W�G�l�~�[�ƃX�e�[�W���������Ă��邩�m�F
          * \param pos ���[�W�G�l�~�[�̈ʒu
          * \param moved ���[�W�G�l�~�[�̈ړ��ʂ̃x�N�g��
          * \return �ړ���̈ʒu
          */
         Vector4 LargeEnemyCheckStage(const Vector4& pos, const Vector4& moved);
         /**
          * \brief �v�A�G�l�~�[�ƃX�e�[�W���������Ă��邩�m�F
          * \param pos �v�A�G�l�~�[�̈ʒu
          * \param moved �v�A�G�l�~�[�̈ړ��ʂ̃x�N�g��
          * \return �ړ���̈ʒu
          */
         Vector4 PoorEnemyCheckStage(const Vector4& pos, const Vector4& moved);
         /**
          * \brief �w��ʒu�̐����ƃX�e�[�W���������Ă��邩�̊m�F
          * \param pos �������o��������ʒu(Y���͍l�����Ȃ�)
          * \return �w��ʒu�̐����ƃX�e�[�W���������Ă��邩
          */
         bool IsLineFromStage(const Vector4& pos);
         /**
          * \brief �X�e�[�W�O�ɂ��邩�m�F
          */
         void OutStage();
         /**
          * \brief �����蔻�茋�ʂ��Ǘ�����|�C���^�̐ݒ�
          * \param report �����蔻�茋�ʂ��Ǘ�����N���X
          */
         void report(Report report) { *_report = report; }
         /**
          * \brief �����蔻�茋�ʂ��Ǘ�����N���X�̎Q�Ƃ̎擾
          * \return �����蔻�茋�ʂ��Ǘ�����N���X�̎Q��
          */
         Report& report() { return *_report; }
         /**
          * \brief ���������ʒu�̐ݒ�
          * \param pos ���������ʒu
          */
         void hitPos(Vector4 pos) { _hitPos = pos; }
         /**
          * \brief ���������ʒu�̎擾
          * \return ���������ʒu
          */
         Vector4 hitPos() { return _hitPos; }
         /**
          * \brief �_���[�W�ʂ̐ݒ�
          * \param damage �_���[�W��
          */
         void damage(double damage) { _damage = damage; }
         /**
          * \brief �_���[�W�ʂ̎擾
          * \return �_���[�W��
          */
         double damage() { return _damage; }
         /**
          * \brief �m�b�N�o�b�N���Ă��邩�ݒ�
          * \param knockBack �m�b�N�o�b�N���Ă��邩
          */
         void knockBack(bool knockBack) { _knockBack = knockBack; }
         /**
          * \brief �m�b�N�o�b�N���Ă��邩�̎擾
          * \return �m�b�N�o�b�N���Ă��邩
          */
         bool knockBack() { return _knockBack; }

      private:

         Object::ObjectBase& _owner;                     //!< �I�u�W�F�N�g�̎Q��
         std::unique_ptr<Param::ParamCollision> _param;  //!< �����蔻�菈�����s���N���X�̒l�Ǘ��N���X
         std::unique_ptr<Report> _report;                //!< �����蔻�茋�ʂ��Ǘ�����|�C���^
         double _damage{ 0.0 };                          //!< �_���[�W��
         bool _knockBack{ false };                       //!< �v���C���[���m�b�N�o�b�N���Ă��邩
         Vector4 _hitPos;                                //!< ���������ʒu

      };

   }
}
