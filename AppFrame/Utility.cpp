
/*****************************************************************//**
 * \file   Utility.cpp
 * \brief  有用算術
 *
 * \author NAOFUMISATO, AHMD2000
 * \date   December 2021
 *********************************************************************/
#include <random>
#include <algorithm>
#include <cmath>
#include <array>
#include <string>
#include "vector4.h"
#include "utility.h"
namespace {
   std::random_device _seedGen;
   std::mt19937_64 random(_seedGen());
}
/**
 * \brief アプリケーションフレーム
 */
namespace AppFrame {
   /**
    * \brief 算術関係
    */
   namespace Math {

      // 矩形と線分の当たり判定
      Collision Utility::CollisionPolygonLine(const Vector4& polygon_point0, const Vector4& polygon_point1,
         const Vector4& polygon_point2, const Vector4& polygon_point3,
         const Vector4& line_start, const Vector4& line_end) {
         // 平面の情報をセット
         const auto normal = GetNormal(polygon_point0, polygon_point1, polygon_point2);
         const auto plane = std::make_tuple(polygon_point0, normal);
         auto result = std::make_tuple(false, Vector4());

         // 平面と線分が交わるか
         if (!CollisionPlaneLine(plane, line_start, line_end, result)) {
            return result;
         }

         // 平面との交点
         const Vector4 point = std::get<1>(result);

         // 矩形を三角形に分けて判定
         if (InsideTrianglePoint(polygon_point0, polygon_point1, polygon_point2, point)) {
            std::get<0>(result) = true;
            std::get<1>(result) = point;

            return result;
         }

         if (InsideTrianglePoint(polygon_point1, polygon_point3, polygon_point2, point)) {
            std::get<0>(result) = true;
            std::get<1>(result) = point;

            return result;
         }

         std::get<0>(result) = false;

         return result;
      }

      // 面の法線を求める
      Vector4 Utility::GetNormal(const Vector4& point0, const Vector4& point1, const Vector4& point2) {
         // 面の法線を求める
         const auto v0 = point1 - point0;
         const auto v1 = point2 - point0;
         auto cross = v0.Cross(v1);

         return cross.Normalize();
      }

      // 三角形と点の内外判定(point は必ず三角形と同平面である事)
      bool Utility::InsideTrianglePoint(const Vector4& triangle_point0, const Vector4& triangle_point1, const Vector4& triangle_point2, const Vector4& point) {
         const auto v01 = triangle_point1 - triangle_point0;
         const auto v12 = triangle_point2 - triangle_point1;
         const auto v21 = triangle_point0 - triangle_point2;
         const auto v0p = point - triangle_point0;
         const auto v1p = point - triangle_point1;
         const auto v2p = point - triangle_point2;

         auto c0 = v01.Cross(v1p);
         auto c1 = v12.Cross(v2p);
         auto c2 = v21.Cross(v0p);

         c0.Normalized();
         c1.Normalized();
         c2.Normalized();

         const auto dot01 = c0.Dot(c1);
         const auto dot02 = c0.Dot(c2);

         return (dot01 > 0.0) && (dot02 > 0.0);
      }

      // 平面と線分の当たり判定
      bool Utility::CollisionPlaneLine(const Plane& p, const Vector4& line_start, const Vector4& line_end, Collision& result) {
         const Vector4 plane_point = std::get<0>(p);
         const Vector4 plane_normal = std::get<1>(p);
         auto point_to_start = line_start - plane_point;
         auto point_to_end = line_end - plane_point;

         point_to_start.Normalized();
         point_to_end.Normalized();

         const auto dot_p_s = plane_normal.Dot(point_to_start);
         const auto dot_p_e = plane_normal.Dot(point_to_end);
         const auto pattern_a = (dot_p_s >= 0.0) && (dot_p_e <= 0.0);
         const auto pattern_b = (dot_p_s <= 0.0) && (dot_p_e >= 0.0);
         const auto ret = pattern_a || pattern_b;

         if (ret) {
            // 平面と線分の交点
            const auto line = line_end - line_start;
            const auto ratio = std::abs(dot_p_s) / std::abs(dot_p_s) + std::abs(dot_p_e);
            const auto point = line_start + (line * ratio);

            std::get<1>(result) = point;
         }

         return ret;
      }

      bool Utility::CollisionSpherePoint(const Vector4& point, const Sphere& s) {
         auto [pos, radian] = s;
         auto sphereFromPoint = pos - point;
         auto [x, y, z] = sphereFromPoint.GetVec3();
         auto checkSize = x * x + y * y + z * z;
         return (checkSize) <= (radian * radian);
      }

