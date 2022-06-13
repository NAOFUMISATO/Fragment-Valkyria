#pragma once
/*****************************************************************//**
 * \file   Matrix44.h
 * \brief  4次元行列クラス
 *
 * \author NAOFUMISATO, YUSUKEAHAMEDO
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
      // 二重インクルード防止
      class Vector4;

      constexpr uint32_t _rowMax = 4;     //!< 行数
      constexpr uint32_t _columnMax = 4;  //!< 列数
      using MatrixArray = std::array<std::array<double, _columnMax>, _rowMax>;   //!< 行列表現の二次配列
      using Quaternion = std::tuple<double, double, double, double>;                     //!< クォータニオンを表すtuple型
      /**
       * \class 4次元行列クラス
       * \brief 行列計算を行う自作クラス
       */
      class Matrix44 {
      public:
         /**
          * \brief 空のコンストラクタ
          */
         Matrix44();
         /**
          * \brief コンストラクタ
          * \param values 行列表現の二次配列
          */
         Matrix44(const MatrixArray values);
         /**
          * \brief コピーコンストラクタ
          */
         Matrix44(const Matrix44&) = default;
         /**
          * \brief ムーブコンストラクタ
          */
         Matrix44(Matrix44&&) = default;
         /**
          * \brief デストラクタ
          */
         virtual ~Matrix44() = default;
         /**
          * \brief コピー
          */
         Matrix44& operator =(const Matrix44&) = default;
         /**
          * \brief ムーブ
          */
         Matrix44& operator =(Matrix44&&) = default;
         /**
          * \brief Matrix44とMatrix44計算の演算子のオーバーロード(*)
          * \param rhs Matrix44クラス
          * \return 計算結果
          */
         const Matrix44 operator *(const Matrix44 rhs) const;
         /**
          * \brief Matrix44とVector4計算の演算子のオーバーロード(+)
          * \param rhs Vector4クラス
          * \return 計算結果
          */
         const Matrix44 operator +(const Vector4 rhs) const;
         /**
          * \brief 行列の値の取得
          * \param row 行
          * \param column 列
          * \return 行列の値
          */
         const double GetValue(const uint32_t row, const uint32_t column) const { return _rowColumn[row][column]; }
         /**
          * \brief 行列の値の設定
          * \param row 行
          * \param column 列
          * \param value 設定する値
          * \return 行列の値
          */
         const void SetValue(const uint32_t row, const uint32_t column, const double value) { _rowColumn[row][column] = value; }
         /**
          * \brief 単位行列を乗算
          * \param target 単位行列を掛ける行列
          */
         void Zero(MatrixArray& target) const;
         /**
          * \brief 単位行列の取得
          */
         void Unit();
         /**
          * \brief カメラのビュー行列の設定
          * \param position カメラ位置ベクトル
          * \param target 注視点のベクトル
          * \param up カメラの上方向ベクトル
          */
         void LookAt(const Vector4& position, const Vector4& target, const Vector4& up);
         /**
          * \brief カメラの透視変換の設定
          * \param fov 視野角
          * \param aspect アスペクト比
          * \param nearZ カメラに最も近い表示限界
          * \param farZ カメラに最も遠い表示限界
          */
         void Perspective(const double fov, const double aspect, const double nearZ, const double farZ);
         /**
          * \brief 表示領域の設定
          * \param width　横領域サイズ
          * \param height　縦領域サイズ
          */
         void Viewport(const double width, const double height);
         /**
          * \brief 平行行列を乗算
          * \param x x成分
          * \param y y成分
          * \param z z成分
          * \param make 単位化するか
          */
         void Transfer(const double x, const double y, const double z, bool make);
         /**
          * \brief 拡大行列を乗算
          * \param x x成分
          * \param y y成分
          * \param z z成分
          * \param make 単位化するか
          */
         void Scale(const double x, const double y, const double z, bool make);
         /**
          * \brief X軸回転行列を乗算
          * \param degree x成分
          * \param make 単位化するか
          */
         void RotateX(const double degree, bool make);
         /**
          * \brief Y軸回転行列を乗算
          * \param degree y成分
          * \param make 単位化するか
          */
         void RotateY(const double degree, bool make);
         /**
          * \brief Z軸回転行列を乗算
          * \param degree z成分
          * \param make 単位化するか
          */
         void RotateZ(const double degree, bool make);
         /**
          * \brief ベクトルの任意軸での回転
          * \param vec 回転軸を取得する位置
          * \param degree 回転角
          * \param make 単位化するか
          */
         void RotateAnyVec(const Vector4 vec, const double degree, bool make);
         /**
          * \brief 
          * \param vec
          * \param degree
          * \param make
          */
         void RotateAnyVecQuaternion(const Vector4 vec, const double degree, bool make);
         /**
          * \brief 回転行列を乗算
          * \return 乗算した行列
          */
         const Matrix44 GetRotate() const;

      private:
         /**
          * \brief 度数法へ変換したsin,cosの取得
          * \param degree 度数法の回転角
          * \return 度数法へ変換したsin,cos
          */
         std::tuple<double, double> GetSinCos(const double degree) const;
         MatrixArray _rowColumn;   //!< 行列表現の二次配列
      };
   }
}
