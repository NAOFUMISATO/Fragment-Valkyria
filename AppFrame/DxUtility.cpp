
/*****************************************************************//**
 * \file   DxUtility.cpp
 * \brief  DxLibä÷êîópéZèp
 *
 * \author NAOFUMISATO, YUSUKEAHAMEDO
 * \date   December 2021
 *********************************************************************/
#include "DxUtility.h"
#include <DxLib.h>
#include "Vector4.h"
#include "Matrix44.h"
 /**
  * \brief ÉAÉvÉäÉPÅ[ÉVÉáÉìÉtÉåÅ[ÉÄ
  */
namespace AppFrame {
   /**
    * \brief éZèpä÷åW
    */
   namespace Math {
      VECTOR ToDX(Math::Vector4& vector) {
         auto [x, y, z] = vector.GetVec3();
         auto cx = static_cast<float>(x);
         auto cy = static_cast<float>(y);
         auto cz = static_cast<float>(z);
         return std::move(VGet(cx, cy, cz));
      }

      Math::Vector4 ToMath(VECTOR& vector) {
         auto x = static_cast<double>(vector.x);
         auto y = static_cast<double>(vector.y);
         auto z = static_cast<double>(vector.z);
         return std::move(Math::Vector4(x, y, z));
      }

      MATRIX ToDX(Math::Matrix44& matrix) {
         MATRIX ret;
         for (auto i = 0; i < Math::_rowMax; ++i) {
            for (auto j = 0; j < Math::_columnMax; ++j) {
               ret.m[i][j] = static_cast<float>(matrix.GetValue(i, j));
            }
         }
         return ret;
      }

      Math::Matrix44 ToMath(MATRIX& matrix) {
         Math::Matrix44 ret;
         for (auto i = 0; i < Math::_rowMax; ++i) {
            for (auto j = 0; j < Math::_columnMax; ++j) {
               ret.SetValue(i, j, static_cast<double>(matrix.m[i][j]));
            }
         }
         return ret;
      }
   }
}