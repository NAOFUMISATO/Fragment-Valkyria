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
      static constexpr double DEGREES_180 = 180.0;                                                //!< 度数法180°
      static constexpr double PI = std::numbers::pi;                                              //!< 円周率
      static constexpr double RADIAN_1 = std::numbers::pi / DEGREES_180;

      class Vector4;

      using Plane = std::tuple<Vector4/*位置*/, Vector4/*法線*/>;                                 //!< 平面の定義
      using Sphere = std::tuple<Vector4/*位置*/, double/*半径*/>;                                 //!< 球の定義
      using Line = std::tuple<Vector4/*位置*/, Vector4/*向き*/>;                                  //!< 直線の定義
      using Segment = std::tuple<Vector4/*始点*/, Vector4/*終点へのベクトル*/>;                   //!< 線分の定義
      using Capsule = std::tuple<Vector4/*線分の始点*/, Vector4/*線分の終点*/, double/*半径*/>;   //!< カプセルの定義
      using Collision = std::tuple</*当たり判定結果*/bool, /*交点の位置*/Vector4>;                //!< 当たり判定の結果用のタプル型の定義
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
          * \brief 矩形と線分の当たり判定
          * \param polygonPoint0 矩形頂点1
          * \param polygonPoint1 矩形頂点2
          * \param polygonPoint2 矩形頂点3
          * \param polygonPoint3 矩形頂点4
          * \param lineStart 線分始点
          * \param lineEnd 線分終点
          * \return 当たり判定及びどこが交差したか
          */
         static Collision CollisionPolygonLine(const Vector4& polygonPoint0, const Vector4& polygonPoint1,
            const Vector4& polygonPoint2, const Vector4& polygonPoint3,
            const Vector4& lineStart, const Vector4& lineEnd);
         /**
          * \brief 面法線の計算
          * \param point0 三角形頂点1
          * \param point1 三角形頂点2
          * \param point2 三角形頂点3
          * \return 面法線ベクトル
          */
         static Vector4 GetNormal(const Vector4& point0, const Vector4& point1, const Vector4& point2);
         /**
          * \brief 三角形の内外判定
          * \param trianglePoint0 三角形頂点1
          * \param trianglePoint1 三角形頂点2
          * \param trianglePoint2 三角形頂点3
          * \param point 判定する点
          * \return 内ならばtrue,外ならばfalseを返す
          */
         static bool InsideTrianglePoint(const Vector4& trianglePoint0, const Vector4& trianglePoint1,
            const Vector4& trianglePoint2, const Vector4& point);
         /**
          * \brief 平面と線分の当たり判定
          * \param p 平面位置
          * \param lineStart 線分始点
          * \param lineEnd 線分終点
          * \param result 当たり判定及びどこが交差したか
          * \return 当たり判定結果
          */
         static bool CollisionPlaneLine(const Plane& p, const Vector4& lineStart, const Vector4& lineEnd, Collision& result);
         /**
          * \brief 球と点の当たり判定
          * \param point 点
          * \param sphere 球
          * \return 当たり判定結果
          */
         static bool CollisionSpherePoint(const Vector4& point, const Sphere& sphere);
         /**
          * \brief カプセルと球の当たり判定
          * \param capsule カプセル
          * \param sphere 球
          * \return 当たり判定結果
          */
         static bool CollisionCapsuleSphere(const Capsule& capsule, const Sphere& sphere);
         /**
          * \brief カプセルとカプセルの当たり判定
          * \param capsule1 カプセル1
          * \param capsule2 カプセル2
          * \return 当たり判定結果
          */
         static bool CollisionCapsuleCapsule(const Capsule& capsule1, const Capsule& capsule2);
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
          * \brief 直線と直線の最短距離を求める
          * \param l1 一つ目の直線
          * \param l2 二つ目の直線
          * \param p1 一つ目の直線から二つ目の直線の最短距離への一つ目の直線の点の位置
          * \param p2 二つ目の直線から一つ目の直線の最短距離への二つ目の直線の点の位置
          * \param t1 一つ目の直線から二つ目の直線の最短距離への一つ目の直線の点への割合
          * \param t2 二つ目の直線から一つ目の直線の最短距離への二つ目の直線の点への割合
          * \return 直線と直線の最短距離
          */
         static double LineLineDistance(const Line& l1, const Line& l2, Vector4& p1, Vector4& p2, double& t1, double& t2);
         /**
          * \brief 線分と線分の最短距離を求める
          * \param s1 一つ目の線分
          * \param s2 二つ目の線分
          * \param p1 一つ目の線分から二つ目の線分の最短距離への一つ目の線分の点の位置
          * \param p2 二つ目の線分から一つ目の線分の最短距離への二つ目の線分の点の位置
          * \param t1 一つ目の線分から二つ目の線分の最短距離への一つ目の線分の点への割合
          * \param t2 二つ目の線分から一つ目の線分の最短距離への二つ目の線分の点への割合
          * \return 線分と線分の最短距離
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
         /**
          * \brief 一様分布での乱数の取得(Vector4型)
          * \param min 最小値
          * \param max 最大値
          * \return 最小値と最大値の範囲での一様分布での乱数
          */
         static Vector4 GetRandom(const Vector4 min, const Vector4 max);

      private:
         /**
          * \brief デフォルトデストラクタ
          */
         Utility() = default;
      };
   }
}