      bool Utility::CollisionCapsuleSphere(const Capsule& c, const Sphere& s) {

          auto [cpos1, cpos2, cradian] = c;

          auto [spos, sradian] = s;

          auto startToSphere = spos - cpos1;
          auto startToEnd = cpos2 - cpos1;

          startToEnd.Normalized();

          auto nearLength = startToEnd.Dot(startToSphere);
          auto nearLengthRate = nearLength / startToEnd.Lenght();
          auto near = cpos1 + startToEnd * std::clamp(nearLengthRate, 0.0, 1.0);

          auto distance = 0.0;
          auto endToSphere = spos - cpos2;
          auto nearToSphere = spos - near;

          if (nearLengthRate < 0) {
              auto [x, y, z] = startToSphere.GetVec3();
              distance = x * x + y * y + z * z;
          }
          else if (nearLengthRate > 1) {
              auto [x, y, z] = endToSphere.GetVec3();
              distance = x * x + y * y + z * z;
          }
          else {
              auto [x, y, z] = nearToSphere.GetVec3();
              distance = x * x + y * y + z * z;
          }

          return distance <= (cradian + sradian) * (cradian + sradian);
      }

      bool Utility::CollisionCapsuleCapsule(const Capsule& c1, const Capsule& c2) {
          Vector4 p1, p2;
          double t1, t2;
          auto [start1, end1, r1] = c1;
          auto [start2, end2, r2] = c2;
          auto s1 = std::make_tuple(start1, end1 - start1);
          auto s2 = std::make_tuple(start2, end2 - start2);
          auto checkSize = SegmentSegmentDistance(s1, s2, p1, p2, t1, t2);

          return (checkSize <= (r1 + r2));
      }

      double Utility::PointLineDistance(const Vector4& point, const Line& line, Vector4& h, double& t) {
          auto [linepoint, direction] = line;
          double length = direction.Dot(direction);
          t = 0.0;

          if (length > 0.0) {
              t = direction.Dot(point - linepoint) / length;
          }

          h = linepoint + direction * t;

          return (h - point).Lenght();
      }

      bool Utility::IsSharpAngle(const Vector4& p1, const Vector4& p2, const Vector4& p3) {
          auto vector1 = p1 - p2;
          auto vector2 = p3 - p2;
          if (vector1.Dot(vector2) < 0.0) {
              return false;
          }
          else {
              return true;
          }
      }

      double Utility::PointSegmentDistance(const Vector4& point, const Segment& segment, Vector4& h, double& t) {
          auto [start, direction] = segment;
          auto end = start + direction;

          auto length = PointLineDistance(point, std::make_tuple(start, end - start), h, t);

          if (!IsSharpAngle(point, start, end)) {
              h = start;
              return (start - point).Lenght();
          }
          else if (!IsSharpAngle(point, end, start)) {
              h = end;
              return (end - point).Lenght();
          }

          return length;
      }

      double Utility::LineLineDistance(const Line& l1, const Line& l2, Vector4& p1, Vector4& p2, double& t1, double& t2) {

          auto [l1Point, l1Direction] = l1;
          auto [l2Point, l2Direction] = l2;
          // 2直線が平行だったら点と直線の最短距離を返す
          if (l1Direction.Cross(l2Direction).Lenght() == 0.0) {
              double length = PointLineDistance(l1Point, l2, p2, t2);
              p1 = l1Point;
              t1 = 0.0;

              return length;
          }

          double dotV1V2 = l1Direction.Dot(l2Direction);
          double dotV1V1 = l1Direction.Dot(l1Direction);
          double dotV2V2 = l2Direction.Dot(l2Direction);
          auto l1PointFroml2Point = l1Point - l2Point;
          t1 = (dotV1V2 * l2Direction.Dot(l1PointFroml2Point) - dotV2V2 * l1Direction.Dot(l1PointFroml2Point)) / (dotV1V1 * dotV2V2 - dotV1V2 * dotV1V2);
          p1 = l1Point + l1Direction * t1;
          t2 = l2Direction.Dot(p1 - l2Point) / dotV2V2;
          p2 = l2Point + l2Direction * t2;


          return (p2 - p1).Lenght();
      }

