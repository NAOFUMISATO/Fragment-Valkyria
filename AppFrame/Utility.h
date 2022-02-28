#pragma once
/*****************************************************************//**
 * \file   Utility.h
 * \brief  �L�p�Z�p�N���X
 *
 * \author NAOFUMISATO, AHMD2000
 * \date   December 2021
 *********************************************************************/
#include <tuple>
#include <numbers>
#include <cmath>
 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   /**
    * \brief �Z�p�֌W
    */
   namespace Math {
      static constexpr double DEGREES_180 = 180.0;                                                //!< �x���@180��
      static constexpr double PI = std::numbers::pi;                                              //!< �~����
      static constexpr double RADIAN_1 = std::numbers::pi / DEGREES_180;

      class Vector4;

      using Plane = std::tuple<Vector4/*�ʒu*/, Vector4/*�@��*/>;                                 //!< ���ʂ̒�`
      using Sphere = std::tuple<Vector4/*�ʒu*/, double/*���a*/>;                                 //!< ���̒�`
      using Line = std::tuple<Vector4/*�ʒu*/, Vector4/*����*/>;                                  //!< �����̒�`
      using Segment = std::tuple<Vector4/*�n�_*/, Vector4/*�I�_�ւ̃x�N�g��*/>;                   //!< �����̒�`
      using Capsule = std::tuple<Vector4/*�����̎n�_*/, Vector4/*�����̏I�_*/, double/*���a*/>;   //!< �J�v�Z���̒�`
      using Collision = std::tuple</*�����蔻�茋��*/bool, /*��_�̈ʒu*/Vector4>;                //!< �����蔻��̌��ʗp�̃^�v���^�̒�`
      /**
      * \class �L�p�Z�p�N���X
      * \brief �S�ĐÓI�����o�ō\��
      */
      class Utility {
      public:
         /**
          * \brief �ʓx�@����x���@�ւ̕ϊ�
          * \param degree �ʓx�@�ł̒l
          * \return �x���@�ł̒l
          */
         static double DegreeToRadian(double degree) {
            return degree * PI / DEGREES_180;
         }
         /**
          * \brief �x��������ʓx�@�ւ̕ϊ�
          * \param radion �x���@�ł̒l
          * \return �ʓx�@�ł̒l
          */
         static double RadianToDegree(double radion) {
            return radion * DEGREES_180 / PI;
         }
         /**
          * \brief ��`�Ɛ����̓����蔻��
          * \param polygonPoint0 ��`���_1
          * \param polygonPoint1 ��`���_2
          * \param polygonPoint2 ��`���_3
          * \param polygonPoint3 ��`���_4
          * \param lineStart �����n�_
          * \param lineEnd �����I�_
          * \return �����蔻��y�тǂ�������������
          */
         static Collision CollisionPolygonLine(const Vector4& polygonPoint0, const Vector4& polygonPoint1,
            const Vector4& polygonPoint2, const Vector4& polygonPoint3,
            const Vector4& lineStart, const Vector4& lineEnd);
         /**
          * \brief �ʖ@���̌v�Z
          * \param point0 �O�p�`���_1
          * \param point1 �O�p�`���_2
          * \param point2 �O�p�`���_3
          * \return �ʖ@���x�N�g��
          */
         static Vector4 GetNormal(const Vector4& point0, const Vector4& point1, const Vector4& point2);
         /**
          * \brief �O�p�`�̓��O����
          * \param trianglePoint0 �O�p�`���_1
          * \param trianglePoint1 �O�p�`���_2
          * \param trianglePoint2 �O�p�`���_3
          * \param point ���肷��_
          * \return ���Ȃ��true,�O�Ȃ��false��Ԃ�
          */
         static bool InsideTrianglePoint(const Vector4& trianglePoint0, const Vector4& trianglePoint1,
            const Vector4& trianglePoint2, const Vector4& point);
         /**
          * \brief ���ʂƐ����̓����蔻��
          * \param p ���ʈʒu
          * \param lineStart �����n�_
          * \param lineEnd �����I�_
          * \param result �����蔻��y�тǂ�������������
          * \return �����蔻�茋��
          */
         static bool CollisionPlaneLine(const Plane& p, const Vector4& lineStart, const Vector4& lineEnd, Collision& result);
         /**
          * \brief ���Ɠ_�̓����蔻��
          * \param point �_
          * \param sphere ��
          * \return �����蔻�茋��
          */
         static bool CollisionSpherePoint(const Vector4& point, const Sphere& sphere);
         /**
          * \brief �J�v�Z���Ƌ��̓����蔻��
          * \param capsule �J�v�Z��
          * \param sphere ��
          * \return �����蔻�茋��
          */
         static bool CollisionCapsuleSphere(const Capsule& capsule, const Sphere& sphere);
         /**
          * \brief �J�v�Z���ƃJ�v�Z���̓����蔻��
          * \param capsule1 �J�v�Z��1
          * \param capsule2 �J�v�Z��2
          * \return �����蔻�茋��
          */
         static bool CollisionCapsuleCapsule(const Capsule& capsule1, const Capsule& capsule2);
         /**
          * \brief �_�ƒ����̍ŒZ���������Ƃ߂�
          * \param point �_�̈ʒu�x�N�g��
          * \param line ����
          * \param h �_���璼���֍~�낵�������ƒ����Ƃ̌�_(�߂�l)
          * \param t �_���璼���֍~�낵�������ƒ����Ƃ̌�_�ւ̃x�N�g���W��(�߂�l)
          * \return �_�ƒ����̍ŒZ����
          */
         static double PointLineDistance(const Vector4& point, const Line& line, Vector4& h, double& t);
         /**
          * \brief p2����p1��p3�ɐL�΂����x�N�g���łł����p�x���s�p��
          * \param p1 1�߂̏I�_
          * \param p2 �n�_
          * \param p3 2�߂̏I�_
          * \return �s�p���ǂ���
          */
         static bool IsSharpAngle(const Vector4& p1, const Vector4& p2, const Vector4& p3);
         /**
          * \brief �_�Ɛ����̍ŒZ���������Ƃ߂�
          * \param point �_�̈ʒu�x�N�g��
          * \param segment ����
          * \param h �_��������֍~�낵�������Ɛ����Ƃ̌�_(�߂�l)
          * \param t �_��������֍~�낵�������Ɛ����Ƃ̌�_�ւ̃x�N�g���W��(�߂�l)
          * \return �_�Ɛ����̍ŒZ����
          */
         static double PointSegmentDistance(const Vector4& point, const Segment& segment, Vector4& h, double& t);
         /**
          * \brief �����ƒ����̍ŒZ���������߂�
          * \param l1 ��ڂ̒���
          * \param l2 ��ڂ̒���
          * \param p1 ��ڂ̒��������ڂ̒����̍ŒZ�����ւ̈�ڂ̒����̓_�̈ʒu
          * \param p2 ��ڂ̒��������ڂ̒����̍ŒZ�����ւ̓�ڂ̒����̓_�̈ʒu
          * \param t1 ��ڂ̒��������ڂ̒����̍ŒZ�����ւ̈�ڂ̒����̓_�ւ̊���
          * \param t2 ��ڂ̒��������ڂ̒����̍ŒZ�����ւ̓�ڂ̒����̓_�ւ̊���
          * \return �����ƒ����̍ŒZ����
          */
         static double LineLineDistance(const Line& l1, const Line& l2, Vector4& p1, Vector4& p2, double& t1, double& t2);
         /**
          * \brief �����Ɛ����̍ŒZ���������߂�
          * \param s1 ��ڂ̐���
          * \param s2 ��ڂ̐���
          * \param p1 ��ڂ̐��������ڂ̐����̍ŒZ�����ւ̈�ڂ̐����̓_�̈ʒu
          * \param p2 ��ڂ̐��������ڂ̐����̍ŒZ�����ւ̓�ڂ̐����̓_�̈ʒu
          * \param t1 ��ڂ̐��������ڂ̐����̍ŒZ�����ւ̈�ڂ̐����̓_�ւ̊���
          * \param t2 ��ڂ̐��������ڂ̐����̍ŒZ�����ւ̓�ڂ̐����̓_�ւ̊���
          * \return �����Ɛ����̍ŒZ����
          */
         static double SegmentSegmentDistance(const Segment& s1, const Segment& s2, Vector4& p1, Vector4& p2, double& t1, double& t2);
        /**
         * \brief �w�肵��RGB�̃J���[�R�[�h�̎擾
         * \param r �Ԓl
         * \param g �Βl
         * \param b �l
         * \return �J���[�R�[�h
         */
         static unsigned int GetColorCode(unsigned char red, unsigned char green, unsigned char blue);
         /**
          * \brief ��l���z�ł̗����̎擾(int�^)
          * \param min �ŏ��l
          * \param max �ő�l
          * \return �ŏ��l�ƍő�l�͈̔͂ł̈�l���z�ł̗���
          */
         static int GetRandom(const int min, const int max);
         /**
          * \brief ��l���z�ł̗����̎擾(double�^)
          * \param min �ŏ��l
          * \param max �ő�l
          * \return �ŏ��l�ƍő�l�͈̔͂ł̈�l���z�ł̗���
          */
         static double GetRandom(const double min, const double max);
         /**
          * \brief ��l���z�ł̗����̎擾(float�^)
          * \param min �ŏ��l
          * \param max �ő�l
          * \return �ŏ��l�ƍő�l�͈̔͂ł̈�l���z�ł̗���
          */
         static float GetRandom(const float min, const float max);
         /**
          * \brief ��l���z�ł̗����̎擾(Vector4�^)
          * \param min �ŏ��l
          * \param max �ő�l
          * \return �ŏ��l�ƍő�l�͈̔͂ł̈�l���z�ł̗���
          */
         static Vector4 GetRandom(const Vector4 min, const Vector4 max);

      private:
         /**
          * \brief �f�t�H���g�f�X�g���N�^
          */
         Utility() = default;
      };
   }
}