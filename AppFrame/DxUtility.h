#pragma once
/*****************************************************************//**
 * \file   DxUtility.h
 * \brief  DxLib関数用算術クラス
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include <DxLib.h>
 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   /**
    * \brief 算術関係
    */
   namespace Math {
      struct tagVECTOR;
      struct tagMATRIX;
      class Vector4;
      class Matrix44;

      VECTOR ToDX(Math::Vector4& vector);

      Math::Vector4 ToMath(VECTOR& vector);

      MATRIX ToDX(Math::Matrix44& matrix);

      Math::Matrix44 ToMath(MATRIX& matrix);
   }
}