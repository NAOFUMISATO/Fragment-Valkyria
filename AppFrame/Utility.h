#pragma once
/*****************************************************************//**
 * \file   Utility.h
 * \brief  有用算術クラス
 *
 * \author NAOFUMISATO, AHMD2000
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
      static constexpr double RADIAN_1 = std::numbers::pi / DEGREES_180;

      class Vector4;

      using Plane = std::tuple<Vector4/*point*/, Vector4/*normal*/>;
      using Sphere = std::tuple<Vector4/*position*/, double/*radian*/>;
      using Line = std::tuple<Vector4/*位置*/, Vector4/*向き*/>;
      using Segment = std::tuple<Vector4/*始点*/, Vector4/*終点へのベクトル*/>;
      using Capsule = std::tuple<Vector4/*線分の始点*/, Vector4/*線分の終点*/, double/*半径*/>;
      using Collision = std::tuple<bool, Vector4>;

      /**
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
          * \brief カプセルと球の当たり判定
          * \param c
          * \param s
          * \return 当たっているか
          */
         static bool CollisionCapsuleSphere(const Capsule& c, const Sphere& s);
         /**
          * \brief カプセルとカプセルの当たり判定
          * \param c1
          * \param c2
          * \return 当たっているか
          */
         static bool CollisionCapsuleCapsule(const Capsule& c1, const Capsule& c2);
         /**
          * \brief 点と直線の最短距離をもとめる
          * \param point 点の位置ベクトル
          * \param line 直線
          * \param h 点から直線へ降ろした垂線と直線との交点(戻り値)
          * \param t 点から直線へ降ろした垂線と直線との交点へのベクトル係数(戻り値)
          * \return 点と直線の最短距離
          */
         static double PointLineDistance(const Vector4& point, const Line& line, Vector4& h, double& t);
         /**
          * \brief p2からp1とp3に伸ばしたベクトルでできた角度が鋭角か
          * \param p1 1つめの終点
          * \param p2 始点
          * \param p3 2つめの終点
          * \return 鋭角かどうか
          */
         static bool IsSharpAngle(const Vector4& p1, const Vector4& p2, const Vector4& p3);
         /**
          * \brief 点と線分の最短距離をもとめる
          * \param point 点の位置ベクトル
          * \param segment 線分
          * \param h 点から線分へ降ろした垂線と線分との交点(戻り値)
          * \param t 点から線分へ降ろした垂線と線分との交点へのベクトル係数(戻り値)
          * \return 点と線分の最短距離
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
         * \brief 指定したRGBのカラーコードの取得
         * \param r 赤値
         * \param g 緑値
         * \param b 青値
         * \return カラーコード
         */
         static unsigned int GetColorCode(unsigned char red, unsigned char green, unsigned char blue);

      private:
         Utility() = default;
      };
   }
}