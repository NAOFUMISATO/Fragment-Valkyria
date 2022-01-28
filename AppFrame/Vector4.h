#pragma once
/*****************************************************************//**
 * \file   Vector4.h
 * \brief  4次元ベクトルクラス
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include <tuple>
 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   /**
    * \brief 算術関係
    */
   namespace Math {
      class Matrix44;

      class Vector4 {
      public:
         // コンストラクタ
         Vector4();

         Vector4(const double x, const double y, const double z) : _x(x), _y(y), _z(z), _w(1.0) {}
         Vector4(const double x, const double y, const double z, double w) : _x(x), _y(y), _z(z), _w(w) {}
         Vector4(const Vector4&) = default; // コピー
         Vector4(Vector4&&) = default; // ムーブ

         // デストラクタ
         virtual ~Vector4() = default;

         Vector4& operator =(const Vector4&) = default; // コピー
         Vector4& operator =(Vector4&&) = default; // ムーブ

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

         inline const std::tuple<double, double, double> GetVec3() const { return { _x, _y, _z }; }
         inline void SetVec3(std::tuple<double, double, double> xyz) {
            auto [x, y, z] = xyz;
            _x=x,_y=y,_z=z; 
         }
         inline const std::pair<double, double> GetVec2() const { return { _x,_y }; }
         inline void SetVec2(double x, double y) { _x = x, _y = y; }
         inline const double GetY() const { return _y; }
         inline const double GetW() const { return _w; }
         inline void SetY(double y) { _y = y; }

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
