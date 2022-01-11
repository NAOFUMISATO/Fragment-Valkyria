#pragma once
/*****************************************************************//**
 * \file   Utility.h
 * \brief  �L�p�Z�p�N���X
 *
 * \author NAOFUMISATO
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
      static constexpr double DEGREES_180 = 180.0;
      static constexpr double PI = std::numbers::pi;
      class Vector4;

      using Plane = std::tuple<Vector4/*point*/, Vector4/*normal*/>;
      using Sphere = std::tuple<Vector4 /*position*/, double/*radian*/>;
      using Collision = std::tuple<bool, Vector4>;

      class Utility {
      public:
         static double DegreeToRadian(double degree) {
            return degree * PI / DEGREES_180;
         }

         static double RadianToDegree(double radion) {
            return radion * DEGREES_180 / PI;
         }

         static int GetRandom(const int min, const int max);
         static double GetRandom(const double min, const double max);
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

      private:
         Utility() = default;
      };
   }
}