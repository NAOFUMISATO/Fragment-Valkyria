#pragma once
/*****************************************************************//**
 * \file   Utility.h
 * \brief  有用算術クラス
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include <tuple>
#include <numbers>
#include <cmath>
 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   /**
    * \brief 算術関係
    */
   namespace Math {
      static constexpr double DEGREES_180 = 180.0;    //!< 度数法180°
      static constexpr double PI = std::numbers::pi;  //!< 円周率

      class Vector4;

      using plane = std::tuple<Vector4/*point*/, Vector4/*normal*/>;
      using collision = std::tuple<bool, Vector4>;


       * \class 有用算術クラス
       * \brief 全て静的メンバで構成
       */
      class Utility {
      public:
         /**
          * \brief 弧度法から度数法への変換
          * \param degree 弧度法での値
          * \return 度数法での値
          */
         static double DegreeToRadian(double degree) {
            return degree * PI / DEGREES_180;
         }
         /**
          * \brief 度数方から弧度法への変換
          * \param radion 度数法での値
          * \return 弧度法での値
          */
         static double RadianToDegree(double radion) {
            return radion * DEGREES_180 / PI;
         }
         /**
          * \brief 一様分布での乱数の取得(int型)
          * \param min 最小値
          * \param max 最大値
          * \return 最小値と最大値の範囲での一様分布での乱数
          */
         static int GetRandom(const int min, const int max);
         /**
          * \brief 一様分布での乱数の取得(double型)
          * \param min 最小値
          * \param max 最大値
          * \return 最小値と最大値の範囲での一様分布での乱数
          */
         static double GetRandom(const double min, const double max);
         /**
          * \brief 一様分布での乱数の取得(float型)
          * \param min 最小値
          * \param max 最大値
          * \return 最小値と最大値の範囲での一様分布での乱数
          */
         static float GetRandom(const float min, const float max);

         // 矩形と線分の当たり判定
         static Collision CollisionPolygonLine(const Vector4& polygonPoint0, const Vector4& polygonPoint1,
            const Vector4& polygonPoint2, const Vector4& polygonPoint3,
            const Vector4& lineStart, const Vector4& lineEnd);

         // 面の法線を求める
         static Vector4 GetNormal(const Vector4& point0, const Vector4& point1, const Vector4& point2);

         // 三角形と点の内外判定(point は必ず三角形と同平面である事)
         static bool InsideTrianglePoint(const Vector4& trianglePoint0, const Vector4& trianglePoint1,
            const Vector4& trianglePoint2, const Vector4& point);
         // 平面と線分の辺り判定
         static bool CollisionPlaneLine(const Plane& p, const Vector4& lineStart, const Vector4& lineEnd, Collision& result);
         //球と点の当たり判定
         static bool CollisionSpherePoint(const Vector4& point, const Sphere& s);
         /**
         * \brief 指定したRGBのカラーコードの取得
         * \param r 赤値
         * \param g 緑値
         * \param b 青値
         * \return カラーコード
         static bool CollisionPlaneLine(const plane& p, const Vector4& lineStart, const Vector4& lineEnd, collision& result);
         static bool CollisionSpherePoint(const Vector4& point, const Sphere& s);

      private:
         Utility() = default;
      };
   }
}