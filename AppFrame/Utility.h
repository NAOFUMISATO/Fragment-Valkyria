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
      static constexpr double DEGREES_180 = 180.0;    //!< �x���@180��
      static constexpr double PI = std::numbers::pi;  //!< �~����
      static constexpr double RADIAN_1 = std::numbers::pi / DEGREES_180;

      class Vector4;

      using Plane = std::tuple<Vector4/*point*/, Vector4/*normal*/>;
      using Sphere = std::tuple<Vector4/*position*/, double/*radian*/>;
      using Line = std::tuple<Vector4/*�ʒu*/, Vector4/*����*/>;
      using Segment = std::tuple<Vector4/*�n�_*/, Vector4/*�I�_�ւ̃x�N�g��*/>;
      using Capsule = std::tuple<Vector4/*�����̎n�_*/, Vector4/*�����̏I�_*/, double/*���a*/>;
      using Collision = std::tuple<bool, Vector4>;

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

         // ��`�Ɛ����̓����蔻��
         static Collision CollisionPolygonLine(const Vector4& polygonPoint0, const Vector4& polygonPoint1,
            const Vector4& polygonPoint2, const Vector4& polygonPoint3,
            const Vector4& lineStart, const Vector4& lineEnd);

         // �ʂ̖@�������߂�
         static Vector4 GetNormal(const Vector4& point0, const Vector4& point1, const Vector4& point2);

         // �O�p�`�Ɠ_�̓��O����(point �͕K���O�p�`�Ɠ����ʂł��鎖)
         static bool InsideTrianglePoint(const Vector4& trianglePoint0, const Vector4& trianglePoint1,
            const Vector4& trianglePoint2, const Vector4& point);
         // ���ʂƐ����̕ӂ蔻��
         static bool CollisionPlaneLine(const Plane& p, const Vector4& lineStart, const Vector4& lineEnd, Collision& result);
         //���Ɠ_�̓����蔻��
         static bool CollisionSpherePoint(const Vector4& point, const Sphere& s);
         /**
          * \brief �J�v�Z���Ƌ��̓����蔻��
          * \param c
          * \param s
          * \return �������Ă��邩
          */
         static bool CollisionCapsuleSphere(const Capsule& c, const Sphere& s);
         /**
          * \brief �J�v�Z���ƃJ�v�Z���̓����蔻��
          * \param c1
          * \param c2
          * \return �������Ă��邩
          */
         static bool CollisionCapsuleCapsule(const Capsule& c1, const Capsule& c2);
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
          * \brief 
          * \param l1 
          * \param l2 
          * \param p1 
          * \param p2 
          * \param t1 
          * \param t2 
          * \return 
          */
         static double LineLineDistance(const Line& l1, const Line& l2, Vector4& p1, Vector4& p2, double& t1, double& t2);
         /**
          * \brief 
          * \param s1 
          * \param s2 
          * \param p1 
          * \param p2 
          * \param t1 
          * \param t2 
          * \return 
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

      private:
         Utility() = default;
      };
   }
}