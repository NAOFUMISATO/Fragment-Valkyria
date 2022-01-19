#pragma once
/*****************************************************************//**
 * \file   Matrix44.h
 * \brief  4次元行列クラス
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include <cstdint>
#include <array>
#include <tuple>
 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   /**
    * \brief 算術関係
    */
   namespace Math {
      class Vector4;

      constexpr uint32_t _rowMax = 4;
      constexpr uint32_t _columnMax = 4;

      using MatrixArray = std::array<std::array<double, _columnMax>, _rowMax>;

      class Matrix44 {
      public:
         // コンストラクタ
         Matrix44();
         Matrix44(const MatrixArray values);
         Matrix44(const Matrix44&) = default; // コピー
         Matrix44(Matrix44&&) = default; // ムーブ

         // デストラクタ
         virtual ~Matrix44() = default;

         Matrix44& operator =(const Matrix44&) = default; // コピー
         Matrix44& operator =(Matrix44&&) = default; // ムーブ

         const Matrix44 operator *(const Matrix44 rhs) const;
         const Matrix44 operator +(const Vector4 rhs) const;

         const double GetValue(const uint32_t row, const uint32_t column) const { return _rowColumn[row][column]; }
         const void SetValue(const uint32_t row, const uint32_t column, const double value) { _rowColumn[row][column] = value; }

         void Zero(MatrixArray& target) const;
         void Unit();

         void LookAt(const Vector4& position, const Vector4& target, const Vector4& up);
         void Perspective(const double fovY, const double aspect, const double nearZ, const double farZ);
         void Viewport(const double width, const double height);

         void Transfer(const double x, const double y, const double z, bool make);
         void Scale(const double x, const double y, const double z, bool make);
         void RotateX(const double degree, bool make);
         void RotateY(const double degree, bool make);
         void RotateZ(const double degree, bool make);
         void RotateAnyVec(const Vector4 vec, const double degree, bool make);

         const Matrix44 GetRotate() const;

      private:
         std::tuple<double, double> GetSinCos(const double degree) const;

         MatrixArray _rowColumn;
      };
   }
}
