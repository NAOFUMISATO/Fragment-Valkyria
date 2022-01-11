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
      static constexpr double DEGREES_180 = 180.0;    //!< �x���@180��
      static constexpr double PI = std::numbers::pi;  //!< �~����

      class Vector4;

      using plane = std::tuple<Vector4/*point*/, Vector4/*normal*/>;
      using collision = std::tuple<bool, Vector4>;


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
         * \brief �w�肵��RGB�̃J���[�R�[�h�̎擾
         * \param r �Ԓl
         * \param g �Βl
         * \param b �l
         * \return �J���[�R�[�h
         static bool CollisionPlaneLine(const plane& p, const Vector4& lineStart, const Vector4& lineEnd, collision& result);
         static bool CollisionSpherePoint(const Vector4& point, const Sphere& s);

      private:
         Utility() = default;
      };
   }
}