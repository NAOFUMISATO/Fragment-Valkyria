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
      //　二重インクルード防止
      class Matrix44;
      /**
       * \class 4次元ベクトルクラス
       * \brief ベクトル計算を行う自作クラス
       */
      class Vector4 {
      public:
         /**
          * \brief 空のコンストラクタ
          */
         Vector4();
         /**
          * \brief コンストラクタ
          * \param x x成分
          * \param y y成分
          * \param z z成分
          */
         Vector4(const double x, const double y, const double z) : _x(x), _y(y), _z(z), _w(1.0) {}
         /**
          * \brief コンストラクタ
          * \param x x成分
          * \param y y成分
          * \param z z成分
          * \param w w成分
          */
         Vector4(const double x, const double y, const double z, double w) : _x(x), _y(y), _z(z), _w(w) {}
         /**
          * \brief コピーコンストラクタ
          */
         Vector4(const Vector4&) = default;
         /**
          * \brief ムーブコンストラクタ
          */
         Vector4(Vector4&&) = default;
         /**
          * \brief デフォルトデストラクタ
          */
         virtual ~Vector4() = default;
         /**
          * \brief コピー
          */
         Vector4& operator =(const Vector4&) = default;
         /**
          * \brief ムーブ
          */
         Vector4& operator =(Vector4&&) = default;
         /**
          * \brief 比較演算子のオーバーロード
          * \param rhs 比較するVector4クラス
          * \return 比較結果
          */
         bool operator <(const Vector4 rhs) const;
         /**
          * \brief Vector4とVector4の演算子のオーバーロード(+)
          * \param rhs Vector4クラス
          * \return 計算結果
          */
         const Vector4 operator +(const Vector4& rhs) const;
         /**
          * \brief Vector4とVector4の演算子のオーバーロード(-)
          * \param rhs Vector4クラス
          * \return 計算結果
          */
         const Vector4 operator -(const Vector4& rhs) const;
         /**
          * \brief Vector4とVector4の演算子のオーバーロード(*)
          * \param rhs Vector4クラス
          * \return 計算結果
          */
         const Vector4 operator *(const Vector4& rhs) const;
         /**
          * \brief Vector4とdouble型の演算子のオーバーロード(*)
          * \param rhs doubleの値
          * \return 計算結果
          */
         const Vector4 operator *(const double rhs) const;
         /**
          * \brief Vector4とdouble型の演算子のオーバーロード(/)
          * \param rhs doubleの値
          * \return 計算結果
          */
         const Vector4 operator /(const double rhs) const;
         /**
          * \brief Vector4とMatrix44の演算子のオーバーロード(*)
          * \param rhs Matrix44クラス
          * \return 計算結果
          */
         const Vector4 operator *(const Matrix44 rhs) const;
         /**
          * \brief Vector4クラスの設定
          * \param vector 設定するVector4クラス
          */
         void Set(const Vector4& vector);
         /**
          * \brief Vector4クラスの設定
          * \param x 設定するx成分
          * \param y 設定するy成分
          * \param z 設定するz成分
          */
         void Set(const double x, const double y, const double z);
         /**
          * \brief Vector4クラスの加算
          * \param vector 加算するVector4クラス
          */
         void Add(const Vector4& vector);
         /**
          * \brief Vector4クラスの加算
          * \param x 加算するx成分
          * \param y 加算するy成分
          * \param z 加算するz成分
          */
         void Add(const double x, const double y, const double z);
         /**
          * \brief Vector4クラスの3次元成分の取得
          * \return xyz成分
          */
         inline const std::tuple<double, double, double> GetVec3() const { return { _x, _y, _z }; }
         /**
          * \brief Vector4クラスの3次元成分の設定
          * \param x x成分
          * \param y y成分
          * \param z z成分
          */
         inline void SetVec3(double x, double y, double z) { _x = x, _y = y, _z = z; }
         /**
          * \brief Vector4クラスの2次元成分の取得
          * \return xy成分
          */
         inline const std::pair<double, double> GetVec2() const { return { _x,_y }; }
         /**
          * \brief Vector4クラスの2次元成分の設定
          * \param x x成分
          * \param y y成分
          */
         inline void SetVec2(double x, double y) { _x = x, _y = y; }
         /**
          * \brief y成分の取得
          * \return y成分
          */
         inline const double GetY() const { return _y; }
         /**
         * \brief z成分の取得
         * \return z成分
         */
         inline const double GetZ() const { return _z; }
         /**
          * \brief w成分の取得
          * \return w成分
          */
         inline const double GetW() const { return _w; }
         /**
          * \brief y成分の取得
          * \return y成分
          */
         inline void SetY(double y) { _y = y; }
         /**
          * \brief ベクトルの長さの取得
          * \return ベクトルの長さ
          */
         const double Lenght() const;
         /**
          * \brief 正規化を行う
          */
         void Normalized();
         /**
          * \brief 正規化した値を戻り値として返す
          * \return 正規化した値
          */
         const Vector4 Normalize() const;
         /**
          * \brief 内積の取得
          * \param rhs 取得するベクトル
          * \return 内積
          */
         const double Dot(const Vector4& rhs) const;
         /**
          * \brief 外積の取得
          * \param rhs 取得するベクトル
          * \return 外積
          */
         const Vector4 Cross(const Vector4& rhs) const;

      private:
         double _x;    //!< x成分
         double _y;    //!< y成分
         double _z;    //!< z成分
         double _w;    //!< w成分
      };
   }
}
