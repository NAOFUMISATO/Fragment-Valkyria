#pragma once
/*****************************************************************//**
 * \file   Vector4.h
 * \brief  4�����x�N�g���N���X
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include <tuple>
 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   /**
    * \brief �Z�p�֌W
    */
   namespace Math {
      class Matrix44;

      class Vector4 {
      public:
         // �R���X�g���N�^
         Vector4();

         Vector4(const double x, const double y, const double z) : _x(x), _y(y), _z(z), _w(1.0) {}
         Vector4(const double x, const double y, const double z, double w) : _x(x), _y(y), _z(z), _w(w) {}
         Vector4(const Vector4&) = default; // �R�s�[
         Vector4(Vector4&&) = default; // ���[�u

         // �f�X�g���N�^
         virtual ~Vector4() = default;

         Vector4& operator =(const Vector4&) = default; // �R�s�[
         Vector4& operator =(Vector4&&) = default; // ���[�u

         const Vector4 operator +(const Vector4& rhs) const;
         const Vector4 operator -(const Vector4& rhs) const;
         const Vector4 operator *(const Vector4& rhs) const;

         const Vector4 operator *(const double rhs) const;
         const Vector4 operator /(const double rhs) const;

         const Vector4 operator *(const Matrix44 rhs) const;

         void Set(const Vector4& vector);
         void Set(const double x, const double y, const double z);

         void Add(const Vector4& vector);
         void Add(const double x, const double y, const double z);

         const std::tuple<double, double, double> GetXYZ() const { return { _x, _y, _z }; }
         const double GetY() const { return _y; }
         const double GetW() const { return _w; }
         void SetY(double y) { _y = y; }

         const double Lenght() const;

         void Normalized();
         const Vector4 Normalize() const;

         const double Dot(const Vector4& rhs) const;
         const Vector4 Cross(const Vector4& rhs) const;

      private:
         double _x;
         double _y;
         double _z;
         double _w;
      };
   }
}