      double Utility::SegmentSegmentDistance(const Segment& s1, const Segment& s2, Vector4& p1, Vector4& p2, double& t1, double& t2) {
          auto [start1, direction1] = s1;
          auto [start2, direction2] = s2;
          if (direction1.Lenght() <= 0.0) {
              if (direction2.Lenght() <= 0.0) {
                  double length = (start2 - start1).Lenght();

                  p1 = start1;
                  p2 = start2;
                  t1 = t2 = 0.0;

                  return length;
              }
              else {
                  double length = PointSegmentDistance(start1, s2, p2, t2);
                  p1 = start1;
                  t1 = 0.0;

                  t2 = std::clamp(t2, 0.0, 1.0);
                  return length;
              }
          }
          else if (direction2.Lenght() <= 0.0) {
              double length = PointSegmentDistance(start2, s1, p1, t1);
              p2 = start2;
              t1 = std::clamp(t1, 0.0, 1.0);
              t2 = 0.0;
              return length;
          }
          //線分同士の判定になった場合
          //平行だったら垂線と線分との交点の1つを線分1の始点に仮決定
          if (direction1.Cross(direction2).Lenght() == 0.0) {
              t1 = 0.0;
              p1 = start1;
              auto length = PointSegmentDistance(start1, s2, p2, t2);
              if (0.0 <= t2 && t2 <= 1.0) {
                  return length;
              }
          }
          else {
              //線分はねじれの関係
              double length = LineLineDistance(s1, s2, p1, p2, t1, t2);
              //線分内に垂線との交点があったらそのまま距離を返す
              if (0.0 <= t1 && t1 <= 1.0 && 0.0 <= t2 && t2 <= 1.0) {
                  return length;
              }
          }
          //垂線との交点が線分外にあった場合
          t1 = std::clamp(t1, 0.0, 1.0);
          p1 = start1 + direction1 * t1;
          auto length = PointSegmentDistance(p1, s2, p2, t2);
          if (0.0 <= t2 && t2 <= 1.0) {
              return length;
          }
          //線分2の垂線との交点が外側にある
          t2 = std::clamp(t2, 0.0, 1.0);
          p2 = start2 + direction2 * t2;
          length = PointSegmentDistance(p2, s1, p1, t1);
          if (0.0 <= t1 && t1 <= 1.0) {
              return length;
          }
          //
          t1 = std::clamp(t1, 0.0, 1.0);
          p1 = start1 + direction1 * t1;
          return (p2 - p1).Lenght();
      }

      unsigned int Utility::GetColorCode(unsigned char red, unsigned char green, unsigned char blue) {
         std::array<unsigned char, 3> color = { red,green,blue };
         std::string redCode;
         std::string greenCode;
         std::string blueCode;
         for (int i = 0; i < color.size(); i++) {
            std::string front;
            auto div = color[i] / 16;
            switch (div) {
            default:
               front = std::to_string(div);
               break;
            case 10:
               front = "A";
               break;
            case 11:
               front = "B";
               break;
            case 12:
               front = "C";
               break;
            case 13:
               front = "D";
               break;
            case 14:
               front = "E";
               break;
            case 15:
               front = "F";
               break;
            }
            std::string back;
            auto rem = color[i] % 16;

            switch (rem) {
            default:
               back = std::to_string(rem);
               break;
            case 10:
               back = "A";
               break;
            case 11:
               back = "B";
               break;
            case 12:
               back = "C";
               break;
            case 13:
               back = "D";
               break;
            case 14:
               back = "E";
               break;
            case 15:
               back = "F";
               break;
            }

            if (i == 0) {
               redCode = front + back;
            }
            if (i == 1) {
               greenCode = front + back;
            }
            if (i == 2) {
               blueCode = front + back;
            }
         }
         auto strCode = "0x" + redCode + greenCode + blueCode;
         unsigned int colorCode = std::stoi(strCode, nullptr, 16);
         return colorCode;
      }

      int Utility::GetRandom(const int min, const int max) {
         std::uniform_int_distribution<> distr(min, max);
         return distr(random);
      }
      double Utility::GetRandom(const double min, const double max) {
         std::uniform_real_distribution<> distr(min, max);
         return distr(random);
      }
      float Utility::GetRandom(const float min, const float max) {
         std::uniform_real_distribution<> distr(min, max);
         return static_cast<float>(distr(random));
      }
      Vector4 Utility::GetRandom(const Vector4 min, const Vector4 max) {
         auto [minX, minY, minZ] = min.GetVec3();
         auto [maxX, maxY, maxZ] = max.GetVec3();
         auto randX = Utility::GetRandom(minX, maxX);
         auto randY = Utility::GetRandom(minY, maxY);
         auto randZ = Utility::GetRandom(minZ, maxZ);
         return Vector4(randX, randY, randZ);
      }
   }
}