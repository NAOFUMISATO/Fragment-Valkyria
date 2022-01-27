
/*****************************************************************//**
 * \file   Vector4.cpp
 * \brief  4次元ベクトル
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "Vector4.h"
#include <cmath>
#include "Matrix44.h"
 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   /**
    * \brief 算術関係
    */
   namespace Math {

      Vector4::Vector4() {
         _x = 0.0; _y = 0.0; _z = 0.0; _w = 1.0;
      }

      void Vector4::Set(const Vector4& vector) {
         auto [x, y, z] = vector.GetVec3();
         auto w = vector.GetW();
         _x = x;
         _y = y;
         _z = z;
         _w = w;
      }

      void Vector4::Set(const double x, const double y, const double z) {
         this->_x = x;
         this->_y = y;
         this->_z = z;
      }

      void Vector4::Add(const Vector4& vector) {
         auto [x, y, z] = vector.GetVec3();
         _x += x;
         _y += y;
         _z += z;
      }

      void Vector4::Add(const double x, const double y, const double z) {
         this->_x += x;
         this->_y += y;
         this->_z += z;
      }

      const double Vector4::Lenght() const {
         return sqrt(_x * _x + _y * _y + _z * _z);
      }

      void Vector4::Normalized() {
         auto len = Lenght();

         _x /= len;
         _y /= len;
         _z /= len;
      }

      const Vector4 Vector4::Normalize() const {
         auto len = Lenght();
         auto nx = _x / len;
         auto ny = _y / len;
         auto nz = _z / len;
         return Vector4(nx, ny, nz);
      }

      const double Vector4::Dot(const Vector4& rhs) const {
         auto [x, y, z] = rhs.GetVec3();
         return _x * x + _y * y + _z * z;
      }

      const Vector4 Vector4::Cross(const Vector4& rhs) const {
         auto [x, y, z] = rhs.GetVec3();
         auto cx = _y * z - _z * y;
         auto cy = _z * x - _x * z;
         auto cz = _x * y - _y * x;
         return Vector4(cx, cy, cz);
      }

      const Vector4 Vector4::operator +(const Vector4& rhs) const {
         auto [x, y, z] = rhs.GetVec3();
         return Vector4(_x + x, _y + y, _z + z);
      }

      const Vector4 Vector4::operator -(const Vector4& rhs) const {
         auto [x, y, z] = rhs.GetVec3();
         return Vector4(_x - x, _y - y, _z - z);
      }

      const Vector4 Vector4::operator *(const Vector4& rhs) const {
         auto [x, y, z] = rhs.GetVec3();
         return Vector4(_x * x, _y * y, _z * z);
      }

      const Vector4 Vector4::operator *(const double rhs) const {
         return Vector4(_x * rhs, _y * rhs, _z * rhs);
      }

      const Vector4 Vector4::operator /(const double rhs) const {
         return Vector4(_x / rhs, _y / rhs, _z / rhs);
      }

      const Vector4 Vector4::operator *(const Matrix44 rhs) const {
         auto mx = _x * rhs.GetValue(0, 0)
            + _y * rhs.GetValue(1, 0)
            + _z * rhs.GetValue(2, 0)
            + rhs.GetValue(3, 0);

         auto my = _x * rhs.GetValue(0, 1)
            + _y * rhs.GetValue(1, 1)
            + _z * rhs.GetValue(2, 1)
            + rhs.GetValue(3, 1);

         auto mz = _x * rhs.GetValue(0, 2)
            + _y * rhs.GetValue(1, 2)
            + _z * rhs.GetValue(2, 2)
            + rhs.GetValue(3, 2);

         auto mw = _x * rhs.GetValue(0, 3)
            + _y * rhs.GetValue(1, 3)
            + _z * rhs.GetValue(2, 3)
            + rhs.GetValue(3, 3);

         return Vector4(mx, my, mz, mw);
      }
   }
}
